const fs = require('fs');
const path = require('path');
const express = require('express');
const router = express();

const testConnection = (req, res) => {
    res.send('Hello, World!')
}

const getHomepage = (req, res) => {
    res.render(
        'index',
        {
            title: 'Homepage',
            message: "Products",
            products: [{ "name": "hi", "quantity": 3, "category": "cat" }]
        }
    )
}

const getBalloonPage = (req, res) => {
    res.sendFile(path.join(__dirname, "public", "balloon.html"));
}

const getProducts = (req, res, next) => {
    const jsonPath = path.join(__dirname, 'products.json');
    let products;
    try {
        products = fs.readFileSync(jsonPath, 'utf-8');
        return res.send(products);
    } catch (err) {
        err.status = 500;
        err.message = "Database doesn't exist on server";
        return next(err);
    }
}

function getProduct(req, res) {

}

function addProduct(req, res) {

}

function deleteProduct(req, res) {

}

function updateProduct(req, res) {

}

function restockProduct(req, res) {

}

function destockProduct(req, res) {

}

module.exports = {
    getProducts, getProduct, addProduct, deleteProduct, updateProduct,
    restockProduct, destockProduct, testConnection, getHomepage, getBalloonPage
}