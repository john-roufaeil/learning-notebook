import { computed } from "vue";

const CONVERSION_RATE = 35;
const formatCurrency = (amount: number) =>
  new Intl.NumberFormat("en-US", { style: "currency", currency: "EGP" })
    .format(amount * CONVERSION_RATE);

export const formatDiscountedPrice = (price: number, discount?: number): string => {
  const discounted = discount ? price * (1 - discount / 100) : price;
  return formatCurrency(discounted);
};

export const useProductPrice = (getProduct: () => { price: number, discount?: number }) => {
  const discountedPrice = computed(() => {
    const product = getProduct();
    return product.discount ?
      product.price * (1 - product.discount / 100) :
      product.price;
  });

  const formattedDiscountedPrice = computed(() => formatCurrency(discountedPrice.value));

  const formattedPrice = computed(() => formatCurrency(getProduct().price));

  return { discountedPrice, formattedDiscountedPrice, formattedPrice }
}