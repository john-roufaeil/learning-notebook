import './skills.css'

export const Skills = () => {
  return (
    <section id="skills" className="skills">
      <div className="skills-header">
        <h1>Skills</h1>
        <p>Technologies I work with</p>
      </div>

      <div className="skills-content">
        <div className="skills-category">
          <h2>Frontend</h2>
          <div className="skills-list">
            <span>React</span>
            <span>TypeScript</span>
            <span>JavaScript</span>
            <span>HTML</span>
            <span>CSS</span>
          </div>
        </div>

        <div className="skills-category">
          <h2>Backend</h2>
          <div className="skills-list">
            <span>Python</span>
            <span>Node.js</span>
            <span>Java</span>
            <span>SQL</span>
          </div>
        </div>

        <div className="skills-category">
          <h2>Tools</h2>
          <div className="skills-list">
            <span>Git</span>
            <span>Docker</span>
            <span>AWS</span>
            <span>Linux</span>
          </div>
        </div>
      </div>
    </section>
  )
}