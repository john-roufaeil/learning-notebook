import { defineStore } from "pinia";
import { computed } from "vue";
import { useLocalStorage } from "@/composables/useLocalStorage";
import { useProductsStore } from "./productStore";

export const useCartStore = defineStore("cart", () => {
  const items = useLocalStorage("cart", []);
  const productsStore = useProductsStore();

  const totalItems = computed(() =>
    items.value.reduce((sum: number, item: { quantity: number }) => sum + item.quantity, 0)
  );

  const totalPrice = computed(() =>
    items.value.reduce((sum: number, item: { price: number, quantity: number, discount?: number }) => {
      const discounted = item.discount ? item.price * (1 - item.discount / 100) : item.price;
      return sum + discounted * item.quantity;
    }, 0)
  );

  const addToCart = (product: { id: number, title: string, price: number, image: string, stock: number, discount?: number }) => {
    const existing = items.value.find((item: { id: number }) => item.id === product.id);
    if (existing) {
      existing.quantity++;
      existing.stock--;
    } else {
      items.value.push({
        id: product.id,
        title: product.title,
        price: product.price,
        image: product.image,
        discount: product.discount,
        stock: product.stock - 1,
        quantity: 1,
      });
    }
    productsStore.decreaseStock(product.id);
  };

  const removeFromCart = (id: number) => {
    const item = items.value.find((item: { id: number }) => item.id === id);
    if (!item) return;
    if (item.quantity > 1) {
      item.quantity--;
      item.stock++;
    } else {
      items.value = items.value.filter((item: { id: number }) => item.id !== id);
    }
    productsStore.increaseStock(id);
  };

  const trashItem = (id: number) => {
    const item = items.value.find((item: { id: number }) => item.id === id);
    if (!item) return;
    items.value = items.value.filter((item: { id: number }) => item.id !== id);
    productsStore.increaseStock(id, item.quantity);
  };

  const clearCart = () => {
    items.value.forEach((item: { id: number, quantity: number }) => {
      productsStore.increaseStock(item.id, item.quantity);
    });
    items.value = [];
  }

  return {
    items,
    totalItems,
    totalPrice,
    addToCart,
    removeFromCart,
    trashItem,
    clearCart
  };
});