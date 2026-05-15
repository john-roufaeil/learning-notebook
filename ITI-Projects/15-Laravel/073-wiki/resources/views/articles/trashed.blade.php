@extends('layouts.master')

@section('content')

    {{-- Navigation & Header Section --}}
    <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-4 mb-6">
        <div>
            <h2 class="text-2xl font-bold tracking-tight text-slate-900">Trash Bin</h2>
            <p class="text-sm text-slate-500 mt-0.5">Manage and restore soft-deleted wiki articles.</p>
        </div>
        <div class="flex gap-2">
          <x-button href="{{ route('articles.index') }}" variant="secondary" tag="a">
              &larr; Back to Active Articles
          </x-button>
          <form action="{{ route('articles.restore-all') }}" method="POST">
              @csrf
              <x-button type="submit" variant="secondary" class="py-1.5">
                Restore All
              </x-button>
          </form>

          <form action="{{ route('articles.force-delete-all') }}" method="POST"
                onsubmit="return confirm('CRITICAL WARNING: This will permanently delete EVERY article in the trash. This action cannot be reversed. Proceed?')">
              @csrf
              @method('DELETE')
              <x-button type="submit" variant="danger" class="py-1.5">
                  Empty Trash
              </x-button>
          </form>
        </div>
    </div>

    {{-- Session Feedback Alerts --}}
    @if(session('success'))
        <div class="mb-6 p-4 rounded-lg bg-emerald-50 border border-emerald-200 text-sm font-medium text-emerald-800">
            {{ session('success') }}
        </div>
    @endif

    {{-- Empty State --}}
    @if($trashedArticles->isEmpty())
        <div class="flex flex-col items-center justify-center p-12 rounded-xl border border-dashed border-slate-300 bg-slate-50 text-center">
            <h3 class="text-base font-semibold text-slate-800">Trash is empty</h3>
            <p class="text-sm text-slate-500 max-w-sm mt-1">There are no soft-deleted articles in the system database right now.</p>
        </div>
    @else
        {{-- Trashed Articles Grid --}}
        <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
        @foreach ($trashedArticles as $article)
            <div class="card flex flex-col justify-between p-5 rounded-xl border border-slate-200 bg-white opacity-90 hover:opacity-100 transition-opacity duration-200 shadow-sm">
                
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
                        <div class="flex items-center gap-1.5 font-sans text-red-600 font-medium">
                            <span>Deleted:</span>
                            <span>{{ $article->deleted_at->format('Y-m-d') }}</span>
                        </div>
                    </div>

                    {{-- Title & Content Preview --}}
                    <div class="space-y-1">
                        <h3 class="text-lg font-semibold text-slate-700 line-through decoration-slate-400">
                            {{ $article->title }}
                        </h3>
                        <p class="text-sm leading-relaxed text-slate-400 line-clamp-2">
                            {{ $article->content }}
                        </p>
                    </div>
                </div>

                {{-- Action Controls --}}
                <div class="flex items-center justify-between pt-4 mt-4 border-t border-slate-100">
                    
                    {{-- Restore Trigger Form --}}
                    <form action="{{ route('articles.restore', $article) }}" method="POST">
                        @csrf
                        @method('PATCH')
                        <x-button type="submit" variant="secondary">
                            Restore
                        </x-button>
                    </form>
                    
                    {{-- Permanent Force Delete Trigger Form --}}
                    <form action="{{ route('articles.force-delete', $article) }}" method="POST"
                          onsubmit="return confirm('WARNING: This will permanently delete this article from the database. This action cannot be undone. Proceed?')">
                        @csrf
                        @method('DELETE')
                        <x-button variant="danger" type="submit">
                            Delete
                        </x-button>
                    </form>

                </div>

            </div>
        @endforeach
        </div>

        {{-- Pagination Footer --}}
        @if($trashedArticles->hasPages())
            <div class="flex flex-col sm:flex-row items-center justify-between gap-4 mt-8 pt-4 border-t border-slate-200 text-sm text-slate-500">
                <div>
                    Showing 
                    <span class="font-medium text-slate-900">{{ $trashedArticles->firstItem() }}</span> 
                    to 
                    <span class="font-medium text-slate-900">{{ $trashedArticles->lastItem() }}</span> 
                    of 
                    <span class="font-medium text-slate-900">{{ $trashedArticles->total() }}</span> 
                    deleted records
                </div>
                <div class="flex items-center gap-1">
                    @if ($trashedArticles->onFirstPage())
                        <span class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 opacity-40 cursor-not-allowed bg-slate-50">Prev</span>
                    @else
                        <a href="{{ $trashedArticles->previousPageUrl() }}" class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 bg-white text-slate-700 hover:bg-slate-50 transition-colors">Prev</a>
                    @endif

                    <span class="px-3 py-1.5 text-xs font-semibold rounded-lg bg-slate-200 text-slate-800">
                        {{ $trashedArticles->currentPage() }} / {{ $trashedArticles->lastPage() }}
                    </span>

                    @if ($trashedArticles->hasMorePages())
                        <a href="{{ $trashedArticles->nextPageUrl() }}" class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 bg-white text-slate-700 hover:bg-slate-50 transition-colors">Next</a>
                    @else
                        <span class="px-3 py-1.5 text-xs font-medium rounded-lg border border-slate-200 opacity-40 cursor-not-allowed bg-slate-50">Next</span>
                    @endif
                </div>
            </div>
        @endif
    @endif

@endsection