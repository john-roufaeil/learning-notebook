@extends('layouts.master')

@section('content')

    <div style="display:flex; align-items:center; justify-content:space-between; margin-bottom:1.5rem;">
        <h2>Articles</h2>
    </div>

    @foreach ($articles as $article)
        <div class="card" style="display:grid; grid-template-columns:1fr auto; gap:1rem; align-items:start; margin-bottom:12px;">

            <div>
                <div style="display:flex; align-items:center; gap:8px; flex-wrap:wrap; margin-bottom:8px;">
                    <code style="font-size:11px; color:var(--color-muted); background:var(--color-bg); padding:2px 8px; border-radius:4px; border:1px solid var(--color-border);">
                        /{{ $article->slug }}
                    </code>
                    <span style="font-size:12px; color:var(--color-muted);">{{ $article->author->name }}</span>
                    <span style="font-size:12px; color:var(--color-muted);">·</span>
                    <span style="font-size:12px; color:var(--color-muted);">{{ $article->created_at->format('Y-m-d') }}</span>
                </div>

                <p style="font-size:15px; font-weight:500; color:var(--color-text); margin:0 0 6px;">
                    {{ $article->title }}
                </p>

                <p style="font-size:13px; color:var(--color-muted); line-height:1.6; margin:0;
                           display:-webkit-box; -webkit-line-clamp:2; -webkit-box-orient:vertical; overflow:hidden;">
                    {{ $article->body }}
                </p>
            </div>

            <div style="display:flex; flex-direction:column; gap:6px; align-items:flex-end;">
                <span style="font-size:11px; color:var(--color-muted); font-family:monospace;">#{{ $article->id }}</span>
                <x-button href="{{ route('articles.show', $article) }}" variant="ghost">View</x-button>
                <x-button href="{{ route('articles.edit', $article) }}" variant="ghost">Edit</x-button>
                <form action="{{ route('articles.destroy', $article) }}" method="POST"
                      onsubmit="return confirm('Delete this article?')">
                    @csrf
                    @method('DELETE')
                    <x-button variant="danger" type="submit">Delete</x-button>
                </form>
            </div>

        </div>
    @endforeach

    <div style="display:flex; align-items:center; justify-content:space-between; margin-top:1.5rem;">
        <div style="display:flex; gap:8px;">
            <x-button href="{{ route('articles.create') }}">New article</x-button>
            <x-button href="{{ route('users.create') }}" variant="outline">New user</x-button>
        </div>
        <div>{{ $articles->links() }}</div>
    </div>

@endsection