// Load all displays when page opens
document.addEventListener('DOMContentLoaded', loadDisplays);

// Handle form submission
document.getElementById('edit-form').addEventListener('submit', saveDisplay);

async function loadDisplays() {
    try {
        const response = await fetch('/api/displays');
        const displays = await response.json();
        
        const grid = document.getElementById('displays-grid');
        grid.innerHTML = '';
        
        displays.forEach(display => {
            const card = document.createElement('div');
            card.className = 'display-card';
            card.onclick = () => openModal(display);
            
            card.innerHTML = `
                <div class="display-number">Display #${display.id}</div>
                <div class="display-name">${display.name}</div>
                <div class="display-price">$${display.price.toFixed(2)}</div>
            `;
            
            grid.appendChild(card);
        });
    } catch (error) {
        console.error('Error loading displays:', error);
    }
}

function openModal(display) {
    document.getElementById('edit-id').value = display.id;
    document.getElementById('edit-name').value = display.name;
    document.getElementById('edit-price').value = display.price;
    document.getElementById('modal').classList.add('active');
}

function closeModal() {
    document.getElementById('modal').classList.remove('active');
}

async function saveDisplay(e) {
    e.preventDefault();
    
    const id = document.getElementById('edit-id').value;
    const name = document.getElementById('edit-name').value;
    const price = parseFloat(document.getElementById('edit-price').value);
    
    try {
        const response = await fetch(`/api/displays/${id}`, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ name, price })
        });
        
        if (response.ok) {
            closeModal();
            loadDisplays();
        } else {
            alert('Error saving changes');
        }
    } catch (error) {
        console.error('Error saving display:', error);
        alert('Error saving changes');
    }
}

// Close modal when clicking outside
document.getElementById('modal').addEventListener('click', function(e) {
    if (e.target === this) {
        closeModal();
    }
});