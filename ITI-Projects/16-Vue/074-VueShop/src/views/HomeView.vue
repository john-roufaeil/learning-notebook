<script setup>
import { onMounted, onUnmounted } from 'vue';
import { useProductsStore } from '@/stores/productStore';
import ProductCard from '@/components/ProductCard.vue';
import CarouselBanner from '@/components/CarouselBanner.vue';

const productStore = useProductsStore();
onMounted(async () => {
  await productStore.fetchProducts();
  console.log(`HomeView mounted - ${productStore.products.length} products loaded`);
})
onUnmounted(() => {
  console.log("HomeView UNMOUNTED");
})
</script>

<template>
  <CarouselBanner />

  <h2 class="text-xl font-semibold mt-10 mb-2">Our Products</h2>
  <div v-if="productStore.loading && !productStore.products.length" class="flex justify-center py-16">
    <span class="loading loading-spinner loading-lg"></span>
  </div>
  <div v-else-if="productStore.error" class="text-error text-center py-16">
    {{ productStore.error }}
  </div>
  <div v-else class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-6 my-4">
    <ProductCard 
      v-for="item in productStore.products" 
      :key="item.id"
      :product="item"
    />
  </div>
</template>
