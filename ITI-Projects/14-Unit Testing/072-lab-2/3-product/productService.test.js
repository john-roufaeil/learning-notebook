const { MongoMemoryServer } = require('mongodb-memory-server');
const mongoose = require('mongoose');
const Product = require('./models/Product');
const { createProduct, getAvailableProducts } = require('./services/productService');
let mongoServer;

beforeAll(async () => {
  mongoServer = await MongoMemoryServer.create();
  await mongoose.connect(mongoServer.getUri());
});

afterEach(async () => {
  await Product.deleteMany({});
});

afterAll(async () => {
  await mongoose.disconnect();
  await mongoServer.stop();
});

describe('createProduct', () => {
  test('product is created and returned with the correct name, slug, and inStock: true default', async () => {
    const product = await createProduct({
      name: 'New Laptop',
      slug: 'new-laptop',
      price: 20000,
    });

    expect(product.name).toBe('New Laptop');
    expect(product.slug).toBe('new-laptop');
    expect(product.price).toBe(20000);
    expect(product.inStock).toBe(true);
  });

  test('duplicate slug: throws "Slug already in use"', async () => {
    await Product.create({ name: 'New Laptop', slug: 'new-laptop', price: 20000 });
    await expect(createProduct({ name: 'Another Laptop', slug: 'new-laptop', price: 29999 }))
      .rejects.toThrow('Slug already in use');
  });

  test('a negative price is rejected by Mongoose (min: 0)', async () => {
    await expect(createProduct({ name: 'New Laptop', slug: 'new-laptop', price: -20000 }))
      .rejects.toThrow('min');
  });
});

describe('getAvailableProducts', () => {
  test('returns only products where inStock is true', async () => {
    await Product.create([
      { name: 'Product 1', slug: 'product-1', price: 120, inStock: true },
      { name: 'Product 2', slug: 'product-2', price: 435, inStock: true },
      { name: 'Product 3', slug: 'product-3', price: 3090, inStock: false },
    ]);

    const results = await getAvailableProducts();
    expect(results).toHaveLength(2);
  });
});