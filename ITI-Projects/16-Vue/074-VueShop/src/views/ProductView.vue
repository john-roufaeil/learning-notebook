<script setup>
import { onMounted, onUnmounted, computed } from 'vue';
import { useProductsStore } from '@/stores/productStore';
import ProductDetails from '@/components/ProductDetails.vue';
import ProductCard from '@/components/ProductCard.vue';

const props = defineProps({
  id: {
    type: Number,
    required: true
  }
})
const productStore = useProductsStore();

onMounted(async () => {
  await productStore.fetchProducts();
  console.log(`ProductView mounted for ID: ${props.id}`);
})
onUnmounted(() => {console.log("ProductView UNMOUNTED");})

const product = computed(() => productStore.getProductById(props.id));

const relatedProducts = computed(() =>
  productStore.products.filter((p) => p.id != props.id)
);
</script>

<template>
  <div v-if="productStore.loading && !productStore.products.length" class="flex justify-center py-16">
    <span class="loading loading-spinner loading-lg"></span>
  </div>
  <div
    v-else-if="!product"
    class="flex flex-col items-center gap-4 py-16"
  >
    <p class="text-xl">Product not found.</p>
    <RouterLink to="/" class="btn btn-primary">Back to Home</RouterLink>
  </div>
  <div
    v-else
    class="flex flex-col items-start gap-4 py-16"
  >
    <ProductDetails
      :product="product"
    />

    <h2 class="text-2xl font-semibold mt-4">Related Projects</h2>
    <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6 my-4">
      <ProductCard
        v-for="item in relatedProducts"
        :key="item.id"
        :product="item"
      />
    </div>
  </div>
</template>