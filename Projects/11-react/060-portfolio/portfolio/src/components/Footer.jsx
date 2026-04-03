import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faGithub, faLinkedin } from '@fortawesome/free-brands-svg-icons'
import { faEnvelope } from '@fortawesome/free-solid-svg-icons';

export const Footer = () => {
  return (
    <footer className="flex flex-col items-center gap-4 p-6 text-center">
      <h2 className="text-xl font-semibold">John Fayez</h2>
      <div className="flex gap-6 mt-2">
        <a href="https://www.github.com/john-roufaeil" className="text-slate-400 hover:text-blue-500 transition" >
          <FontAwesomeIcon icon={faGithub} />
        </a>
        <a href="linkedin.com/john-roufaeil" className="text-slate-400 hover:text-blue-500 transition" >
          <FontAwesomeIcon icon={faLinkedin} />
        </a>
        <a href="mailto:john.f.roufaeil@gmail.com" className="text-slate-400 hover:text-blue-500 transition" >
          <FontAwesomeIcon icon={faEnvelope} />
        </a>
      </div>

      <p className="text-xs text-slate-600 mt-4">
        © {new Date().getFullYear()} John Fayez
      </p>
    </footer>
  )
}