import { Outlet } from "react-router-dom";
import Navbar from "./Navbar";
import { useContext } from "react";
import { LanguageContext } from "../contexts/LanguageContext";

function Layout() {
  const { lang, setLang } = useContext(LanguageContext);
  return (
    <div dir={lang === "ar" ? "rtl" : "ltr"}>
      <Navbar />
      <main>
        <Outlet />
      </main>
    </div >
  )
}

export default Layout;