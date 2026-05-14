@extends('layouts.master')

@section('content')
    <div>
        <a href="{{ route('posts.create') }}">Create Post</a>
        <br /><br />
        <a href="{{ route('users.create') }}">Create User</a>
        <br /><br />
    </div>

    <table>
        <thead>
            <tr>
                <th>ID</th>
                <th>Title</th>
                <th>Posted By</th>
                <th>Created At</th>
                <th>Actions</th>
            </tr>
        </thead>
        <tbody>
            @foreach ($posts as $post)
                <tr>
                    <td>{{ $post['id'] }}</td>
                    <td>{{ $post['title'] }}</td>
                    <td>{{ $post->author->name }}</td>
                    <td>{{ $post['created_at']->format('H:i d/m/Y') }}</td>
                    <td>
                        <a href="{{ route('posts.show', $post['id']) }}">View</a>
                        <a href="{{ route('posts.edit', $post['id']) }}">Edit</a>
                        <form action="{{ route('posts.destroy', $post['id']) }}" method="POST" style="display:inline;"
                            onsubmit="return confirm('Confirm Delete')">
                            @csrf
                            @method('DELETE')
                            <button>Delete</button>
                        </form>
                    </td>
                </tr>
            @endforeach
        </tbody>
    </table>
    {{ $posts->links() }}
@endsection