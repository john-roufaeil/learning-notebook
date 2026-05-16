@extends('layouts.app')

@section('content')
<div class="max-w-lg mx-auto bg-white p-6 rounded-xl border border-slate-200 shadow-sm space-y-6">
    <div class="flex items-center justify-between">
        <div>
            <h1 class="text-xl font-bold text-slate-900">Edit Artwork Details</h1>
            <p class="text-xs text-slate-500">Modify information properties associated with this gallery entity.</p>
        </div>
        <form action="{{ route('pictures.destroy', $picture) }}" method="POST" onsubmit="return confirm('Permanently remove this picture?');">
            @csrf
            @method('DELETE')
            <button type="submit" class="text-xs font-medium text-red-600 hover:underline">Delete Picture</button>
        </form>
    </div>

    <form action="{{ route('pictures.update', $picture) }}" method="POST" class="space-y-4">
        @csrf
        @method('PUT')
        
        <div>
            <label class="block text-xs font-medium text-slate-700 mb-1">Artwork Title</label>
            <input type="text" name="title" value="{{ old('title', $picture->title) }}" required
                class="w-full text-sm p-2.5 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-slate-900/10">
            @error('title') <p class="text-xs text-red-600 mt-1">{{ $message }}</p> @enderror
        </div>

        <div>
            <label class="block text-xs font-medium text-slate-700 mb-1">Description / Context</label>
            <textarea name="description" rows="4"
                class="w-full text-sm p-2.5 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-slate-900/10">{{ old('description', $picture->description) }}</textarea>
            @error('description') <p class="text-xs text-red-600 mt-1">{{ $message }}</p> @enderror
        </div>

        <div class="flex items-center justify-end gap-3 pt-2">
            <a href="{{ route('pictures.show', $picture) }}" class="text-xs font-medium text-slate-600 hover:text-slate-900">Cancel</a>
            <button type="submit" class="px-4 py-2 bg-slate-900 text-white rounded-lg text-xs font-medium hover:bg-slate-800">
                Save Changes
            </button>
        </div>
    </form>
</div>
@endsection