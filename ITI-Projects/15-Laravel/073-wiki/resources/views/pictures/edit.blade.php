@extends('layouts.app')

@section('content')
<div class="max-w-lg mx-auto bg-white p-6 rounded-xl border border-slate-200 shadow-sm space-y-6">
  <div class="flex items-center justify-between">
    <div>
      <h1 class="text-xl font-bold text-slate-900">Edit Artwork Details</h1>
      <p class="text-xs text-slate-500">Modify information properties associated with this gallery entity.</p>
    </div>
    <form action="{{ route('pictures.destroy', $picture) }}" method="POST" onsubmit="return confirm('Move this piece to the trash?');">
      @csrf
      @method('DELETE')
      <button type="submit" class="text-xs font-medium text-red-600 hover:underline">Delete Picture</button>
    </form>
  </div>

  {{-- Added enctype="multipart/form-data" to support file handling payloads --}}
  <form action="{{ route('pictures.update', $picture) }}" method="POST" enctype="multipart/form-data" class="space-y-4">
    @csrf
    @method('PUT')

    {{-- Current Image Asset Preview Thumbnail --}}
    <div>
      <label class="block text-xs font-medium text-slate-700 mb-2">Current Asset File</label>
      <div class="flex items-center gap-4 p-3 border border-slate-100 bg-slate-50 rounded-lg">
        <img src="{{ asset('storage/' . $picture->image_path) }}" alt="Preview" class="w-16 h-16 object-cover rounded-md bg-slate-200 border border-slate-200 shadow-sm">
        <div class="truncate text-xs">
          <p class="font-medium text-slate-700 truncate max-w-70" title="{{ $picture->image_path }}">
            {{ basename($picture->image_path) }}
          </p>
          <p class="text-slate-400 mt-0.5 font-mono">ID: #{{ $picture->id }}</p>
        </div>
      </div>
    </div>

    {{-- New Replacement File Input Field --}}
    <div>
      <label class="block text-xs font-medium text-slate-700 mb-1">Replace Image File (Optional)</label>
      <input type="file" name="image" accept="image/*"
        class="w-full text-xs p-2 border border-slate-200 rounded-lg bg-white file:mr-3 file:py-1 file:px-2.5 file:rounded-md file:border-0 file:text-xs file:font-medium file:bg-slate-900 file:text-white hover:file:bg-slate-800 cursor-pointer">
      <p class="text-[10px] text-slate-400 mt-1">Leave blank to retain the current image.</p>
      @error('image') <p class="text-xs text-red-600 mt-1">{{ $message }}</p> @enderror
    </div>

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
      <x-button href="{{ route('pictures.show', $picture) }}" variant="secondary">Cancel</x-button>
      <x-button type="submit" variant="primary">Save Changes</x-button>
    </div>
  </form>
</div>
@endsection