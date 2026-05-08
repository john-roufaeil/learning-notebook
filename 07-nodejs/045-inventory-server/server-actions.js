const fs = require('fs');
const path = require('path');

// Test Connection
function testConnection(res) {
    res.writeHead(200, { 'content-type': 'text/plain' });
    res.write("Server is running and listening.\n");
    res.end();
}

// Return a static resource
function returnResource(res, pathInput, contentType) {
    const reader = fs.createReadStream(path.join(__dirname, 'public', pathInput));
    res.writeHead(200, { 'content-type': contentType });
    reader.pipe(res);
}

// List Inventory
function listInventory(res) {
    res.writeHeader(200, { 'content-type': 'text/html' });

    const jsonPath = path.join(__dirname, '..', '044-inventory-script', 'todos.json');
    const htmlPath = path.join(__dirname, 'public', 'homepage.html');

    const items = JSON.parse(fs.readFileSync(jsonPath, 'utf-8')).data;
    let itemsHtml = "";

    items.forEach(item => {
        itemsHtml += `<p>${item.name} under category ${item.category} with quantity ${item.quantity}</p>\n`;
    });

    let html = fs.readFileSync(htmlPath, 'utf-8');
    html = html.replace('INVENTORY_ITEMS', itemsHtml);

    res.end(html);
}

// Add Item to Inventory
function addItem(req, res) {
    res.writeHead(200);

    const jsonPath = path.join(__dirname, '..', '044-inventory-script', 'todos.json');
    const parsed = JSON.parse(fs.readFileSync(jsonPath, 'utf-8'));
    const itemsId = parsed.nextItemId;
    const items = parsed.data;

    let body = "";
    req.on('data', chunk => body += chunk);
    req.on('end', () => {
        const parsedBody = JSON.parse(body);
        const newItem = {
            name: parsedBody.name,
            quantity: 1,
            category: "General",
            id: itemsId
        };
        console.log("body", body)
        items.push(newItem);
        fs.writeFileSync(jsonPath, `{"data": ${JSON.stringify(items)}, "nextItemId": ${itemsId + 1}}`)
        res.end();
    })
}

module.exports = {
    testConnection,
    listInventory,
    addItem,
    returnResource
}
