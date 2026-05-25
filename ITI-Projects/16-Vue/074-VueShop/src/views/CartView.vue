<script setup>
import { formatDiscountedPrice } from '@/composables/useProductPrice';
import { useCartStore } from '@/stores/cartStore';
import { Trash2 } from '@lucide/vue';
const cartStore = useCartStore();
</script>

<template>
  <div class="py-8">
    <h1 class="text-3xl font-bold mb-6">Your Cart</h1>

    <div v-if="cartStore.items.length === 0" class="text-center py-16">
      <p class="text-xl text-gray-500">Your cart is empty.</p>
      <RouterLink to="/" class="btn btn-primary mt-4">Shop Now</RouterLink>
    </div>

    <div v-else class="flex flex-col gap-4">
      <div
        v-for="item in cartStore.items"
        :key="item.id"
        class="flex items-center gap-4 bg-base-200 rounded-lg p-4 shadow hover:bg-base-200/60 transition"
      >
        <RouterLink :to="`/products/${item.id}`" class="flex items-center gap-4 flex-1 hover:opacity-80 transition">
        <img :src="item.image" :alt="item.title" class="w-20 h-20 object-cover rounded-lg" />
        <div class="flex-1">
          <p class="font-semibold">{{ item.title }}</p>
          <p class="text-sm text-gray-500">{{ formatDiscountedPrice(item.price, item.discount) }} each</p>
          <p class="text-sm">x{{ item.quantity }}</p>
        </div>
      </RouterLink>
        <div class="text-right">
          <p class="font-bold text-primary">{{ formatDiscountedPrice(item.price * item.quantity, item.discount) }}</p>
          <div class="flex gap-4 items-center">
            <button class="btn btn-error btn-outline btn-sm" @click.stop="cartStore.trashItem(item.id)">
              <Trash2 :size="16" />
            </button>
            <button v-if="item.quantity > 1" @click.stop="cartStore.removeFromCart(item.id)">
              <span class="btn btn-hover">-</span>
            </button>
            <p>{{ item.quantity }}</p>
            <button class="btn btn-hover" :disabled="item.stock === 0" @click.stop="cartStore.addToCart(item)" >
              <span>+</span>
            </button>
            
          </div>
        </div>
    </div>
    <div class="flex justify-between items-center mt-4 p-4 bg-base-200 rounded-lg">
      <span class="text-xl font-bold">Total: {{ formatDiscountedPrice(cartStore.totalPrice) }}</span>
      <button class="btn btn-error btn-outline" @click="cartStore.clearCart()">Clear Cart</button>
    </div>
    </div>
  </div>
</template>