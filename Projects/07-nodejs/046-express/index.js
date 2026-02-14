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

app.get('/test', (req, res) => testConnection(req, res));

app.get('/', (req, res, next) => {
    getHomepage(req, res, next);
})

app.get('/balloon', (req, res) => getBalloonPage(req, res));

app.get('/products', (req, res, next) => getProducts(req, res, next));

app.get('/products/:id', (req, res, next) => getProduct(req, res, next));

app.post('/products', (req, res, next) => addProduct(req, res, next));

app.delete('/products/:id', (req, res, next) => deleteProduct(req, res, next));

app.patch('/products/:id', (req, res, next) => updateProduct(req, res, next));

app.patch('/products/:id/restock', (req, res, next) =>
    restockProduct(req, res, next)
);

app.patch('/products/:id/destock', (req, res, next) =>
    destockProduct(req, res, next)
);

app.use((err, req, res, next) => {
    console.error(err);
    const status = err.status || 500;
    return res.status(status).json({
        message: err.message || "Internal Server Error"
    })
})

app.use((req, res) => {
    return res.status(404).send("Sorry can't find that!")
})

app.listen(port, () => console.log(`Up and running on port ${port}`))
