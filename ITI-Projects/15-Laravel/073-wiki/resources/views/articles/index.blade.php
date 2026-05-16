@extends('layouts.app')

@section('content')

    {{-- Header Section --}}
    <div class="flex items-center justify-between mb-6">
        <h2 class="text-2xl font-bold tracking-tight text-slate-900">Articles</h2>
        <div class="flex gap-2">
            <x-button href="{{ route('articles.trashed') }}" variant="secondary">View trash</x-button>
            <x-button href="{{ route('users.create') }}" variant="secondary">New User</x-button>
            <x-button href="{{ route('articles.create') }}" variant="primary">New Article</x-button>
        </div>
    </div>

    {{-- Articles Grid --}}
    <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
    @foreach ($articles as $article)
        <div class="card flex flex-col justify-between p-5 rounded-xl border border-slate-200 bg-white shadow-sm hover:shadow-md transition-shadow duration-200">
            
            {{-- Card Body --}}
            <div class="space-y-3">
                {{-- Metadata Row --}}
                <div class="flex flex-wrap items-center justify-between gap-2 text-xs font-mono text-slate-500">
                    <div class="flex items-center gap-2">
                        <span class="opacity-60">#{{ $article->id }}</span>
                        <code class="px-2 py-0.5 text-[11px] rounded bg-slate-50 border border-slate-200">
                            /{{ str($article->slug)->limit(10, '...') }}
                        </code>
                    </div>
                    <div class="flex items-center gap-1.5 font-sans">
                        <span class="font-medium text-slate-800 opacity-80">{{ $article->author->name }}</span>
                        <span>·</span>
                        <span>{{ $article->created_at->format('Y-m-d') }}</span>
                    </div>
                </div>

                {{-- Title & Content --}}
                <div class="space-y-1">
                    <h3 class="text-lg font-semibold text-slate-900 leading-snug">
                        {{ $article->title }}
                    </h3>
                    <p class="text-sm leading-relaxed text-slate-500 line-clamp-2">
                        {{ $article->content }}
                    </p>
                </div>
            </div>

            {{-- Card Actions Footer --}}
            <div class="flex items-center justify-between pt-4 mt-4 border-t border-slate-100">
                <div class="flex gap-2">
                    <x-button href="{{ route('articles.show', $article) }}" variant="secondary" tag="a">View</x-button>
                    <x-button href="{{ route('articles.edit', $article) }}" variant="secondary" tag="a">Edit</x-button>
                </div>
                
                <form action="{{ route('articles.destroy', $article) }}" method="POST"
                      onsubmit="return confirm('Delete this article?')" class="inline">
                    @csrf
                    @method('DELETE')
                    <x-button variant="danger" type="submit">Delete</x-button>
                </form>
            </div>

        </div>
    @endforeach
    </div>

    {{-- Pagination Footer --}}
    @if($articles->hasPages())
    <div class="flex flex-col sm:flex-row items-center justify-between gap-4 mt-8 pt-4 border-t border-slate-200 text-sm text-slate-500">
        
        {{-- Left Side: Showing X of Y text --}}
        <div>
            Showing 
            <span class="font-medium text-slate-900">{{ $articles->firstItem() }}</span> 
            to 
            <span class="font-medium text-slate-900">{{ $articles->lastItem() }}</span> 
            of 
            <span class="font-medium text-slate-900">{{ $articles->total() }}</span> 
            results
        </div>

        {{-- Right Side: Clean Navigation Buttons --}}
        <div class="flex items-center gap-1">
            {{-- First Page Button --}}
            @if ($articles->onFirstPage())
                <span class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 opacity-40 cursor-not-allowed bg-slate-50">First</span>
            @else
                <a href="{{ $articles->url(1) }}" class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 bg-white text-slate-700 hover:bg-slate-50 transition-colors">First</a>
            @endif

            {{-- Previous Page Button --}}
            @if ($articles->onFirstPage())
                <span class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 opacity-40 cursor-not-allowed bg-slate-50">Prev</span>
            @else
                <a href="{{ $articles->previousPageUrl() }}" class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 bg-white text-slate-700 hover:bg-slate-50 transition-colors">Prev</a>
            @endif

            {{-- Compact Current Page Indicator --}}
            <span class="px-3 py-1.5 text-xs font-semibold rounded-lg bg-slate-200 text-slate-800">
                {{ $articles->currentPage() }} / {{ $articles->lastPage() }}
            </span>

            {{-- Next Page Button --}}
            @if ($articles->hasMorePages())
                <a href="{{ $articles->nextPageUrl() }}" class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 bg-white text-slate-700 hover:bg-slate-50 transition-colors">Next</a>
            @else
                <span class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 opacity-40 cursor-not-allowed bg-slate-50">Next</span>
            @endif

            {{-- Last Page Button --}}
            @if ($articles->hasMorePages())
                <a href="{{ $articles->url($articles->lastPage()) }}" class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 bg-white text-slate-700 hover:bg-slate-50 transition-colors">Last</a>
            @else
                <span class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 opacity-40 cursor-not-allowed bg-slate-50">Last</span>
            @endif
        </div>

    </div>
    @endif

@endsection