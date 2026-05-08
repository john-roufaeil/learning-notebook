const mongoose = require('mongoose');
const bcrypt = require('bcrypt');
const jwt = require('jsonwebtoken');
require('dotenv').config();

const JWT_SECRET = process.env.JWT_SECRET
const { Schema } = mongoose;

const userSchema = new Schema({
    username: {
        type: String,
        required: true,
        unique: true,
        minLength: 4,
        lowercase: true,
        trim: true
    },
    password: {
        type: String,
        required: true
    },
    firstName: {
        type: String,
        required: true,
        minLength: 3,
        maxLength: 15
    },
    lastName: {
        type: String,
        required: true,
        minLength: 3,
        maxLength: 15
    },
    dob: {
        type: Date,
        min: new Date('1920-01-01'),
        max: new Date('2026-01-01')
    },
    role: {
        type: String,
        enum: ['user', 'admin'],
        default: 'user',
        required: true
    }
}, { timestamps: true })

userSchema.pre('save', function () {
    this.password = bcrypt.hashSync(this.password, 10);
    return this;
})

userSchema.set('toJSON', {
    transform: (_, ret) => {
        const { password, ...rest } = ret;
        return rest;
    }
});

userSchema.methods.verifyPassword = function (password) {
    return bcrypt.compareSync(password, this.password);
}

userSchema.methods.generateToken = function () {
    return jwt.sign(
        { userId: this._id }
        , JWT_SECRET, { expiresIn: '1d' }
    )
}

const User = mongoose.model('User', userSchema);

module.exports = User;