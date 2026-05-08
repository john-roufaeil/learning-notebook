const Product = require('../models/product');
const User = require('../models/user');
const mongoose = require('mongoose');
const StatusError = require('../helpers/StatusError');

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
    return user;
}

const editUser = async (id, data) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, 'Invalid User ID');
    const existingUser = await User.findById(id);
    if (!existingUser)
        throw new StatusError(404, 'This user does not exist');
    if (data.firstName) {
        existingUser.firstName = data.firstName;
    }
    if (data.lastName) {
        existingUser.lastName = data.lastName;
    }
    if (data.dob) {
        existingUser.dob = data.dob;
    }
    if (data.password) {
        existingUser.password = data.password;
    }
    await existingUser.save();
    return existingUser;
}

const deleteUser = async (id) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, 'Invalid User ID');
    const existingUser = await User.findById(id);
    if (!existingUser)
        throw new StatusError(404, 'This user does not exist');
    await User.deleteOne({ '_id': id });
    return existingUser;
}

module.exports = {
    getUsers, getProductsOfUser, createUser, editUser, deleteUser
}