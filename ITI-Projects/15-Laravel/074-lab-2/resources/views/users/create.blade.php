@extends('layouts.master')

@section('content')
  <form action="{{ route('users.store') }}" method="POST">
    @csrf

    <div>
        <label>Name</label>
        <input type="text" name="name">
    </div>

    <div>
        <label>Email</label>
        <input type="email" name="email">
    </div>

    <div>
        <label>Password</label>
        <input type="password" name="password">
    </div>

    <button>Create User</button>
    <a href="{{ route('posts.index') }}">Cancel</a>
  </form>
@endsection