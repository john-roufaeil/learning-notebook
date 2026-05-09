import { useState } from "react";
import { LanguageContext } from "../contexts/LanguageContext";

const LanguageProvider = ({ children }) => {
  const [lang, setLang] = useState("en");

  return (
    <LanguageContext.Provider value={{ lang, setLang }}>
      {children}
    </LanguageContext.Provider>
  )
}

export default LanguageProvider;