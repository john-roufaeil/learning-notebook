import { computed } from "vue";

const CONVERSION_RATE = 35;

interface Product {
  price: number;
  discount?: number;
}

export const useProductPrice = (product: Product) => {
  const discountedPrice = computed(() => {
    return product.discount ?
      product.price * (1 - product.discount / 100) :
      product.price;
  });


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
    }).format(product.price * CONVERSION_RATE),
  );

  return {
    discountedPrice,
    formattedDiscountedPrice,
    formattedPrice
  }
}