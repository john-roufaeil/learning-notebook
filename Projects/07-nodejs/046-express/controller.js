const fs = require('fs');
const path = require('path');
const products = require('./products.json');
const filePath = path.join(__dirname, "products.json");

const testConnection = (req, res) => {
    res.send('Hello, World!')
}

const getHomepage = (req, res) => {
    res.render(
        'index',
        {
            title: 'Homepage',
            message: "Products",
            products: products
        }
    )
}

const getBalloonPage = (req, res) => {
    res.sendFile(path.join(__dirname, "public", "balloon.html"));
}

const getProducts = (req, res) => {
    const { status, category } = req.query;
    let filtered = products;
    if (status) {
        filtered = filtered.filter(product => {
            const currStatus = product.quantity > 2 ?
                "available" : product.quantity > 0 ?
                    "low-stock" : "out-of-stock";
            return currStatus === status.toLowerCase()
        });
    }
    if (category) {
        filtered = filtered.filter(product =>
            product.category.toLowerCase() === category.toLowerCase()
        );
    }
    return res.json(filtered);
}

const getProduct = (req, res) => {
    const { id } = req.params;
    const product = products.find(product => product.id === Number(id));
    if (!product) res.status(404).send("Product not found");
    return res.json(product);
}

const addProduct = (req, res) => {
    const { name, category, id, quantity } = req.body;

    const existingProductId = products.find(product => product.id === id)
    const existingProductName = products.find(product => product.name === name);
    if (existingProductId || existingProductName)
        return res.status(400).send("This item already exists");

    const product = {
        name, category, id, quantity
    };
    products.push(product);
    console.log("added", products);
    fs.writeFileSync(filePath, JSON.stringify(products));
    return res.json(product);
}

const deleteProduct = (req, res) => {
    const { id } = req.params;
    const productId = products.findIndex(product => product.id === Number(id));
    if (productId === -1) return res.status(404).send("Product not found");
    const deletedProduct = products.splice(productId, 1);
    fs.writeFileSync(filePath, JSON.stringify(products));
    return res.json(deletedProduct);
}

const updateProduct = (req, res) => {
    const { id } = req.params;
    const { name, category } = req.body;
    const existingProductName = products.find(product => product.name === name);
    if (existingProductName)
        return res.status(400)
            .send("Cannot use this name, another item already has it");
    const product = products.find(product => product.id === Number(id));
    if (!product) res.status(404).send("Product not found");
    if (name) product.name = name;
    if (category) product.category = category;
    fs.writeFileSync(filePath, JSON.stringify(products));
    return res.json(product);
}

const restockProduct = (req, res) => {
    const { id } = req.params;
    const product = products.find(product => product.id === Number(id));
    if (!product) res.status(404).send("Product not found");
    if (product.quantity >= 100)
        res.status(400).send("Product reached maximum stock");
    product.quantity = product.quantity + 1;
    fs.writeFileSync(filePath, JSON.stringify(products));
    return res.json(product);
}

const destockProduct = (req, res) => {
    const { id } = req.params;
    const product = products.find(product => product.id === Number(id));
    if (!product) res.status(404).send("Product not found");
    if (product.quantity <= 0)
        res.status(400).send("Product is out of stock");
    product.quantity = product.quantity - 1;
    fs.writeFileSync(filePath, JSON.stringify(products));
    return res.json(product);
}

module.exports = {
    getProducts, getProduct, addProduct, deleteProduct, updateProduct,
    restockProduct, destockProduct, testConnection, getHomepage, getBalloonPage
}