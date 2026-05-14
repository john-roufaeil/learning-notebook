@extends('layouts.master')

@section('content')
  <div>
    <p>Title: {{ $post['title'] }}</p>
    <p>Description: {{ $post['description'] }}</p>
    <p>Name: {{ $post['user'] }}</p>
    <a href="{{ route('posts.index') }}">Homepage</a>
  </div>
@endsection