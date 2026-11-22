# ===============================
# ESP32 Wi-Fi Scanner - MicroPython
# ===============================

import network
import time

# ===============================
# Wi-Fi mode configuration
# ===============================
# Create a WLAN object in Station mode
wlan = network.WLAN(network.STA_IF)
wlan.active(True)          # Activate station mode
wlan.disconnect()          # Disconnect from any previous network
print("ESP32 Wi-Fi Scanner starting...")
print("ESP32 is now in Station mode (ready to scan).")

# ===============================
# Function: Convert RSSI to ASCII bar
# Visual indicator of signal strength
# ===============================
def rssi_bar(rssi):
    bars = int((rssi + 100) / 5)  # Map RSSI (-100 to -50) to 0–10
    bars = max(0, min(bars, 10))
    return "|" * bars

# ===============================
# Function: Decode authmode to string
# ===============================
def authmode_to_string(authmode):
    if authmode == 0:
        return "OPEN"
    elif authmode == 1:
        return "WEP"
    elif authmode == 2:
        return "WPA-PSK"
    elif authmode == 3:
        return "WPA2-PSK"
    elif authmode == 4:
        return "WPA/WPA2-PSK"
    elif authmode == 5:
        return "WPA2-Enterprise"
    else:
        return "UNKNOWN"

# ===============================
# Function: Scan Wi-Fi networks
# Includes sorting and filters
# ===============================
def scan_networks():
    # Print timestamp for scan
    t = time.localtime()
    print("\n--- Wi-Fi Scan at {:02d}:{:02d}:{:02d} ---".format(t[3], t[4], t[5]))

    nets = wlan.scan()  # Returns a list of tuples: (ssid, bssid, channel, RSSI, authmode, hidden)
    n = len(nets)
    print("Number of networks found:", n)

    if n == 0:
        print("No networks found")
        return

    # Convert tuples to dictionaries for easier handling
    networks = []
    for net in nets:
        networks.append({
            'ssid': net[0].decode('utf-8'),  # SSID as string
            'rssi': net[3],
            'channel': net[2],
            'encryption': net[4]
        })

    # ===============================
    # Sort networks by RSSI (strongest first)
    # ===============================
    networks.sort(key=lambda x: x['rssi'], reverse=True)

    # ===============================
    # Print all networks (sorted)
    # ===============================
    print("\n--- All Networks (sorted by RSSI) ---")
    for i, net in enumerate(networks):
        print("[{}] SSID: {} | RSSI: {} dBm {} | CH: {} | ENC: {}".format(
            i,
            net['ssid'],
            net['rssi'],
            rssi_bar(net['rssi']),
            net['channel'],
            authmode_to_string(net['encryption'])
        ))

    # ===============================
    # Filter: Open networks only
    # ===============================
    print("\n--- Open Networks Only ---")
    found_open = False
    for i, net in enumerate(networks):
        if net['encryption'] == 0:  # 0 = OPEN
            print("[{}] SSID: {} | RSSI: {} dBm {} | CH: {} | ENC: {}".format(
                i,
                net['ssid'],
                net['rssi'],
                rssi_bar(net['rssi']),
                net['channel'],
                authmode_to_string(net['encryption'])
            ))
            found_open = True
    if not found_open:
        print("No open networks found.")

    # ===============================
    # Filter: Channels 1, 6, 11
    # ===============================
    print("\n--- Networks on Channels 1, 6, 11 ---")
    found_channel = False
    for i, net in enumerate(networks):
        if net['channel'] in [1, 6, 11]:
            print("[{}] SSID: {} | RSSI: {} dBm {} | CH: {} | ENC: {}".format(
                i,
                net['ssid'],
                net['rssi'],
                rssi_bar(net['rssi']),
                net['channel'],
                authmode_to_string(net['encryption'])
            ))
            found_channel = True
    if not found_channel:
        print("No networks found on channels 1, 6, 11.")

# ===============================
# Main loop: Repeat scan every 10 seconds
# ===============================
while True:
    scan_networks()
    time.sleep(10)
