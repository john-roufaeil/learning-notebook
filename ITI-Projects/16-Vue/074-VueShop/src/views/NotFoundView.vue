<script setup>
import { RouterLink } from 'vue-router';
</script>

<template>
  <div class="flex flex-col items-center gap-4 py-16">
    <h1 class="text-3xl font-bold text-center mt-20">404 - Page Not Found</h1>
    <p class="text-center mt-4 text-gray-600">Sorry, the page you are looking for does not exist.</p>
    <RouterLink to="/" class="btn btn-primary">Back to Home</RouterLink>
  </div>
</template>