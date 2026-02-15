const jwt = require('jsonwebtoken');
const StatusError = require('../helpers/StatusError');
const User = require('../models/user');
require('dotenv').config();

const JWT_SECRET = process.env.JWT_SECRET;

async function authMiddleware(req, res, next) {
    const token = req.headers.authorization;
    if (!token)
        throw new StatusError(401, "Unauthorized access");

    const payload = jwt.verify(token, JWT_SECRET);
    const user = await User.findById(payload.userId);
    req.user = user;
    next();
}

module.exports = authMiddleware;