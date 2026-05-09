const Product = require('../models/product');
const mongoose = require('mongoose');
const StatusError = require('../helpers/StatusError');

const getProducts = async (limit, skip, status) => {
    let query = Product.find();
    if (limit) {
        if (limit > 1000) limit = 1000;
        query = query.limit(limit);
    }
    if (skip)
        query = query.skip(skip);
    let products = await query;
    if (status)
        products = products.filter(p => p.status === status);
    
    console.log(products);
    return products;
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

const editProduct = async (id, body) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, 'Invalid Product ID');
    const product = await Product.findById(id);
    if (!product)
        throw new StatusError(404, "This product does not exist");
    // if (product.owner.toString() !== userId) {
    //     throw new StatusError(401, "Unauthorized to edit this product");
    // }
    if (body.name) {
        const existingName = await Product.findOne({ 'name': body.name });
        if (existingName)
            throw new StatusError(409, "A product with this name exists");
        product.name = body.name;
    }
    product.save();

    return product;
}

const editProductStock = async (id, operation, quantity) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, "Invalid Product ID");
    const existingProduct = await Product.findById(id);
    if (!existingProduct)
        throw new StatusError(404, "This product does not exist");
    // if (existingProduct.owner.toString() !== userId)
    //     throw new StatusError(401, "Unauthorized to edit this product");
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

const deleteProduct = async (id) => {
    if (!mongoose.Types.ObjectId.isValid(id))
        throw new StatusError(400, 'Invalid Product ID');
    const existingProduct = await Product.findById(id);
    if (!existingProduct)
        throw new StatusError(404, 'This product does not exist');
    // if (existingProduct.owner.toString() !== userId)
    //     throw new StatusError(401, "Unauthorized to delete this product");
    await Product.deleteOne({ '_id': id });
    return existingProduct;
}

module.exports = {
    getProducts, createProduct, editProduct, editProductStock, deleteProduct
};