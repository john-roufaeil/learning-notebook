import { Formik, Field, ErrorMessage } from "formik";
import { useNavigate } from "react-router-dom";
import * as Yup from "yup";
import PhoneInput from 'react-phone-number-input'
import "react-phone-number-input/style.css";
import { useContext } from "react";
import { AuthContext } from "../../contexts/AuthContext";

function RegisterPage() {
  const navigate = useNavigate();
  const { setUser } = useContext(AuthContext);

  const registerSchema = Yup.object({
    username: Yup.string().required("* Username is required"),
    password: Yup.string().required("* Password is required"),
    confirmPassword: Yup.string().required("* Confirm Password is required").
      oneOf([Yup.ref('password'), null], "* Passwords must match"),
    phone: Yup.string().required("* Phone number is required"),
    email: Yup.string().required("* Email is required")
      .email("* Invalid email format")
  });

  return (
    <div className="mt-20 mx-auto max-w-xl flex flex-col items-center p-8 border-gray-200 bg-gray-100 border-2 rounded-xl m-8">
      <h1 className="font-bold text-2xl mb-4">Register</h1>

      <Formik
        initialValues={{ username: "", password: "", confirmPassword: "", phone: "", email: "" }}
        validationSchema={registerSchema}
        onSubmit={(values) => {
          localStorage.setItem("username", values.username);
          setUser(values.username);
          navigate('/products');
        }}
      >
        {({ handleSubmit, values, setFieldValue }) => (
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

            <div>
              <label className="block mb-1">Confirm Password</label>
              <Field
                type="password"
                name="confirmPassword"
                className="w-full border rounded px-3 py-2"
              />
              <ErrorMessage name="confirmPassword" component="div" className="text-red-600 text-sm" />
            </div>

            <div>
              <label className="block mb-1">Phone Number</label>
              <PhoneInput
                placeholder="Enter phone number"
                defaultCountry="EG"
                value={values.phone}
                onChange={(value) => setFieldValue("phone", value)}
                className="w-full border rounded px-3 py-2 mt-1"
              />
              <ErrorMessage name="phone" component="div" className="text-red-600 text-sm" />
            </div>

            <div>
              <label className="block mb-1">Email</label>
              <Field
                type="email"
                name="email"
                className="w-full border rounded px-3 py-2"
              />
              <ErrorMessage name="email" component="div" className="text-red-600 text-sm" />
            </div>

            <button
              type="submit"
              className="bg-blue-600 text-white py-2 px-4 rounded hover:bg-blue-700 transition"
            >
              Register
            </button>

            <div>
              <span>Already have an account?</span><a href="/login" className="hover:underline">
                &nbsp;Login
              </a>
            </div>
          </form>
        )}
      </Formik>
    </div>
  )
}

export default RegisterPage;