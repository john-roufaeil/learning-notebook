import { defineStore } from "pinia";
import { computed } from "vue";
import { useApi } from "@/composables/useApi";

export const useProductsStore = defineStore("products", () => {
  const { data: products, loading, error, getAll, update }
    = useApi("http://localhost:3000/products");

  const fetchProducts = async () => {
    await getAll();
  };

  const getProductById = computed(() => (id: number) => {
    return products.value.find((p: any) => p.id == id);
  });

  const decreaseStock = async (id: number) => {
    try {
      const product = products.value.find((p: any) => p.id == id);
      if (!product || product.stock <= 0) return;
      product.stock--;
      await update(id, product);
    } catch (err) {
      console.error("Failed to sync stock reduction to server:", err);
    }
  };

  const increaseStock = async (id: number, by: number = 1) => {
    try {
      const product = products.value.find((p: any) => p.id == id);
      if (!product) return;
      product.stock += by;
      await update(id, product);
    } catch (err) {
      console.error("Failed to sync stock increase to server:", err);
    }
  };

  return { products, error, loading, fetchProducts, getProductById, decreaseStock, increaseStock };
});