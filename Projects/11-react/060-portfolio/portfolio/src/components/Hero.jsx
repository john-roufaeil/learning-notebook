import photoImage from '../assets/photo.jpg'

export const Hero = () => {
  return (
    <div className="flex justify-between gap-8 p-8 py-16 items-center border-b-1 border-slate-500">
      <div className='flex-1 flex flex-col gap-4 text-center'>
        <h1 className='tracking-lg text-5xl'>John Fayez</h1>
        <p className='tracking-tighter font-mono text-slate-500 text-lg'>SOFTWARE ENGINEER</p>
        <a href="#about" className='bg-blue-500 hover:bg-blue-700 w-fit mx-auto hover:cursor-pointer transition-all duration-300 text-white font-bold py-2 px-4 rounded'>
          Learn more
        </a>
      </div>
      <div className='flex-1'>
        <div className='w-1/2 mx-auto h-1/2'>
          <img src={photoImage} alt="" className='rounded-4xl' />
        </div>
      </div>
    </div>
  )
}