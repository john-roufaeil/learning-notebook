const mongoose = require('mongoose');

const productSchema = new mongoose.Schema({
    owner: {
        type: mongoose.Types.ObjectId,
        ref: 'User'
    },
    name: {
        type: String,
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
    toJSON: { virtuals: true }
})

productSchema.virtual('status').get(function () {
    if (this.quantity > 2) return 'available';
    if (this.quantity > 0) return 'low-stock';
    return 'out-of-stock';
});

productSchema.index({ owner: 1, name: 1 }, { unique: true });

const Product = mongoose.model('Product', productSchema);

module.exports = Product;