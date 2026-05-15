@extends('layouts.master')

@section('content')
    <div class="flex gap-4 mb-4">
        <x-button href="{{ route('posts.create') }}">Create new post</x-button>        
        <x-button href="{{ route('users.create') }}" variant="outline">Create new user</x-button>
    </div>

    <table class="border mb-4 w-2/3">
        <thead>
            <tr class="border-b">
                <th class="p-2 border-l">ID</th>
                <th class="p-2 border-l">Title</th>
                <th class="p-2 border-l">Posted By</th>
                <th class="p-2 border-l">Created At</th>
                <th class="p-2 border-l">Actions</th>
            </tr>
        </thead>
        <tbody>
            @foreach ($posts as $post)
                <tr class="border-b">
                    <td class="px-2 border-r">{{ $post['id'] }}</td>
                    <td class="px-2 border-r">{{ $post['title'] }}</td>
                    <td class="px-2 border-r">{{ $post->author->name }}</td>
                    <td class="px-2 border-r">{{ $post['created_at']->format('Y-m-d') }}</td>
                    <td class="px-2 flex gap-2">
                        <x-button href="{{ route('posts.show', $post['id']) }}" variant="ghost">View</x-button>
                        <x-button href="{{ route('posts.edit', $post['id']) }}" variant="ghost">Edit</x-button>
                        <form action="{{ route('posts.destroy', $post['id']) }}" method="POST" style="display:inline;"
                            onsubmit="return confirm('Confirm Delete')">
                            @csrf
                            @method('DELETE')
                            <x-button variant="danger" type="submit">Delete</x-button>
                        </form>
                    </td>
                </tr>
            @endforeach
        </tbody>
    </table>
    <div class="w-2/3">
        {{ $posts->links() }}
    </div>
@endsection