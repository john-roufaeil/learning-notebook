import { useContext } from "react";
import { AuthContext } from "../../contexts/AuthContext";
import { Formik, Field, ErrorMessage } from "formik";
import { useNavigate } from "react-router-dom";
import * as Yup from "yup";

function LoginPage() {
  const { setUser } = useContext(AuthContext);
  const navigate = useNavigate();

  const loginSchema = Yup.object({
    username: Yup.string().required("* Username is required"),
    password: Yup.string().required("* Password is required")
  });

  return (
    <div className="mt-20 mx-auto max-w-xl flex flex-col items-center p-8 border-gray-200 bg-gray-100 border-2 rounded-xl ">
      <h1 className="font-bold text-2xl mb-4">Login to Your Account</h1>

      <Formik
        initialValues={{ username: "", password: "" }}
        validationSchema={loginSchema}
        onSubmit={(values) => {
          localStorage.setItem("username", values.username);
          setUser(values.username);
          navigate('/products');
        }}
      >
        {({ handleSubmit }) => (
          <form onSubmit={handleSubmit} className="space-y-4 max-w-sm flex-col flex">
            <div>
              <label className="block mb-1">Username</label>
              <Field
                type="text"
                name="username"
                className="w-full border rounded px-3 py-2"
              />
              <ErrorMessage name="username" component="div" className="text-red-600 text-sm" />
            </div>

            <div>
              <label className="block mb-1">Password</label>
              <Field
                type="password"
                name="password"
                className="w-full border rounded px-3 py-2"
              />
              <ErrorMessage name="password" component="div" className="text-red-600 text-sm" />
            </div>

            <button
              type="submit"
              className="bg-blue-600 text-white py-2 px-4 rounded hover:bg-blue-700 transition"
            >
              Login
            </button>

            <div>
              <span>Don't have an account?</span><a href="/register" className="hover:underline">
                &nbsp;Register
              </a>
            </div>
          </form>
        )}
      </Formik>
    </div>
  )
}

export default LoginPage;