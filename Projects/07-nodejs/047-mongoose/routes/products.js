const express = require('express');
const { productsController } = require('../controllers');

const router = express.Router();

//  /products?limit=10 & skip=0 & status=$value
router.get('/', async (req, res) => {
    // Return the products with specific required filters
    // (status, defaults are limit 10 skip 0) (don’t reinvent the wheel) 
    const { limit, skip, status } = req.query;
    const products = await productsController.getProducts(limit, skip, status);
    res.json(products);
});

router.post('/', async (req, res) => {
    // Create new product (name, category, quantity)
    // You must save it with userId of the logged in user
    // Return the new product to the user
    const { body } = req.body;
    const product = await productsController.createProduct(body);
    res.json(product);
});

router.patch('/:id', async (req, res) => {
    // Edit Product
    const { id } = req.params;
    const { body } = req.body;
    const product = await productsController.editProduct(id, body);
    res.json(product);
});

router.patch('/:id/stock', async (req, res) => {
    // Restock or destock product using {operation: “restock”
    // or “destock”, quantity: number}
    const { id } = req.params;
    const { body } = req.body;
    const product = await productsController.editProductStock(id, body);
    res.json(product);
});

router.delete('/:id', async (req, res) => {
    // Delete product
    const { id } = req.params;
    const product = await productsController.deleteProduct(id);
    res.json(product);
})

module.exports = router;