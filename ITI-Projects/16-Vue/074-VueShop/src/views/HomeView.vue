<script setup>
  import { ref, computed } from 'vue';
  import ProductDetails from '@/components/ProductDetails.vue';
  import ProductCard from '@/components/ProductCard.vue';
  
  const product = ref({
    "id": 1,
    "name": "Cozy Sneakers",
    "description": "High-quality sneakers that go with everything you wear.",
    "image": "https://images.unsplash.com/photo-1460353581641-37baddab0fa2",
    "badge": "NEW",
    "price": 120,
    "discount": 20,
    "tags": ["Fashion", "Casual", "Sport"],
    "isAvailable": false,
  });
  
  const relatedProducts = ref([
    {
      "id": 2,
      "name": "Running Shoes",
      "price": 90,
      "discount": 10,
      "badge": "NEW",
      "image": "https://images.unsplash.com/photo-1542291026-7eec264c27ff?w=300"
    },
    {
      "id": 3,
      "name": "Casual Boots",
      "price": 150,
      "discount": 0,
      "badge": "NEW",
      "image": "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQkaRqdR2NoHTS2Dkgpt4R_DvBS6BEcuIjPkw&s"
    },
    {
      "id": 4,
      "name": "Flip Flops",
      "price": 30,
      "discount": 50,
      "image": "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSNrNFthNTQ6msWzMqc8KpxbbsShciFhc_3Pw&s"
    }
  ]);

  const discountedPrice = computed(() => {
    return product.value.discount ?
      product.value.price * (1 - product.value.discount / 100) :
      product.value.price;
  });

  const toggleAvailability = () => {
    product.value.isAvailable = !product.value.isAvailable;
  }
</script>

<template>
  <ProductDetails 
    :product="product" 
    :discounted-price="discountedPrice" 
    @toggleAvailability="toggleAvailability" 
  />

  <h3 class="text-xl font-semibold mt-10 mb-2">Related Products</h3>
  <div class="flex gap-x-6 my-4">
    <ProductCard 
      v-for="item in relatedProducts" 
      :product="item"
    />
  </div>
</template>
