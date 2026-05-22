<script setup>
import { computed } from 'vue';

const props = defineProps({
  product: Object
})

const discountedPrice = computed(() => {
  return props.product.discount ?
    props.product.price * (1 - props.product.discount / 100) :
    props.product.price;
});
</script>

<template>
  <div class="bg-base-200 w-75 shadow-md">
    <figure class="rounded-t-lg overflow-hidden h-50">
      <img :src="product.image" :alt="product.name" class="w-full" />
    </figure>
    <div class="p-4">
      <p>{{ product.name }}</p>
      <div class="flex items-baseline gap-2">
        <span class="text-primary">${{ discountedPrice.toFixed(2) }}</span>
        <span v-if="product.discount > 0" class="text-sm line-through text-gray-400">
          ${{ product.price }}
        </span>
        <span v-if="product.badge" class="badge badge-accent">{{ product.badge }}</span>
      </div>
    </div>
  </div>
</template>