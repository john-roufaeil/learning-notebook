const Product = require('../models/product');
const User = require('../models/user');
const mongoose = require('mongoose');
const StatusError = require('../helpers/StatusError');
const bcrypt = require('bcrypt');

const login = async (data) => {
    const user = await User.findOne({ username: data.username });
    if (!user)
        throw new StatusError(401, 'Username or password is incorrect');
    if (!data || !data.username || !data.password)
        throw new StatusError(400, "Username and password requried");

    const isValidPassword = user.verifyPassword(data.password);
    if (!isValidPassword)
        throw new StatusError(401, 'Username or password is incorrect');
    const token = user.generateToken();

    return token;
};

const getUsers = async () => {
    const usersFirstName = await User.find().select('firstName');
    return usersFirstName;
}

const getProductsOfUser = async (userId) => {
    if (!mongoose.Types.ObjectId.isValid(userId))
        throw new StatusError(400, 'Invalid User ID');
    const userExists = await User.findById(userId);
    if (!userExists)
        throw new StatusError(404, 'This user does not exist');
    const productsOfUser = await Product.find({ 'owner': userId });
    return productsOfUser;
}

const createUser = async (data) => {
    const existingUser = await User.findOne({ 'username': data.username });
    if (existingUser)
        throw new StatusError(409, 'This username is already taken');
    const user = await User.create({
        "firstName": data.firstName,
        "lastName": data.lastName,
        "username": data.username,
        "password": data.password
    })
    const jwt = user.generateToken();
    return jwt;
}

const editUser = async (id, data) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, 'Invalid User ID');

    const fieldsToUpdate = {};
    if (data.firstName) fieldsToUpdate.firstName = data.firstName;
    if (data.lastName) fieldsToUpdate.lastName = data.lastName;
    if (data.dob) fieldsToUpdate.dob = data.dob;
    if (data.password) {
        fieldsToUpdate.password = bcrypt.hashSync(data.password, 10);
    };
    const updatedUser = await User.findByIdAndUpdate(
        id,
        fieldsToUpdate,
        { new: true }
    );

    if (!updatedUser)
        throw new StatusError(404, 'This user does not exist');
    return updatedUser;
}

const deleteUser = async (id) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, 'Invalid User ID');
    const deletedUser = await User.findByIdAndDelete(id);
    if (!deletedUser)
        throw new StatusError(404, 'This user does not exist');
    return deletedUser;
}

module.exports = {
    getUsers, getProductsOfUser, createUser, editUser, deleteUser, login
}