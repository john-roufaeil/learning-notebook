import { Routes, Route } from "react-router-dom";
import NotFound from "./pages/NotFound.jsx";
import Layout from "./shared/Layout.js";
import CartPage from "./features/cart/CartPage.jsx";
import ProductDetailsPage from "./features/products/ProductDetailsPage.jsx";
import ProductsListPage from "./features/products/ProductsListPage.jsx";

function App() {
  return (
    <Routes>
      <Route element={<Layout />}>
        <Route path="/products" element={<ProductsListPage />} />
        <Route path="/products/:id" element={<ProductDetailsPage />} />
        <Route path="/cart" element={<CartPage />} />
        <Route path="*" element={<NotFound />} />
      </Route>
    </Routes >
  )
}

export default App;