@extends('layouts.app')

@section('content')
    <div class="max-w-2xl mx-auto mt-6">
        <h2 class="text-2xl font-bold tracking-tight text-slate-900 mb-2">Edit Article</h2>
        <p class="text-sm text-slate-500 mb-6">Modifying: <span class="font-semibold text-slate-700">"{{ $article->title }}"</span></p>
        
        <form action="{{ route('articles.update', $article) }}" method="POST" 
              class="card p-6 rounded-xl border border-slate-200 bg-white shadow-sm space-y-5">
            @csrf
            @method('PUT')

            {{-- Title Field --}}
            <div class="flex flex-col gap-1.5">
                <label class="text-sm font-semibold text-slate-700">Title</label>
                <input type="text" name="title" value="{{ old('title', $article->title) }}"
                       class="w-full px-3 py-2 border border-slate-200 rounded-lg text-sm text-slate-900 focus:outline-none focus:ring-2 focus:ring-blue-600/20 focus:border-blue-600 transition-shadow">
                @error('title')
                    <p class="text-xs font-medium text-red-600 mt-1">{{ $message }}</p>
                @enderror
            </div>

            {{-- Content Field --}}
            <div class="flex flex-col gap-1.5">
                <label class="text-sm font-semibold text-slate-700">Content</label>
                <textarea name="content" rows="6" 
                          class="w-full px-3 py-2 border border-slate-200 rounded-lg text-sm text-slate-900 focus:outline-none focus:ring-2 focus:ring-blue-600/20 focus:border-blue-600 transition-shadow resize-y">{{ old('content', $article->content) }}</textarea>
                @error('content')
                    <p class="text-xs font-medium text-red-600 mt-1">{{ $message }}</p>
                @enderror
            </div>

            {{-- Author Selection Field --}}
            <div class="flex flex-col gap-1.5">
                <label class="text-sm font-semibold text-slate-700">Article Author</label>
                <select name="author_id" 
                        class="w-full px-3 py-2 border border-slate-200 rounded-lg text-sm text-slate-900 bg-white focus:outline-none focus:ring-2 focus:ring-blue-600/20 focus:border-blue-600 transition-shadow">
                    @foreach ($users as $id => $name)
                        <option value="{{ $id }}" {{ old('author_id', $article->author_id) == $id ? 'selected' : '' }}>
                            {{ $name }}
                        </option>
                    @endforeach
                </select>
                @error('author_id')
                    <p class="text-xs font-medium text-red-600 mt-1">{{ $message }}</p>
                @enderror
            </div>

            {{-- Form Control Buttons --}}
            <div class="flex items-center gap-3 pt-2 border-t border-slate-100">
                <x-button type="submit">Update Article</x-button>
                <x-button variant="secondary" href="{{ route('articles.show', $article) }}" tag="a">Cancel</x-button>
            </div>
        </form>
    </div>
@endsection