<script setup>
import { onMounted, onUnmounted } from 'vue';

onMounted(() => {
  console.log("AboutView mounted");
})

onUnmounted(() => {
  console.log("AboutView UNMOUNTED");
})
</script>

<template>
  <h1 class="text-2xl font-bold">About Us</h1>
  <p>Created by John Fayez for ITI Vue.js course.</p>
  <p>Welcome to our store! We are a team of passionate individuals dedicated to providing the best shopping experience.</p>
</template>