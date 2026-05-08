const express = require('express');
const mongoose = require('mongoose');
const routes = require('./routes');

mongoose.connect('mongodb://127.0.0.1:27017/inventory');

const app = express();

app.use(express.json());
app.use(routes);
app.use((req, res) => {
    res.status(404).send("Sorry, this endpoint is not configured")
});
app.listen(3000, (err) => {
    if (err) console.error(err);
    console.log("Up and running on localhost:3000");
});