<script setup lang="ts">
import { ref } from 'vue';
import Header from './components/Header.vue'
import Footer from './components/Footer.vue'

const products = ref([
  {
    "id": 1,
    "name": "Cozy Sneakers",
    "description": "High-quality sneakers that go with everything you wear.",
    "image": "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQfDXbNhhcqMGgJXPHK0aiCScudwYwdnqNsgA&s",
    "badge": "SALE",
    "price": 120,
    "discount": 20,
    "stock": 10,
    "tags": ["Fashion", "Casual", "Sport"]
  },
  {
    "id": 2,
    "name": "Running Shoes",
    "description": "Built for speed and comfort on any terrain.",
    "image": "https://images.unsplash.com/photo-1542291026-7eec264c27ff?w=300",
    "badge": "",
    "price": 90,
    "discount": 10,
    "stock": 5,
    "tags": ["Sport", "Running"]
  },
  {
    "id": 3,
    "name": "Casual Boots",
    "description": "Rugged boots for everyday adventures.",
    "image": "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQkaRqdR2NoHTS2Dkgpt4R_DvBS6BEcuIjPkw&s",
    "badge": "NEW",
    "price": 150,
    "discount": 0,
    "stock": 8,
    "tags": ["Casual", "Winter"]
  },
  {
    "id": 4,
    "name": "Flip Flops",
    "description": "Light and breezy for sunny days.",
    "image": "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSNrNFthNTQ6msWzMqc8KpxbbsShciFhc_3Pw&s",
    "badge": "",
    "price": 30,
    "discount": 50,
    "stock": 20,
    "tags": ["Summer", "Casual"]
  }
]);

const decreaseStock = (productId: Number) => {
  const product = products.value.find(p => p.id === productId);
  if (product && product.stock > 0) {
    product.stock--;
  }
}
</script>

<template>
  <div class="min-h-screen flex flex-col max-w-5xl mx-auto">
    <Header />
    <main class="flex-1 p-6">
      <RouterView :products="products" @addToCart="decreaseStock" />
    </main>
    <Footer />
  </div>
</template> 