import { createRoot } from 'react-dom/client'
import { BrowserRouter } from "react-router-dom";
import LanguageProvider from "./providers/LanguageProvider.jsx";
import { Provider } from 'react-redux';
import store from './redux/store.js';
import App from './App.jsx'
import AuthProvider from './providers/AuthProvider.jsx';

createRoot(document.getElementById('root')).render(
  <LanguageProvider>
    <AuthProvider>
      <Provider store={store}>
        <BrowserRouter>
          <App />
        </BrowserRouter>
      </Provider>
    </AuthProvider>
  </LanguageProvider>
)
