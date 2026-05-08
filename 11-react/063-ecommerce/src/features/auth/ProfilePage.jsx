import { useContext } from "react";
import { AuthContext } from "../../contexts/AuthContext";

function ProfilePage() {
  const { user } = useContext(AuthContext);
  return (
    <div>
      <h1>Profile Page</h1>
      <p>Welcome, {user}!</p>
      <button onClick={() => {
        localStorage.removeItem("username");
        window.location.href = "/login";
      }} className="bg-red-600 text-white py-2 px-4 rounded hover:bg-red-700 transition">
        Logout
      </button>
    </div>
  )
}

export default ProfilePage;