const jwt = require('jsonwebtoken');
const StatusError = require('../helpers/StatusError');
require('dotenv').config();

const JWT_SECRET = process.env.JWT_SECRET;

async function authMiddleware(req, res, next) {
    const token = req.headers.authorization.split(" ")[1];
    if (!token)
        throw new StatusError(401, "You are unauthorized");

    const { userId } = jwt.verify(token, JWT_SECRET);
    req.userId = userId;
    next();
}

module.exports = authMiddleware;