const Product = require('../models/product');

const getProducts = async (limit, skip, status) => {
    if (limit && skip && status)
        return await Product.find({ 'status': status }).limit(limit).skip(skip);
    if (limit && skip)
        return await Product.find().limit(limit).skip(skip);
}

const createProduct = async (data, userId) => {
    const existingName = await Product.findOne({ 'name': data.name });
    if (existingName)
        throw new Error("A product with this name already exists");
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
    const product = await Product.findById(id);
    if (!product)
        throw new Error("This product does not exist");
    if (product.owner.toString() !== userId) {
        throw new Error("You are not authorized to edit this product");
    }
    if (body.name) {
        const existingName = Product.findOne({ 'name': body.name });
        if (existingName)
            throw new Error("A product with this name already exists");
        product.name = body.name;
    }
    product.save();

    return product;
}

const editProductStock = async (id, operation, quantity, userId) => {
    const existingProduct = await Product.findById(id);
    if (!existingProduct)
        throw new Error('This product does not exist');
    if (existingProduct.owner.toString() !== userId)
        throw new Error("You are not authorized to edit this product");
    if (operation === "restock" && existingProduct.quantity + quantity > 100)
        throw new Error("Stock cannot exceed 100");
    if (operation === "destock" && existingProduct.quantity - quantity < 0)
        throw new Error("Stock cannot be negative");
    if (operation === "restock") {
        existingProduct.quantity += quantity;
    } else if (operation === "destock") {
        existingProduct.quantity -= quantity;
    } else {
        throw new Error("Invalid operation");
    }
    await existingProduct.save();
    return existingProduct;
}

const deleteProduct = async (id, userId) => {
    const existingProduct = await Product.findById(id);
    if (!existingProduct)
        throw new Error('This product does not exist');
    if (existingProduct.owner.toString() !== userId)
        throw new Error("You are not authorized to delete this product");
    await Product.deleteOne({ '_id': id });
    return existingProduct;
}

module.exports = {
    getProducts, createProduct, editProduct, editProductStock, deleteProduct
};