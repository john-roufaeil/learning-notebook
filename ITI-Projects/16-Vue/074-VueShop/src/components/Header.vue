<script setup>
import { RouterLink } from 'vue-router';
import { useCartStore } from '@/stores/cartStore';
import { ShoppingCart } from '@lucide/vue';
const cartStore = useCartStore();
</script>
<template>
  <header class="navbar bg-base-100 border-b border-base-300 px-6">
    <RouterLink to="/" class="text-xl btn btn-hover font-bold flex gap-2 items-center py-6">
      <img src="/logo.png" alt="VueShop Logo" class="h-10 w-10" />
      <p>VueShop</p>
    </RouterLink>
    <nav class="flex gap-4 text-sm absolute items-center inset-e-6">
      <RouterLink to="/" class="link link-hover">Home</RouterLink>
      <RouterLink to="/products/101" class="link link-hover">Product</RouterLink>
      <RouterLink to="/about" class="link link-hover">About</RouterLink>
      <RouterLink to="/cart" class="btn btn-hover btn-sm relative">
        <ShoppingCart :size="20" />
        <span
          v-if="cartStore.totalItems > 0"
          class="badge badge-primary badge-xs absolute -top-2 -right-2"
        >
          {{ cartStore.totalItems }}
        </span>
      </RouterLink>
    </nav>
  </header>
</template>
