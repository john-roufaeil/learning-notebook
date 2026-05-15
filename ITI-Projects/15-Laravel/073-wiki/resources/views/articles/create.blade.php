@extends('layouts.master')

@section('content')
    <div class="max-w-2xl mx-auto mt-6">
        <h2 class="text-2xl font-bold tracking-tight text-slate-900 mb-6">New Article</h2>
        
        <form action="{{ route('articles.store') }}" method="POST" 
              class="card p-6 rounded-xl border border-slate-200 bg-white shadow-sm space-y-5">
            @csrf

            {{-- Title Field --}}
            <div class="flex flex-col gap-1.5">
                <label class="text-sm font-semibold text-slate-700">Title</label>
                <input type="text" name="title" value="{{ old('title') }}" placeholder="Give your page a descriptive title..."
                       class="w-full px-3 py-2 border border-slate-200 rounded-lg text-sm text-slate-900 focus:outline-none focus:ring-2 focus:ring-blue-600/20 focus:border-blue-600 transition-shadow">
                @error('title')
                    <p class="text-xs font-medium text-red-600 mt-1">{{ $message }}</p>
                @enderror
            </div>

            {{-- Content Field --}}
            <div class="flex flex-col gap-1.5">
                <label class="text-sm font-semibold text-slate-700">Content</label>
                <textarea name="content" rows="6" placeholder="Write your markdown or text documentation body..."
                          class="w-full px-3 py-2 border border-slate-200 rounded-lg text-sm text-slate-900 focus:outline-none focus:ring-2 focus:ring-blue-600/20 focus:border-blue-600 transition-shadow resize-y">{{ old('content') }}</textarea>
                @error('content')
                    <p class="text-xs font-medium text-red-600 mt-1">{{ $message }}</p>
                @enderror
            </div>

            {{-- Author Selection Field --}}
            <div class="flex flex-col gap-1.5">
                <label class="text-sm font-semibold text-slate-700">Article Author</label>
                <div class="relative">
                    <select name="author_id" 
                            class="w-full px-3 py-2 border border-slate-200 rounded-lg text-sm text-slate-900 bg-white focus:outline-none focus:ring-2 focus:ring-blue-600/20 focus:border-blue-600 transition-shadow appearance-none">
                        <option value="" disabled {{ old('author_id') ? '' : 'selected' }}>Select assigned author...</option>
                        @foreach ($users as $id => $name)
                            <option value="{{ $id }}" {{ old('author_id') == $id ? 'selected' : '' }}>{{ $name }}</option>
                        @endforeach
                    </select>
                </div>
                @error('author_id')
                    <p class="text-xs font-medium text-red-600 mt-1">{{ $message }}</p>
                @enderror
            </div>

            {{-- Form Control Buttons --}}
            <div class="flex items-center gap-3 pt-2 border-t border-slate-100">
                <x-button type="submit">Create Article</x-button>
                <x-button variant="secondary" href="{{ route('articles.index') }}" tag="a">Cancel</x-button>
            </div>
        </form>
    </div>
@endsection