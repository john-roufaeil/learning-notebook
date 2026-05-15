@extends('layouts.master')

@section('content')

    {{-- Navigation & Utility Header --}}
    <div class="flex items-center justify-between mb-6">
        <x-button href="{{ route('articles.index') }}" variant="secondary" tag="a">
            &larr; Back to Articles
        </x-button>
        <div class="flex gap-2">
            <x-button href="{{ route('articles.edit', $article) }}" variant="secondary" tag="a">Edit</x-button>
            <form action="{{ route('articles.destroy', $article) }}" method="POST"
                  onsubmit="return confirm('Delete this article?')" class="inline">
                @csrf
                @method('DELETE')
                <x-button variant="danger" type="submit">Delete</x-button>
            </form>
        </div>
    </div>

    {{-- Main Article Card --}}
    <div class="card p-6 md:p-8 rounded-xl border border-slate-200 bg-white shadow-sm">
        
        {{-- Metadata Header --}}
        <div class="flex flex-wrap items-center justify-between gap-4 pb-4 mb-6 border-b border-slate-100 text-xs font-mono text-slate-500">
            <div class="flex items-center gap-2">
                <span class="px-2 py-0.5 text-[11px] rounded bg-slate-50 border border-slate-200">
                    #{{ $article->id }}
                </span>
                <code class="px-2 py-0.5 text-[11px] rounded bg-slate-50 border border-slate-200">
                    /{{ $article->slug }}
                </code>
            </div>
            <div class="flex items-center gap-1.5 font-sans">
                <span class="font-medium text-slate-800 opacity-80">{{ $article->author->name }}</span>
                <span>·</span>
                <span title="{{ $article->created_at->format('Y-m-d H:i:s') }}">
                    {{ $article->created_at->format('l, F j, Y') }}
                </span>
            </div>
        </div>

        {{-- Core Content Area --}}
        <article class="space-y-4">
            <h1 class="text-3xl font-bold tracking-tight text-slate-900 leading-tight">
                {{ $article->title }}
            </h1>
            
            <p class="text-base leading-relaxed text-slate-700 whitespace-pre-line">
                {{ $article->content }}
            </p>
        </article>

        {{-- Post comment form --}}
        <div class="mt-8 pt-6 border-t border-slate-100">
            <h3 class="text-lg font-semibold text-slate-800 mb-4">Comments ({{ $article->comments->count() }})</h3>
            
            <form action="{{ route('comments.store') }}" method="POST" class="space-y-3 mb-6">
                @csrf
                <input type="hidden" name="commentable_id" value="{{ $article->id }}">
                <input type="hidden" name="commentable_type" value="{{ App\Models\Article::class }}">
                
                <textarea name="body" rows="3" 
                    class="w-full rounded-lg border border-slate-200 p-3 text-sm focus:border-slate-400 focus:ring-0 text-slate-700 placeholder-slate-400"
                    placeholder="Write a comment...">{{ old('body') }}</textarea>
                
                @error('body') 
                    <p class="text-xs font-medium text-red-500 mt-1">{{ $message }}</p> 
                @enderror
                
                <div class="flex justify-end">
                    <x-button type="submit" variant="secondary">Add comment</x-button>
                </div>
            </form>

            {{-- List comments --}}
            <div class="space-y-4">
                @forelse ($article->comments as $comment)
                    <div class="p-4 rounded-lg bg-slate-50 border border-slate-100 flex justify-between items-start gap-4">
                        <div class="space-y-1">
                            <div class="flex items-center gap-2 text-xs">
                                <span class="font-semibold text-slate-800">{{ $comment->author->name }}</span>
                                <span class="text-slate-400">·</span>
                                <span class="text-slate-400">{{ $comment->created_at->diffForHumans() }}</span>
                            </div>
                            <p class="text-sm text-slate-600 leading-relaxed">{{ $comment->body }}</p>
                        </div>

                        {{-- Only show delete button if the logged user owns the comment --}}
                        <form action="{{ route('comments.destroy', $comment) }}" method="POST" onsubmit="return confirm('Delete comment?')">
                            @csrf
                            @method('DELETE')
                            <button type="submit" class="text-xs text-red-500 hover:text-red-700 font-medium transition-colors">
                                Delete
                            </button>
                        </form>
                    </div>
                @empty
                    <p class="text-sm text-slate-400 italic text-center py-4">No comments yet. Be the first to start the conversation!</p>
                @endforelse
            </div>
        </div>

        {{-- Advanced Backend Practice Anchor (Optional UI placeholder for later) --}}
        <div class="mt-8 pt-6 border-t border-slate-100 flex items-center justify-between text-xs text-slate-400">
            <span>System Timestamp: {{ $article->created_at->format('Y-m-d H:i:s A') }}</span>
        </div>

    </div>

@endsection