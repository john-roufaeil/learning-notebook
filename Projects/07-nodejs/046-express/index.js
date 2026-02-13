const express = require('express');
const {
    getProducts, getProduct, addProduct, deleteProduct,
    updateProduct, restockProduct, destockProduct,
    testConnection, getHomepage, getBalloonPage
} = require('./controller');

const app = express();
const port = 3000;

app.use(express.json())
app.use(express.static('public'))
app.set('view engine', 'pug')

app.get('/test', (req, res) => testConnection(res));

app.get('/', (err, req, rest, next) => {

})

app.get('/products', (err, req, res, next) => {
    console.log(Date.now())
    req.hamada = 'Ahmed'
    next()
}, (req, res, next) => {
    console.log(req.hamada)
    res.status(201).json({ "hi": "hello" })
    next()
}, (req, res) => {
    console.log(Date.now())
    getProducts(res)
});

app.get('/products/:id', (req, res) => getProduct(req, res));

app.post('/products', (req, res) => addProduct(req, res));

app.delete('/products/:id', (req, res) => deleteProduct(req, res));

app.patch('/products/:id', (req, res) => updateProduct(req, res));

app.patch('/products/:id/restock', (req, res) => restockProduct(req, res));

app.patch('/products/:id/destock', (req, res) => destockProduct(req, res));

app.use((req, res) => {
    res.status(404).send("Sorry can't find that!")
})

app.listen(port, () => console.log(`Up and running on port ${port}`))
