import { Routes, Route } from "react-router-dom";
import NotFound from "./pages/NotFound.jsx";
import Layout from "./shared/Layout.jsx";
import CartPage from "./features/cart/CartPage.jsx";
import ProductDetailsPage from "./features/products/ProductDetailsPage.jsx";
import ProductsListPage from "./features/products/ProductsListPage.jsx";
import LoginPage from "./features/auth/LoginPage.jsx";
import RegisterPage from "./features/auth/RegisterPage.jsx";
import ContactPage from "./pages/ContactPage.jsx";
import ProfilePage from "./features/auth/ProfilePage.jsx";
import ProtectedRoute from "./shared/components/ProtectedRoute.jsx";
import CheckoutPage from "./features/cart/CheckoutPage.jsx";

function App() {
  return (
    <Routes>
      <Route element={<Layout />}>
        <Route path="/products" element={<ProductsListPage />} />
        <Route path="/products/:id" element={<ProductDetailsPage />} />
        <Route path="/cart" element={<CartPage />} />
        <Route path="/login" element={<LoginPage />} />
        <Route path="/register" element={<RegisterPage />} />
        <Route path="/contact" element={<ContactPage />} />
        <Route path="*" element={<NotFound />} />
        <Route element={<ProtectedRoute />}>
          <Route path="/profile" element={<ProfilePage />} />
          <Route path="/checkout" element={<CheckoutPage />} />
        </Route>
      </Route>
    </Routes >
  )
}

export default App;