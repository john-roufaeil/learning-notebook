@extends('layouts.master')

@section('content')
  <form action="{{ route('posts.store') }}" method="POST">
    @csrf

    <div>
        <label>Title</label>
        <input type="text" name="title">
    </div>

    <div>
        <label>Content</label>
        <textarea name="content"></textarea>
    </div>

    <div>
        <label>Post Creator</label>
        <select name="author_id">
            @foreach ($users as $id => $name)
              <option value="{{ $id }}">{{ $name }}</option>
            @endforeach
        </select>
    </div>

    <button>Create</button>
    <a href="{{ route('posts.index') }}">Cancel</a>
  </form>
@endsection