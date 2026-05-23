<script setup> 
import { onMounted, onUnmounted, ref, watch } from 'vue';
import ProductDetails from '@/components/ProductDetails.vue';
import ProductCard from '@/components/ProductCard.vue';

const product = ref(null);

const props = defineProps({
  products: {
    type: Array,
    required: true,
    default: () => []
  },
  id: {
    type: Number,
    required: true
  }
})
const emit = defineEmits(['addToCart']);

onMounted(() => {
  console.log(`ProductView mounted for ID: ${props.id}`);
})

onUnmounted(() => {
  console.log("ProductView UNMOUNTED");
})

watch(() => props.id, (newId) => {
  product.value = props.products.find(p => p.id === props.id)
}, { immediate: true})
</script>

<template>
  <div
    v-if="!product"
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
      @addToCart="$emit('addToCart', $event)"
    />

    <h2 class="text-2xl font-semibold mt-4">Related Projects</h2>
    <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6 my-4">
      <ProductCard
        v-for="item in products.filter(p => p.id !== product.id)"
        :key="item.id"
        :product="item"
      />
    </div>
  </div>
</template>
