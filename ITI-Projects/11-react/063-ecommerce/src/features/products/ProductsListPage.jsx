import { useEffect, useState } from "react";
import ProductCard from "./components/ProductCard";
import Pagination from "./components/Pagination";
import axios from "axios";

function ProductsListPage() {
  const [products, setProducts] = useState([]);
  const [page, setPage] = useState(1);
  const [totalPages, setTotalPages] = useState(0);

  useEffect(() => {
    const skip = (page - 1) * 10;
    axios.get(`https://dummyjson.com/products?limit=10&page=${page}&skip=${skip}`)
      .then(res => {
        setProducts(res.data.products);
        setTotalPages(Math.ceil(res.data.total / 10));
      })
      .catch(err => console.error("Error fetching products:", err));
  }, [page])

  return (
    <div className="p-6">
      <h1 className="text-center my-6 text-3xl font-bold">Products</h1>

      <Pagination page={page} setPage={setPage} totalPages={totalPages} />

      <div className="grid grid-cols-5 gap-6">
        {products.map(product => (
          <ProductCard key={product.id} product={product} />
        ))}
      </div>

      <Pagination page={page} setPage={setPage} totalPages={totalPages} />
    </div>
  )
}

export default ProductsListPage;