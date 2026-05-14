@extends('layouts.master')

@section('content')
  <form action="{{ route('posts.store') }}" method="POST" class="border w-1/2 mx-auto p-4 rounded flex flex-col gap-4">
    @csrf

    <div class="flex gap-4 justify-between">
        <label class="font-bold">Title</label>
        <input type="text" name="title" class="ring rounded w-4/5 p-2">
    </div>

    <div class="flex gap-4 justify-between">
        <label class="font-bold">Content</label>
        <textarea name="content" class="ring rounded w-4/5 p-2"></textarea>
    </div>

    <div class="flex gap-4 justify-between">
        <label class="font-bold">Post Creator</label>
        <select name="author_id" class="ring rounded w-4/5 p-2">
            @foreach ($users as $id => $name)
              <option value="{{ $id }}">{{ $name }}</option>
            @endforeach
        </select>
    </div>

    <x-button type="submit">Create</x-button>
    <x-button variant="outline" href="{{ route('posts.index') }}">Cancel</x-button>
  </form>
@endsection