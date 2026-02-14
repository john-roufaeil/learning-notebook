const mongoose = require('mongoose');

const productSchema = new mongoose.Schema({
    owner: {
        type: mongoose.Types.ObjectId,
        ref: 'User'
    },
    name: {
        type: String,
        unique: true,
        required: true,
        minLength: 5,
        maxLength: 20
    },
    categories: {
        type: [String],
        default: ["General"]
    },
    quantity: {
        type: Number,
        min: 0,
        max: 100
    },
}, {
    timestamps: true,
    toJSON: { virtuals: true, },
    toObject: { virtuals: true }
})

productSchema.virtual('status').get(function () {
    if (this.quantity > 2) return 'Available';
    if (this.quantity > 0) return 'Low Stock';
    return 'Out of Stock';
});

const Product = mongoose.model('Product', productSchema);

module.exports = Product;