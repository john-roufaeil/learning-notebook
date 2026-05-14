@extends('layouts.master')

@section('content')
  <form action="{{ route('posts.update', $post['id']) }}" method="POST">
    @csrf
    @method('PUT')

    <div>
      <label>Title</label>
      <input type="text" name="title" value="{{ $post['title'] }}">
    </div>

    <div>
      <label>Description</label>
      <textarea name="description">{{ $post['description'] }}</textarea>
    </div>

    <div>
      <label>Post Creator</label>
      <select name="user">
        @foreach ($users as $user)
          <option value="{{ $user }}" {{ $user == $post['user'] ? 'selected' : '' }}>
            {{ $user }}
          </option>
        @endforeach
      </select>
    </div>

    <button>Update</button>
    <a href="{{ route('posts.index') }}">Cancel</a>
  </form>
@endsection