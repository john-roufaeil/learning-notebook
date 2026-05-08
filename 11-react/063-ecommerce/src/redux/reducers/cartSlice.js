import { createSlice } from "@reduxjs/toolkit";

const cartSlice = createSlice({
  name: "cart",
  initialState: {
    items: {},
    totalItems: 0,
    totalPrice: 0
  },
  reducers: {
    addToCart: (state, action) => {
      if (state.items[action.payload.id]) {
        state.items[action.payload.id].quantity += 1;
      } else {
        state.items[action.payload.id] = { ...action.payload, quantity: 1 };
      }
      state.totalItems += 1;
      state.totalPrice += action.payload.price;
    },
    removeFromCart: (state, action) => {
      state.totalPrice -= state.items[action.payload]?.price * state.items[action.payload]?.quantity || 0;
      state.totalItems -= state.items[action.payload]?.quantity || 0;
      delete state.items[action.payload];
    },
    decreaseQuantity: (state, action) => {
      const item = state.items[action.payload];
      if (item && item.quantity > 1) {
        item.quantity -= 1;
        state.totalItems -= 1;
        state.totalPrice -= item.price;
      } else {
        removeFromCart(state, { payload: action.payload });
      }
    }
  }
});

export const { addToCart, removeFromCart, increaseQuantity, decreaseQuantity } = cartSlice.actions;
export default cartSlice.reducer;