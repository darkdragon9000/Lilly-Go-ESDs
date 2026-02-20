# Wireless Digital Price Tag System

A web-based price management system for retail store displays, built with Node.js and ESP32-powered LilyGo T-Display S3 screens. Staff can update prices across all displays instantly from any browser — no technical knowledge required.

Built as part of an E4USA engineering entrepreneurship class for a school store client.

---

## How It Works

1. The Node.js server runs on a computer or Raspberry Pi connected to the store's WiFi network
2. Staff open the admin panel in any browser and click a display to update its price
3. Each ESP32 display polls the server every 30 seconds and refreshes automatically

---

## Hardware Required

- [LilyGo T-Display S3](https://www.lilygo.cc/products/t-display-s3) (one per price tag)
- Raspberry Pi 4B (recommended for always-on hosting) or any computer
- WiFi router/network that all devices share

---

## Project Structure

```
PriceDisplays/
├── server.js          ← Node.js/Express backend + REST API
├── package.json       ← Project dependencies
├── package-lock.json  ← Locked dependency versions
├── public/
│   ├── index.html     ← Admin panel UI
│   ├── style.css      ← Styling
│   └── app.js         ← Frontend JavaScript
└── PriceDisplay/
    └── PriceDisplay.ino  ← ESP32 Arduino firmware
```

---

## Setup: Web Server

### Prerequisites
- [Node.js](https://nodejs.org/) (v18 or higher)
- [npm](https://www.npmjs.com/) (comes with Node.js)

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/PriceDisplays.git
   cd PriceDisplays
   ```

2. Install dependencies:
   ```bash
   npm install
   ```

3. Start the server:
   ```bash
   node server.js
   ```

4. Open the admin panel in your browser:
   ```
   http://localhost:3000
   ```

To access from other devices on the same network, use the host machine's IP address instead of `localhost` (e.g., `http://192.168.1.10:3000`).

### Running on Raspberry Pi (Recommended)

For always-on hosting, deploy to a Raspberry Pi 4B using PM2 so the server auto-starts on boot:

```bash
sudo npm install -g pm2
pm2 start server.js --name price-tag-server
pm2 startup    # run the command it outputs to enable autostart
pm2 save
```

You can also manage the Pi remotely via [Raspberry Pi Connect](https://connect.raspberrypi.com/) without needing SSH.

---

## Setup: ESP32 Displays

### Prerequisites
- [Arduino IDE](https://www.arduino.cc/en/software)
- LilyGo T-Display S3 board support installed in Arduino IDE
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library installed

### Configuration

Before flashing, open `PriceDisplay/PriceDisplay.ino` and update these values at the top of the file:

```cpp
const char* ssid = "YOUR_WIFI_NETWORK";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverURL = "http://192.168.1.10:3000";  // Your server's IP
const int displayID = 1;  // Unique ID for each display (1, 2, 3...)
```

Give each display a unique `displayID` before flashing. Flash each display individually via USB using Arduino IDE.

---

## Usage

1. Open the admin panel in a browser
2. You'll see a grid of all registered displays and their current prices
3. Click any display to edit its price
4. The change saves immediately — the display will refresh within 30 seconds

---

## Power Optimization

The displays are optimized for the store's 1.5-hour daily operating window. To maximize battery life:
- The polling interval can be adjusted in `PriceDisplay.ino`
- Screen brightness is set conservatively by default

---

## License

MIT License — free to use and modify.
