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
      <label>Content</label>
      <textarea name="content">{{ $post['content'] }}</textarea>
    </div>

    <div>
      <label>Post Author</label>
      <select name="author_id">
        @foreach ($users as $id=> $name)
          <option value="{{ $id }}" {{ $id == $post['author_id'] ? 'selected' : '' }}>
            {{ $name }}
          </option>
        @endforeach
      </select>
    </div>

    <button>Update</button>
    <a href="{{ route('posts.index') }}">Cancel</a>
  </form>
@endsection