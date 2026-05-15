@extends('layouts.master')

@section('content')
<h2 class="text-center mb-2">Editing Blog "{{ $article->title }}"</h2>
<form action="{{ route('articles.update', $article['id']) }}" method="POST" class="border w-1/2 mx-auto p-4 py-12 rounded flex flex-col gap-4 items-center bg-slate-200 border-slate-300">
  @csrf
  @method('PUT')
  
    <div class="flex flex-col gap-2 justify-between w-4/5">
      <label class="font-bold">Title</label>
      <input class="ring rounded p-2" type="text" name="title" value="{{ $article['title'] }}">
        @error('title')
          <p style="color: red; font-size: 13px;">{{ $message }}</p>
        @enderror
    </div>

    <div class="flex flex-col gap-2 justify-between w-4/5">
      <label class="font-bold">Content</label>
      <textarea class="ring rounded p-2" name="content">{{ $article['content'] }}</textarea>
        @error('content')
          <p style="color: red; font-size: 13px;">{{ $message }}</p>
        @enderror
    </div>

    <div class="flex flex-col gap-2 justify-between w-4/5">
      <label class="font-bold">Article Author</label>
      <select name="author_id" class="ring rounded p-2">
        @foreach ($users as $id=> $name)
          <option value="{{ $id }}" {{ $id == $article['author_id'] ? 'selected' : '' }}>
            {{ $name }}
          </option>
        @endforeach
      </select>
      @error('author_id')
        <p style="color: red; font-size: 13px;">{{ $message }}</p>
      @enderror
    </div>

    <div class="flex gap-4">
      <x-button type="submit">Update</x-button>
      <x-button variant="outline" href="{{ route('articles.index') }}">Cancel</x-button>
    </div>
  </form>
@endsection