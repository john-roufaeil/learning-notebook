const express = require('express');
const { usersController } = require('../controllers');
const authMiddleware = require('../middlewares/auth');

const router = express.Router();

router.post('/login', async (req, res) => {
    try {
        const token = await usersController.login(req.body);
        res.json({ token });
    }
    catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
})

router.get('/', async (req, res) => {
    try {
        const users = await usersController.getUsers();
        res.json(users);
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
});

router.get('/:userId/products', authMiddleware, async (req, res) => {
    const { userId } = req.params;
    if (req.userId !== userId)
        return res.status(401).send("Unauthorized");
    try {
        const userProducts = await usersController.getProductsOfUser(userId);
        res.json(userProducts);
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
})

router.post('/', async (req, res) => {
    const body = req.body;
    try {
        const token = await usersController.createUser(body);
        res.json({
            message: "user added successfully",
            token
        });
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
});

router.patch('/:id', authMiddleware, async (req, res) => {
    const { id } = req.params;
    const body = req.body;
    if (req.userId !== id)
        return res.status(401).send("Unauthorized");
    try {
        const updatedUser = await usersController.editUser(id, body);

        res.json({
            message: "user was edited successfully",
            user: updatedUser
        });
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
});

router.delete('/:id', authMiddleware, async (req, res) => {
    const { id } = req.params;
    if (req.userId !== id)
        return res.status(401).send("Unauthorized");
    try {
        const user = await usersController.deleteUser(id);
        res.json({
            message: "user deleted successfully",
            user
        });
    } catch (err) {
        console.error(err);
        res.status(err.code || 400).send(err.message);
    }
});

module.exports = router;