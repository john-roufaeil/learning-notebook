const express = require('express');
const mongoose = require('mongoose');
const routes = require('./routes');
require('dotenv').config();

mongoose.connect('mongodb://127.0.0.1:27017/inventory');

const app = express();

app.use(express.json());
app.use(routes);
app.use((req, res) => { res.status(404).send("Endpoint is not configured") });
app.use((err, req, res, next) => {
    if (err) res.status(err.code || 400).send(err.message);
})
app.listen(3000, (err) => {
    if (err) console.error(err);
    console.log("Up and running on localhost:3000");
});