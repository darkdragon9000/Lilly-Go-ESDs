const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
const PORT = 3000;
const DATA_FILE = 'displays.json';

// Middleware
app.use(express.json());
app.use(express.static('public'));

// Initialize data file if it doesn't exist
if (!fs.existsSync(DATA_FILE)) {
    const initialData = [];
    for (let i = 1; i <= 10; i++) {
        initialData.push({
            id: i,
            name: `Item ${i}`,
            price: 0.00
        });
    }
    fs.writeFileSync(DATA_FILE, JSON.stringify(initialData, null, 2));
}

// Get all displays
app.get('/api/displays', (req, res) => {
    const data = JSON.parse(fs.readFileSync(DATA_FILE));
    res.json(data);
});

// Update a display
app.put('/api/displays/:id', (req, res) => {
    const id = parseInt(req.params.id);
    const { name, price } = req.body;
    
    let data = JSON.parse(fs.readFileSync(DATA_FILE));
    const index = data.findIndex(d => d.id === id);
    
    if (index !== -1) {
        data[index].name = name;
        data[index].price = price;
        fs.writeFileSync(DATA_FILE, JSON.stringify(data, null, 2));
        res.json({ success: true, display: data[index] });
    } else {
        res.status(404).json({ error: 'Display not found' });
    }
});

// Get single display (for the ESP32 devices)
app.get('/api/displays/:id', (req, res) => {
    const id = parseInt(req.params.id);
    const data = JSON.parse(fs.readFileSync(DATA_FILE));
    const display = data.find(d => d.id === id);
    
    if (display) {
        res.json(display);
    } else {
        res.status(404).json({ error: 'Display not found' });
    }
});

// app.listen(PORT, '172.20.10.2', () => {
//     console.log(`Server running at http://172.20.10.2:${PORT}`);
// });

// app.listen(PORT, '192.168.68.63', () => {
  //  console.log(`Server running at http://192.168.68.63:${PORT}`);
 //});
 
 app.listen(PORT, '0.0.0.0', () => {
    console.log(`Server running at http://localhost:${PORT}`);
});