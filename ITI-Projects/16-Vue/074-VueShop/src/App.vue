<script setup lang="ts">
import Header from './components/Header.vue'
import Footer from './components/Footer.vue'
</script>

<template>
  <div class="min-h-screen flex flex-col max-w-5xl mx-auto">
    <Header />
    <main class="flex-1 p-6">
      <RouterView />
    </main>
    <Footer />
  </div>
</template> 