import { Navigate, Outlet } from 'react-router-dom';

const ProtectedRoute = () => {
  const user = localStorage.getItem("username");
  return (
    user ? <Outlet /> : <Navigate to="/login" replace />
  )
};

export default ProtectedRoute;
