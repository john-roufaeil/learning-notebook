<script setup>
defineProps({
  product: Object,
  discountedPrice: Number
})

const getTagColor = (tag) => {
  switch (tag) {
    case 'Fashion':
      return 'badge-primary';
    case 'Casual':
      return 'badge-secondary';
    default:
      return 'badge-accent';
  }
}
</script>

<template>
<div class="flex flex-col md:flex-row bg-base-200 shadow-xl rounded-lg p-6 pe-16 items-center gap-8 w-fit">
    <figure class="w-75 rounded-lg overflow-hidden h-fit">
      <img :src="product.image" :alt="product.name" />
    </figure>
    <div class="flex flex-col gap-2">
      <div class="flex gap-2">
        <span v-if="product.badge" class="badge badge-outline">{{ product.badge }}</span>
        <span v-if="!product.isAvailable" class="badge badge-error">Out of Stock</span>
      </div>
      <h2 class="text-3xl">{{ product.name }}</h2>
      <p class="text-gray-600">{{ product.description }}</p>

      <div class="flex items-end gap-3">
        <span class="text-3xl font-bold text-primary">${{ discountedPrice.toFixed(2) }}</span>
        <span v-if="product.discount > 0" class="line-through text-gray-400">
          ${{ product.price }}
        </span>
      </div>

      <div class="flex gap-2 mt-2">
        <span v-for="tag in product.tags" class="badge" :class="getTagColor(tag)">
          {{ tag }}
        </span>
      </div>

      <div class="mt-4 flex gap-4">
        <button v-if="product.isAvailable" class="btn btn-primary">Add to Cart</button>
        <button v-else class="btn btn-primary" disabled>Out of Stock</button>
        <button @click="$emit('toggleAvailability')" class="btn btn-outline">Toggle Availability</button>
      </div>
    </div>
  </div>
</template>