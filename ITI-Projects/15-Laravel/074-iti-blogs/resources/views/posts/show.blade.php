@extends('layouts.master')

@section('content')
  <div class="flex flex-col gap-2">
    <p><span class="font-bold">ID:</span> {{ $post->id }}</p>
    <p><span class="font-bold">Title:</span> {{ $post->title }}</p>
    <p><span class="font-bold">Content:</span> {{ $post->content }}</p>
    <p><span class="font-bold">Author:</span> {{ $post->author->name }}</p>
    <p><span class="font-bold">Created At:</span> {{ $post->created_at->format('l jS \o\f F Y h:i:s A') }}</p>
    <x-button href="{{ route('posts.index') }}" variant="ghost" class="w-fit">Homepage</x-button>
  </div>
@endsection