import { UserCard } from "../components/UserCard";
import { useState } from "react";
import { UserSearch } from "../components/UserSearch";

export const UsersPage = () => {
  const [search, setSearch] = useState('');

  const users = [
    {
      id: 1,
      role: 'admin',
      username: 'john',
      email: 'john@email.com',
      phone: '1234567890',
      birthDate: '2000-01-01',
      imgUrl: 'https://randomuser.me/api/portraits/lego/1.jpg'
    },
    {
      id: 2,
      role: 'user',
      username: 'jane',
      email: 'jane@email.com',
      phone: '9876543210',
      birthDate: '2002-02-02',
      imgUrl: 'https://randomuser.me/api/portraits/lego/2.jpg'
    },
    {
      id: 3,
      role: 'moderator',
      username: 'moderator',
      email: 'moderator@email.com',
      phone: '1231231234',
      birthDate: '1995-05-05',
      imgUrl: 'https://randomuser.me/api/portraits/lego/3.jpg'
    },
    {
      id: 4,
      role: 'user',
      username: 'alice',
      email: 'alice@email.com',
      phone: '3213214321',
      birthDate: '1998-08-08',
      imgUrl: 'https://randomuser.me/api/portraits/lego/4.jpg'
    },
    {
      id: 5,
      role: 'user',
      username: 'bob',
      email: 'bob@email.com',
      phone: '2222222222',
      birthDate: '1990-01-01',
      imgUrl: 'https://randomuser.me/api/portraits/lego/5.jpg'
    },
    {
      id: 6,
      role: 'user',
      username: 'charlie',
      email: 'charlie@email.com',
      phone: '4444444444',
      birthDate: '1992-03-03',
      imgUrl: 'https://randomuser.me/api/portraits/lego/6.jpg'
    },
    {
      id: 7,
      role: 'admin',
      username: 'dave',
      email: 'dave@email.com',
      phone: '3333333333',
      birthDate: '1985-07-07',
      imgUrl: 'https://randomuser.me/api/portraits/lego/7.jpg'
    },
    {
      id: 8,
      role: 'moderator',
      username: 'eve',
      email: 'eve@email.com',
      phone: '4444444444',
      birthDate: '1999-09-09',
      imgUrl: 'https://randomuser.me/api/portraits/lego/8.jpg'
    }
  ];

  const filteredUsers = users.filter(user =>
    user.email.includes(search)
  )

  return (
    <div className="container mx-auto p-4">
      <h1 className="text-3xl font-bold mb-6 text-center">Users List</h1>

      <UserSearch search={search} setSearch={setSearch} />

      <div className="grid grid-cols-4 gap-6">
        {filteredUsers.map(user => (
          <UserCard key={user.id} {...user} />
        ))}
      </div>
    </div>
  )
}