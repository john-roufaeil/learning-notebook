import { Formik, Field, ErrorMessage } from "formik";
import * as Yup from "yup";

function ContactPage() {
  const contactSchema = Yup.object({
    email: Yup.string().required("* Email is required")
      .email("* Invalid email format"),
    message: Yup.string().required("* Message is required")
      .min(10, "* Message is too short")
      .max(200, "* Message is too long"),
    firstName: Yup.string().required("* First name is required"),
    lastName: Yup.string().required("* Last name is required"),
    phoneNumber: Yup.string()
  });

  return (
    <div className="mt-20 mx-auto max-w-xl flex flex-col items-center p-8 border-gray-200 bg-gray-100 border-2 rounded-xl m-8">
      <h1 className="font-bold text-2xl mb-4">Contact Us</h1>

      <Formik
        initialValues={{ email: "", message: "", firstName: "", lastName: "", phoneNumber: "" }}
        validationSchema={contactSchema}
        onSubmit={(values, { resetForm }) => {
          alert("Message sent! We'll get back to you soon.");
          resetForm();
        }}
      >
        {({ handleSubmit }) => (
          <form onSubmit={handleSubmit} className="space-y-4 max-w-sm flex-col flex">

            <div>
              <label className="block mb-1">First Name</label>
              <Field
                type="text"
                name="firstName"
                className="w-full border rounded px-3 py-2"
              />
              <ErrorMessage name="firstName" component="div" className="text-red-600 text-sm" />
            </div>

            <div>
              <label className="block mb-1">Last Name</label>
              <Field
                type="text"
                name="lastName"
                className="w-full border rounded px-3 py-2"
              />
              <ErrorMessage name="lastName" component="div" className="text-red-600 text-sm" />
            </div>

            <div>
              <label className="block mb-1">Phone Number</label>
              <Field
                type="text"
                name="phoneNumber"
                className="w-full border rounded px-3 py-2"
              />
              <ErrorMessage name="phoneNumber" component="div" className="text-red-600 text-sm" />
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

            <div>
              <label className="block mb-1">Message</label>
              <Field
                as="textarea"
                name="message"
                className="w-full border rounded px-3 py-2"
              />
              <ErrorMessage name="message" component="div" className="text-red-600 text-sm" />
            </div>

            <button
              type="submit"
              className="bg-blue-600 text-white py-2 px-4 rounded hover:bg-blue-700 transition"
            >
              Send Message
            </button>
          </form>
        )}
      </Formik>
    </div>
  )
}

export default ContactPage;