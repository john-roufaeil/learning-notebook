import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '@/views/HomeView.vue'
import ProductView from '@/views/ProductView.vue'
import AboutView from '@/views/AboutView.vue'
import CartView from '@/views/CartView.vue'
import NotFoundView from '@/views/NotFoundView.vue'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: HomeView,
    },
    {
      path: '/about',
      name: 'about',
      component: AboutView
    },
    {
      path: '/products/:id(\\d+)',
      name: 'product',
      component: ProductView,
      props: (route) => ({ id: Number(route.params.id) })
    },
    {
      path: '/cart',
      name: 'cart',
      component: CartView
    },
    {
      path: '/not-found',
      name: 'not-found',
      component: NotFoundView
    },
    {
      path: "/:catchAll(.*)",
      redirect: '/not-found'
    }
  ],
})

export default router
