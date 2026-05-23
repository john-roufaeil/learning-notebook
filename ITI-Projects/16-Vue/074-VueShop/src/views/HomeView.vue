<script setup>
import { onMounted, onUnmounted } from 'vue';
import ProductCard from '@/components/ProductCard.vue';
import CarouselBanner from '@/components/CarouselBanner.vue';

const props = defineProps({
  products: {
    type: Array,
    required: true,
    default: () => []
  }
})

onMounted(() => {
  console.log(`HomeView mounted - ${props.products.length} products loaded`);
})

onUnmounted(() => {
  console.log("HomeView UNMOUNTED");
})
</script>

<template>
  <CarouselBanner />

  <h2 class="text-xl font-semibold mt-10 mb-2">Our Products</h2>
  <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-6 my-4">
    <ProductCard 
      v-for="item in props.products" 
      :key="item.id"
      :product="item"
    />
  </div>
</template>
