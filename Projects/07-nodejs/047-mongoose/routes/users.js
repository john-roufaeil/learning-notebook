const express = require('express');
const { usersController } = require('../controllers');

const router = express.Router();

router.get('/', async (req, res) => {
    // Return the first name of registered users
    const users = await usersController.getUsers();
    res.json(users);
});

router.get('/:userId/products', async (req, res) => {
    // Return the products of specific user
    const { userId } = req.params;
    const userProducts = await usersController.getProductsOfUser(userId);
    res.json(userProducts);
})

router.post('/', async (req, res) => {
    // - Register a user with the following required attributes
    // Username,password , firstName, lastName
    // Note: Handle validation errors returned from mongo
    const { body } = req.body;
    const user = await usersController.createUser(body);
    res.json(user);
});

router.patch('/:id', async (req, res) => {
    // Edit the user with the selected id
    // Return ({message:”user was edited successfully”, user: theUserAfterEdit”}) if success
    // Handle validation errors returned from mongo
    const { id } = req.params;
    const { body } = req.body;
    const user = await usersController.editUser(id, body);
    res.json(user);
});

router.delete('/:id', async (req, res) => {
    // Delete the user with selected id
    const { id } = req.params;
    const user = await usersController.deleteUser(id);
    res.json(user);
});

module.exports = router;