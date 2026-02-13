const express = require('express');
const router = express();

const testConnection = (res) => {
    res.send('Hello, World!')
}

const getHomepage = (req, res) => {
    router.get('/', (req, res) => {
        res.render(
            'index',
            { title: 'Hey OSAD', message: 'Hello there ITI!' }
        )
    })
}

const getBalloonPage = (req, res) => {

}

function getProducts(res) {

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