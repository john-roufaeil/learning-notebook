<script setup>
  import { RouterLink } from 'vue-router';
</script>
<template>
  <header class="navbar bg-base-100 border-b border-base-300 px-6">
    <RouterLink to="/" class="text-xl font-bold flex gap-2 items-center">
      <img src="/logo.png" alt="VueShop Logo" class="h-10 w-10" />
      <p>VueShop</p>
    </RouterLink>
    <nav class="flex gap-4 text-sm absolute inset-e-6">
      <RouterLink to="/" class="link link-hover">Home</RouterLink>
      <RouterLink to="/products/1" class="link link-hover">Product</RouterLink>
      <RouterLink to="/about" class="link link-hover">About</RouterLink>
    </nav>
  </header>
</template>