const express = require('express');
const { productsController } = require('../controllers');
const authMiddleware = require('../middlewares/auth');

const router = express.Router();

router.get('/', authMiddleware, async (req, res) => {
    const { limit, skip, status } = req.query;
    try {
        const products = await productsController
            .getProducts(Number(limit), Number(skip), status);
        res.json(products);
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
});

router.post('/', authMiddleware, async (req, res) => {
    const body = req.body;
    const userId = req.body.userId;
    try {
        const product = await productsController.createProduct(body, userId)
        res.json(product);
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
});

router.patch('/:id', authMiddleware, async (req, res) => {
    const { id } = req.params;
    const body = req.body;
    const userId = req.body.userId;
    try {
        const product = await productsController.editProduct(id, body, userId);
        res.json(product);
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
});

router.patch('/:id/stock', authMiddleware, async (req, res) => {
    const { id } = req.params;
    const { operation, quantity, userId } = req.body;
    try {
        const product = await productsController.editProductStock(
            id, operation, Number(quantity), userId
        );
        res.json(product);
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
});

router.delete('/:id', authMiddleware, async (req, res) => {
    const { id } = req.params;
    if (!req.body || !req.body.userId)
        res.status(400).send("userId is required");
    const userId = req.body.userId;
    try {
        const product = await productsController.deleteProduct(id, userId);
        res.json(product);
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
})

module.exports = router;