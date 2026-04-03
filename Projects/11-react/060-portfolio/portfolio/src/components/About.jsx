import CVFile from '../assets/CV.pdf';

export const About = () => {
  return (
    <div id="about" className="flex justify-between gap-8 p-8 py-16 items-start border-b-1 border-slate-500">
      <div className='flex-1 flex flex-col gap-4 text-center'>
        <h1 className='tracking-lg text-5xl'>About me</h1>
      </div>
      <div className='flex-3 flex flex-col gap-4 justify-start items-start'>
        <p>Lorem ipsum dolor sit amet consectetur adipisicing elit. Neque illum in nisi aut totam ipsa obcaecati, quo voluptatem amet natus sed fugiat pariatur vel est ullam laboriosam? Consequuntur voluptas cum nesciunt nisi odit adipisci optio, quisquam est, maxime culpa rem dignissimos illum hic velit quo harum aliquid voluptatem architecto accusantium nam soluta iusto enim ex iste. Asperiores quisquam quas repudiandae eius amet mollitia esse expedita beatae tempore ab non laborum aspernatur, et perspiciatis unde, corrupti eos tenetur id saepe nostrum adipisci sed quam molestias! Aut quo porro, facere omnis aperiam iure doloribus voluptatibus quam necessitatibus reiciendis excepturi eius optio voluptate.</p>
        <a href={CVFile} download="CV.pdf">
          <button className='bg-transparent hover:bg-blue-700 hover:border-blue-700 text-blue-500 border-2 border-blue-500 w-fit hover:cursor-pointer transition-all duration-300 hover:text-white font-bold py-2 px-4 rounded'>
            Download CV
          </button>
        </a>
      </div>
    </div>
  )
}