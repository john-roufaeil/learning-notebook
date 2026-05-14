@extends('layouts.master')

@section('content')
  <form action="{{ route('posts.store') }}" method="POST">
    @csrf

    <div>
        <label>Title</label>
        <input type="text" name="title">
    </div>

    <div>
        <label>Description</label>
        <textarea name="description"></textarea>
    </div>

    <div>
        <label>Post Creator</label>
        <select name="user">
            @foreach ($users as $user)
              <option value="{{ $user }}">{{ $user }}</option>
            @endforeach
        </select>
    </div>

    <button>Create</button>
    <a href="{{ route('posts.index') }}">Cancel</a>
  </form>
@endsection