const Product = require('../models/product');
const mongoose = require('mongoose');
const StatusError = require('../helpers/StatusError');

const getProducts = async (limit, skip, status) => {
    let products;
    if (status)
        products = await Product.find({ 'status': status });
    else
        products = await Product.find();
    if (limit)
        products = await products.limit(limit);
    if (skip)
        products = await products.skip(skip);
    return await products;
}

const createProduct = async (data, userId) => {
    const existingName = await Product.findOne({ 'name': data.name });
    if (existingName)
        throw new StatusError(409, "A product with this name already exists");
    const newProduct = {
        "name": data.name,
        "quantity": data.quantity,
        "categories": data.categories,
        "owner": userId
    }
    const product = await Product.create(newProduct);
    return product;
};

const editProduct = async (id, body, userId) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, 'Invalid Product ID');
    const product = await Product.findById(id);
    if (!product)
        throw new StatusError(404, "This product does not exist");
    if (product.owner.toString() !== userId) {
        throw new StatusError(401, "Unauthorized to edit this product");
    }
    if (body.name) {
        const existingName = await Product.findOne({ 'name': body.name });
        if (existingName)
            throw new StatusError(409, "A product with this name exists");
        product.name = body.name;
    }
    product.save();

    return product;
}

const editProductStock = async (id, operation, quantity, userId) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, "Invalid Product ID");
    const existingProduct = await Product.findById(id);
    if (!existingProduct)
        throw new StatusError(404, "This product does not exist");
    if (existingProduct.owner.toString() !== userId)
        throw new StatusError(401, "Unauthorized to edit this product");
    if (operation === "restock" && existingProduct.quantity + quantity > 100)
        throw new StatusError(400, "Stock cannot exceed 100");
    if (operation === "destock" && existingProduct.quantity - quantity < 0)
        throw new StatusError(400, "Not enough stock");
    if (operation === "restock") {
        existingProduct.quantity += quantity;
    } else if (operation === "destock") {
        existingProduct.quantity -= quantity;
    } else {
        throw new StatusError(400, "Invalid operation");
    }
    await existingProduct.save();
    return existingProduct;
}

const deleteProduct = async (id, userId) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, 'Invalid Product ID');
    const existingProduct = await Product.findById(id);
    if (!existingProduct)
        throw new StatusError(404, 'This product does not exist');
    if (existingProduct.owner.toString() !== userId)
        throw new StatusError(401, "Unauthorized to delete this product");
    await Product.deleteOne({ '_id': id });
    return existingProduct;
}

module.exports = {
    getProducts, createProduct, editProduct, editProductStock, deleteProduct
};