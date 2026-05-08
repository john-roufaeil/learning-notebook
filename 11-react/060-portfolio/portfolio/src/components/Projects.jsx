export const Projects = () => {
  return (
    <section
      id="projects"
      className="flex flex-col gap-12 p-8 py-16 border-b border-slate-500"
    >
      <div className="text-center">
        <h1 className="text-5xl tracking-wide">Projects</h1>
        <p className="text-slate-500 font-mono mt-2">
          Some of my work
        </p>
      </div>

      <div className="grid grid-cols-2 gap-8">
        <div className="bg-slate-600 text-slate-100 rounded-2xl shadow-lg ">
          <div className="p-4 flex flex-col gap-3">
            <h2 className="text-xl font-bold">Project One</h2>

            <p className="text-slate-300 text-sm">
              Lorem ipsum dolor sit amet consectetur adipisicing elit. Magnam vitae adipisci illum dignissimos porro asperiores dolorum labore velit sunt cumque.
            </p>
          </div>
        </div>
        <div className="bg-slate-600 text-slate-100 rounded-2xl shadow-lg ">
          <div className="p-4 flex flex-col gap-3">
            <h2 className="text-xl font-bold">Project Two</h2>

            <p className="text-slate-300 text-sm">
              Lorem ipsum dolor sit amet consectetur adipisicing elit. Magnam vitae adipisci illum dignissimos porro asperiores dolorum labore velit sunt cumque.
            </p>
          </div>
        </div>
        <div className="bg-slate-600 text-slate-100 rounded-2xl shadow-lg ">
          <div className="p-4 flex flex-col gap-3">
            <h2 className="text-xl font-bold">Project Three</h2>

            <p className="text-slate-300 text-sm">
              Lorem ipsum dolor sit amet consectetur adipisicing elit. Magnam vitae adipisci illum dignissimos porro asperiores dolorum labore velit sunt cumque.
            </p>
          </div>
        </div>
        <div className="bg-slate-600 text-slate-100 rounded-2xl shadow-lg ">
          <div className="p-4 flex flex-col gap-3">
            <h2 className="text-xl font-bold">Project Four</h2>

            <p className="text-slate-300 text-sm">
              Lorem ipsum dolor sit amet consectetur adipisicing elit. Magnam vitae adipisci illum dignissimos porro asperiores dolorum labore velit sunt cumque.
            </p>
          </div>
        </div>
      </div>
    </section>
  )
}