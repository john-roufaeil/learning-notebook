@extends('layouts.master')

@section('content')
    <div>
        <a href="{{ route('posts.create') }}">Create Post</a>
    </div>

    <table>
        <thead>
            <tr>
                <th>ID</th>
                <th>Title</th>
                <th>Posted By</th>
                <th>Actions</th>
            </tr>
        </thead>
        <tbody>
            @foreach ($posts as $post)
                <tr>
                    <td>{{ $post['id'] }}</td>
                    <td>{{ $post['title'] }}</td>
                    <td>{{ $post['user'] }}</td>
                    <td>
                        <a href="{{ route('posts.show', $post['id']) }}">View</a>
                        <a href="{{ route('posts.edit', $post['id']) }}">Edit</a>
                        <form action="{{ route('posts.destroy', $post['id']) }}" method="POST" style="display:inline;">
                            @csrf
                            @method('DELETE')
                            <button>Delete</button>
                        </form>
                    </td>
                </tr>
            @endforeach
        </tbody>
    </table>
@endsection