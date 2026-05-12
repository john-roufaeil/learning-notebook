/* eslint-disable no-unused-vars */
const Header = ({ courseName }) => (<h1>{courseName}</h1>)

const Content = ({ content }) => (
  <>
    {content.map(item => (
      <Part key={item.name} part={item} />
    ))}
  </>
)

const Part = ({ part }) => (
  <p> {part.name} {part.exercises} </p>
)

const Total = ({ content }) => {
  const total = content.reduce((acc, curr) => acc + curr.exercises, 0)
  return <p>Total number of exercises: {total}</p>
}

const App = () => {
  const courseName = 'Half Stack application development'
  const content = [
    { name: 'Fundamentals of React', exercises: 10 },
    { name: 'Using props to pass data', exercises: 7 },
    { name: 'State of a component', exercises: 14 }
  ]

  return (
    <div>
      <Header courseName={courseName} />
      <Content content={content} />
      <Total content={content} />
    </div>
  )
}

export default App