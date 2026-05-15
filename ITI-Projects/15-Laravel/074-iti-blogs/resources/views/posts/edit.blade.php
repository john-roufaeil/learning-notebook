@extends('layouts.master')

@section('content')
<h2 class="font-semibold text-center mb-2">Editing Blog "{{ $post->title }}"</h2>
<form action="{{ route('posts.update', $post['id']) }}" method="POST" class="border w-1/2 mx-auto p-4 rounded flex flex-col gap-4">
  @csrf
  @method('PUT')
  
    <div class="flex gap-4 justify-between">
      <label class="font-bold">Title</label>
      <input class="ring rounded w-4/5 p-2" type="text" name="title" value="{{ $post['title'] }}">
    </div>

    <div class="flex gap-4 justify-between">
      <label class="font-bold">Content</label>
      <textarea class="ring rounded w-4/5 p-2" name="content">{{ $post['content'] }}</textarea>
    </div>

    <div class="flex gap-4 justify-between">
      <label class="font-bold">Post Author</label>
      <select name="author_id" class="ring rounded w-4/5 p-2">
        @foreach ($users as $id=> $name)
          <option value="{{ $id }}" {{ $id == $post['author_id'] ? 'selected' : '' }}>
            {{ $name }}
          </option>
        @endforeach
      </select>
    </div>

    <x-button type="submit">Update</x-button>
    <x-button variant="outline" href="{{ route('posts.index') }}">Cancel</x-button>
  </form>
@endsection