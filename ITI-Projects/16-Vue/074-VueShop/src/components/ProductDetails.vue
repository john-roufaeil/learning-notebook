<script setup>
import { useProductPrice } from '@/composables/useProductPrice';
import { useCartStore } from '@/stores/cartStore';
const cartStore = useCartStore();
const props = defineProps({product: Object})
const { formattedDiscountedPrice, formattedPrice } = useProductPrice(() => props.product);
</script>

<template>
<div class="flex flex-col md:flex-row bg-base-200 shadow-xl rounded-lg p-6 pe-16 items-center gap-8 w-full">
    <figure class="w-75 rounded-lg overflow-hidden h-fit">
      <img loading="lazy" :src="product.image" :alt="product.title" class="w-full h-full object-cover" />
    </figure>
    <div class="flex flex-col gap-2">
      <div class="flex gap-2">
        <span v-if="product.badge" class="badge badge-outline">{{ product.badge }}</span>
        <span v-if="product.stock === 0" class="badge badge-error">Out of Stock</span>
      </div>
      <h2 class="text-3xl">{{ product.title }}</h2>
      <p class="text-gray-600">{{ product.description }}</p>

      <div class="flex items-end gap-3">
        <span class="text-3xl font-bold text-primary">{{ formattedDiscountedPrice }}</span>
        <span v-if="product.discount > 0" class="line-through text-gray-400">
          {{ formattedPrice }}
        </span>
      </div>

      <div class="flex gap-2 mt-2">
        <span v-for="tag in product.tags" class="badge">
          {{ tag }}
        </span>
      </div>

      <div class="mt-4 flex gap-4 items-center">
        <button v-if="product.stock > 0" class="btn btn-primary"
          @click="cartStore.addToCart(product)">
          Add to Cart
        </button>
        <button v-else class="btn btn-primary" disabled>Out of Stock</button>
        <p class="text-gray-600">Stock: {{ product.stock }}</p>
      </div>
    </div>
  </div>
</template>