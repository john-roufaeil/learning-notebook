@extends('layouts.app')

@section('content')
<div class="max-w-lg mx-auto bg-white p-6 rounded-xl border border-slate-200 shadow-sm space-y-6">
    <div>
        <h1 class="text-xl font-bold text-slate-900">Upload to Gallery</h1>
        <p class="text-xs text-slate-500">Publish a new piece of visual media containing localized artist credits.</p>
    </div>

    <form action="{{ route('pictures.store') }}" method="POST" enctype="multipart/form-data" class="space-y-4">
        @csrf
        <div>
            <label class="block text-xs font-medium text-slate-700 mb-1">Artwork Title</label>
            <input type="text" name="title" value="{{ old('title') }}" 
                class="w-full text-sm p-2.5 border @error('title') border-red-500 @else border-slate-200 @enderror rounded-lg focus:outline-none">
            @error('title') <p class="text-xs text-red-600 mt-1" style="color: #dc2626;">{{ $message }}</p> @enderror
        </div>

        <div>
            <label class="block text-xs font-medium text-slate-700 mb-1">Description / Context</label>
            <textarea name="description" rows="4"
                class="w-full text-sm p-2.5 border @error('description') border-red-500 @else border-slate-200 @enderror rounded-lg focus:outline-none">{{ old('description') }}</textarea>
            @error('description') <p class="text-xs text-red-600 mt-1" style="color: #dc2626;">{{ $message }}</p> @enderror
        </div>

        <div>
            <label class="block text-xs font-medium text-slate-700 mb-1">Select File (Image)</label>
            <input type="file" name="image" 
                class="w-full text-sm text-slate-500 file:mr-4 file:py-2 file:px-4 file:rounded-md file:border-0 file:text-xs file:font-semibold file:bg-slate-100 file:text-slate-700 hover:file:bg-slate-200">
            @error('image') <p class="text-xs text-red-600 mt-1" style="color: #dc2626;">{{ $message }}</p> @enderror
        </div>

        <div class="flex items-center justify-end gap-3 pt-2">
            <a href="{{ route('pictures.index') }}" class="text-xs font-medium text-slate-600 hover:text-slate-900">Cancel</a>
            <button type="submit" class="btn btn-primary" style="padding:0.4rem 1rem; font-size:0.8rem;">
                Publish Artwork
            </button>
        </div>
    </form>
</div>
@endsection