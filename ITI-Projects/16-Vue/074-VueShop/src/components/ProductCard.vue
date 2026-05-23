<script setup>
import { computed } from 'vue';
import { RouterLink } from 'vue-router';
import { onMounted, onUnmounted } from 'vue';

onMounted(() => {
  console.log(`ProductCard mounted - ${props.product.name}`);
})

onUnmounted(() => {
  console.log(`ProductCard UNMOUNTED - ${props.product.name}`);
})

const props = defineProps({
  product: Object
})

const discountedPrice = computed(() => {
  return props.product.discount ?
    props.product.price * (1 - props.product.discount / 100) :
    props.product.price;
});

const CONVERSION_RATE = 35;

const formattedDiscountedPrice = computed(() =>
  new Intl.NumberFormat("en-US", {
    style: "currency",
    currency: "EGP",
  }).format(discountedPrice.value * CONVERSION_RATE),
);

const formattedPrice = computed(() =>
  new Intl.NumberFormat("en-US", {
    style: "currency",
    currency: "EGP",
  }).format(props.product.price * CONVERSION_RATE),
);
</script>

<template>
  <RouterLink :to="`/products/${product.id}`" class="h-full flex flex-1">
    <div class="bg-base-200 w-full h-full shadow-md">
      <figure class="rounded-t-lg overflow-hidden h-50 w-full shrink-0">
        <img loading="lazy" :src="product.image" :alt="product.name" class="w-full h-full object-cover" />
      </figure>
      <div class="p-4 flex flex-col flex-1">
        <p>{{ product.name }}</p>
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