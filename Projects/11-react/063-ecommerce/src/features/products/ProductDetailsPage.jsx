import { useEffect, useState } from "react";
import { useParams } from "react-router-dom";
import { faStar } from "@fortawesome/free-solid-svg-icons";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import axios from "axios";

function ProductDetailsPage() {
  const { id } = useParams();
  const [product, setProduct] = useState(null);

  useEffect(() => {
    axios.get(`https://dummyjson.com/products/${id}`)
      .then(res => setProduct(res.data))
      .catch(err => console.error("Error fetching product:", err));
  }, [id]);

  useEffect(() => {
    console.log(product);
  }, [product]);

  return (
    <div className="p-6">
      {product && (
        <>
          <div className="flex gap-6">
            <img src={product.images[0]} alt={product.title} className="w-1/3 rounded-lg bg-gray-200" />
            <div>
              <h1 className="my-6 text-3xl font-bold">{product.title}</h1>
              <p className="text-gray-600 mb-4">{product.description}</p>
              <p className="text-xl font-bold mb-4">${product.price.toFixed(2)}</p>
              <p className="text-gray-600 mb-4">
                Rating: {product.rating} ({product.reviews.length}) <FontAwesomeIcon icon={faStar} className="text-yellow-500" />
              </p>
              <hr className="my-4 text-gray-300" />
              <p className="text-gray-600 mb-4">Brand: {product.brand}</p>
              <p className="text-gray-600 mb-4">Category: {product.category}</p>
              <button className="bg-blue-500 text-white p-2 rounded-lg hover:bg-blue-600 hover:cursor-pointer">
                Add to Cart
              </button>
            </div>
          </div>
        </>
      )}
    </div>
  )
}

export default ProductDetailsPage;