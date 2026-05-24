<script setup>
import { RouterLink } from 'vue-router';
import { onMounted, onUnmounted } from 'vue';
import { useProductPrice } from '@/composables/useProductPrice';

onMounted(() => {
  console.log(`ProductCard mounted - ${props.product.title}`);
})

onUnmounted(() => {
  console.log(`ProductCard UNMOUNTED - ${props.product.title}`);
})

const props = defineProps({
  product: Object
})

const { formattedDiscountedPrice, formattedPrice } = useProductPrice(() => props.product);
</script>

<template>
  <RouterLink :to="`/products/${product.id}`" class="h-full flex flex-1 ">
    <div class="bg-base-200 w-full h-full shadow-md hover:bg-base-300/60 duration-300 transition-all hover:-translate-y-1">
      <figure class="rounded-t-lg overflow-hidden h-50 w-full shrink-0">
        <img loading="lazy" :src="product.image" :alt="product.title" class="w-full h-full object-cover" />
      </figure>
      <div class="p-4 flex flex-col flex-1">
        <p>{{ product.title }}</p>
        <div class="flex items-baseline gap-2">
          <span class="text-primary">{{ formattedDiscountedPrice }}</span>
          <span v-if="product.discount > 0" class="text-sm line-through text-gray-400">
            {{ formattedPrice }}
          </span>
        </div>
        <span v-if="product.badge" class="badge badge-accent">{{ product.badge }}</span>
      </div>
    </div>
  </RouterLink>
</template>