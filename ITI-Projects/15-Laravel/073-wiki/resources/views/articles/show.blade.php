@extends('layouts.master')

@section('content')
  <div class="flex flex-col gap-2">
    <p><span class="font-bold">ID:</span> {{ $article->id }}</p>
    <p><span class="font-bold">Title:</span> {{ $article->title }}</p>
    <p><span class="font-bold">Content:</span> {{ $article->content }}</p>
    <p><span class="font-bold">Author:</span> {{ $article->author->name }}</p>
    <p><span class="font-bold">Created At:</span> {{ $article->created_at->format('l jS \o\f F Y h:i:s A') }}</p>
    <x-button href="{{ route('articles.index') }}" variant="ghost" class="w-fit">Homepage</x-button>
  </div>
@endsection