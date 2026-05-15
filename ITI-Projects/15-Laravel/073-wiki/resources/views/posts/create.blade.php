@extends('layouts.master')

@section('content')
  <h2 class="text-center mb-4 text-xl">New Post</h2>
  <form action="{{ route('posts.store') }}" method="POST" class="border w-1/2 mx-auto p-4 py-12 rounded flex flex-col gap-4 items-center bg-slate-200 border-slate-300">
    @csrf

    <div class="flex flex-col gap-2 justify-between w-4/5">
        <label class="font-bold">Title</label>
        <input type="text" name="title" class="ring rounded p-2">
        @error('title')
          <p style="color: red; font-size: 13px;">{{ $message }}</p>
        @enderror
    </div>

    <div class="flex flex-col gap-2 justify-between w-4/5">
        <label class="font-bold">Content</label>
        <textarea name="content" class="ring rounded p-2"></textarea>
        @error('content')
          <p style="color: red; font-size: 13px;">{{ $message }}</p>
        @enderror
    </div>

    <div class="flex flex-col gap-2 justify-between w-4/5">
        <label class="font-bold">Post Creator</label>
        <select name="author_id" class="ring rounded p-2">
            @foreach ($users as $id => $name)
              <option value="{{ $id }}">{{ $name }}</option>
            @endforeach
        </select>
        @error('author_id')
          <p style="color: red; font-size: 13px;">{{ $message }}</p>
        @enderror
    </div>

    <div class="flex gap-4">
      <x-button type="submit">Create</x-button>
      <x-button variant="outline" href="{{ route('posts.index') }}">Cancel</x-button>
    </div>
  </form>
@endsection