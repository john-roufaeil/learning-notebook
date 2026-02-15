const mongoose = require('mongoose');
const bcrypt = require('bcrypt');

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
    }
}, { timestamps: true })

userSchema.pre('save', function () {
    this.password = bcrypt.hashSync(this.password, 10);
    return this;
})

const User = mongoose.model('User', userSchema);

module.exports = User;