
import glob
import time

import requests
import serial

# ---------- CONFIG ----------
LAT, LON, RADIUS_NM = 37.44, -122.15, 15   # <- your coordinates
RADIUS_NM = 15
PORT = "/dev/cu.usbmodem12401"   # falls back to auto-detect if not found
BAUD = 9600          # must match Serial.begin() in the sketch
POLL_SECONDS = 15
MAX_RETRIES = 3
# ----------------------------


def open_serial():
    port = PORT
    if not glob.glob(port):
        found = glob.glob("/dev/cu.usbmodem*")
        if not found:
            raise SystemExit("No Arduino found. Check USB cable and close the Serial Monitor.")
        port = found[0]
    print("Opening", port)
    ser = serial.Serial(port, BAUD, timeout=1)
    time.sleep(3)  # the Mega resets when the port opens
    ser.reset_input_buffer()
    return ser


ser = open_serial()


def send_frame(line1, line2):
    """Send '@<len>:<line1>|<line2>' and wait for the Mega's OK/BAD reply."""
    payload = f"{line1[:16]}|{line2[:16]}"
    payload = payload.encode("ascii", errors="replace").decode("ascii")
    frame = f"@{len(payload)}:{payload}\n"

    for attempt in range(1, MAX_RETRIES + 1):
        ser.reset_input_buffer()
        ser.write(frame.encode("ascii"))
        ser.flush()
        print(f"sent (try {attempt}):", repr(frame))

        deadline = time.time() + 2
        while time.time() < deadline:
            reply = ser.readline().decode("ascii", errors="replace").strip()
            if not reply:
                continue
            print("  mega:", reply)
            if reply.startswith("OK"):
                return True
            if reply.startswith("BAD"):
                break          # retry
        time.sleep(0.3)

    print("  giving up on this update")
    return False


def closest_plane():
    url = f"https://api.adsb.lol/v2/point/{LAT}/{LON}/{RADIUS_NM}"
    data = requests.get(url, timeout=10).json()
    planes = [p for p in data.get("ac", []) if p.get("flight", "").strip()]
    if not planes:
        return None
    return min(planes, key=lambda p: p.get("dst", 999))


def get_route(callsign):
    try:
        r = requests.get(f"https://api.adsbdb.com/v0/callsign/{callsign}", timeout=10)
        fr = r.json()["response"]["flightroute"]
        return f'{fr["origin"]["iata_code"]}>{fr["destination"]["iata_code"]}'
    except Exception:
        return "???"


def format_altitude(alt):
    if alt == "ground":
        return "GND"
    if isinstance(alt, (int, float)):
        return f"{int(alt)}ft"
    return "?"


send_frame("Flight tracker", "starting...")

last_callsign = None
route = "???"

try:
    while True:
        try:
            plane = closest_plane()
            if plane is None:
                send_frame("No planes", "nearby")
                last_callsign = None
            else:
                callsign = plane["flight"].strip()
                if callsign != last_callsign:
                    route = get_route(callsign)
                    last_callsign = callsign
                aircraft = plane.get("t", "?")
                alt = format_altitude(plane.get("alt_baro"))
                send_frame(f"{callsign} {aircraft}", f"{route} {alt}")
        except Exception as e:
            print("error:", e)
        time.sleep(POLL_SECONDS)
except KeyboardInterrupt:
    print("Stopping.")
finally:
    ser.close()