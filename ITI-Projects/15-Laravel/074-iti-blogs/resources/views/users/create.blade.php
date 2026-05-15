@extends('layouts.master')

@section('content')
  <form action="{{ route('users.store') }}" method="POST" class="border w-1/2 mx-auto p-4 rounded flex flex-col gap-4">
    @csrf

    <div class="flex gap-4 justify-between">
        <label class="font-bold">Name</label>
        <input type="text" name="name" class="ring rounded w-4/5 p-2">
    </div>

    <div class="flex gap-4 justify-between">
        <label class="font-bold">Email</label>
        <input type="email" name="email" class="ring rounded w-4/5 p-2">
    </div>

    <div class="flex gap-4 justify-between">
        <label class="font-bold">Password</label>
        <input type="password" name="password" class="ring rounded w-4/5 p-2">
    </div>

    <x-button type="submit">Create User</x-button>
    <x-button variant="outline" href="{{ route('posts.index') }}">Cancel</x-button>
  </form>
@endsection