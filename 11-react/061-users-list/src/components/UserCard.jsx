import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faEnvelope, faUser, faPhone, faCalendarDays } from '@fortawesome/free-solid-svg-icons';

export const UserCard = ({ role, username, email, phone, birthDate, imgUrl }) => {
  return (
    <div className="group rounded-xl border-2 p-4 shadow-md border-gray-300 hover:border-cyan-500 transition-colors duration-300 w-full">
      <img src={imgUrl} alt={username} className="group-hover:scale-103 transition-all duration-300 mx-auto h-48 object-cover rounded-full border-4 border-cyan-500 mt-4 mb-8" />

      <div className="flex flex-col gap-1 text-center mb-2">
        <p className={` text-white text-center rounded-lg w-1/2 mx-auto
          ${role === 'admin' ? 'bg-red-500' :
            role === 'user' ? 'bg-green-500' :
              'bg-yellow-500'}`}>
          {role}
        </p>

        <div className='text-left flex flex-col gap-2 mt-4 w-3/4 mx-auto'>
          <p><FontAwesomeIcon icon={faUser} /> {username}</p>
          <p><FontAwesomeIcon icon={faEnvelope} /> {email}</p>
          <p><FontAwesomeIcon icon={faPhone} /> {phone}</p>
          <p><FontAwesomeIcon icon={faCalendarDays} /> {birthDate}</p>
        </div>
      </div>
    </div>
  )
}