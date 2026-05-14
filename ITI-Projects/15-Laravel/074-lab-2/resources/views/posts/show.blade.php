@extends('layouts.master')

@section('content')
  <div>
    <p>Title: {{ $post->title }}</p>
    <p>Content: {{ $post->content }}</p>
    <p>Author: {{ $post->author->name }}</p>
    <p>Created At: {{ $post->created_at->format('H:i d/m/Y') }}</p>
    <a href="{{ route('posts.index') }}">Homepage</a>
  </div>
@endsection