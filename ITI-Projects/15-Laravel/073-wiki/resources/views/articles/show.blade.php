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

        {{-- Advanced Backend Practice Anchor (Optional UI placeholder for later) --}}
        <div class="mt-8 pt-6 border-t border-slate-100 flex items-center justify-between text-xs text-slate-400">
            <span>System Timestamp: {{ $article->created_at->format('Y-m-d H:i:s A') }}</span>
        </div>

    </div>

@endsection