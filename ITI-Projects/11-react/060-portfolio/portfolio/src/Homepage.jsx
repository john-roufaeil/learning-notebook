import { Hero } from "./components/Hero"
import { About } from "./components/About"
import { Skills } from "./components/Skills"
import { Projects } from "./components/Projects"
import { Footer } from "./components/Footer"

export const Homepage = () => {
  return (
    <div className="max-w-5xl shadow-2xl min-h-[100vh] mx-auto scroll-smooth">
      <Hero />
      <About />
      <Skills />
      <Projects />
      <Footer />
    </div>
  )
}