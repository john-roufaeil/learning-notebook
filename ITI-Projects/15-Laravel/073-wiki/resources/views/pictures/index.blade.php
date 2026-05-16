@extends('layouts.app')

@section('content')
<div class="space-y-6">
  <div class="flex items-center justify-between">
    <div>
      <h1 class="text-2xl font-bold tracking-tight">Gallery Showcase</h1>
      <p class="text-xs text-slate-500">A shared curation of fine art, illustrations, and digital photography.</p>
    </div>
    <div class="flex gap-2">
      <x-button href="{{ route('pictures.trashed') }}" variant="secondary">View Trash</x-button>
      <x-button href="{{ route('pictures.create') }}" variant="primary">Upload Image</x-button>
    </div>
  </div>

  <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
    @forelse($pictures as $picture)
    <div class="bg-white border border-slate-200 rounded-xl overflow-hidden shadow-sm flex flex-col">
      <img src="{{ asset('storage/' . $picture->image_path) }}" alt="{{ $picture->title }}" class="w-full h-64 object-cover bg-slate-100">
      <div class="p-5 flex flex-col grow justify-between space-y-4">
        <div class="space-y-3">
          {{-- Top Header Row: Title & Artist --}}
          <div>
            <h2 class="font-semibold text-lg text-slate-900 leading-tight">
              {{ $picture->title }}
            </h2>
            <p class="text-xs text-slate-500 mt-0.5">By {{ $picture->artist->name ?? 'Unknown Artist' }}</p>
          </div>

          {{-- Metadata Badge Row: ID, Slug & Created At --}}
          <div class="flex flex-wrap items-center gap-2 text-[11px] font-mono text-slate-400">
            {{-- Database ID Badge --}}
            <span class="px-1.5 py-0.5 rounded bg-slate-100 text-slate-600 font-bold border border-slate-200">
              #{{ $picture->id }}
            </span>

            {{-- Slug Wrapper --}}
            <span class="px-2 py-0.5 rounded bg-slate-50 border border-slate-200 truncate max-w-45" title="Slug Reference: {{ $picture->slug }}">
              /{{ $picture->slug }}
            </span>

            {{-- Date Stamps --}}
            <span class="flex items-center gap-1 font-sans">
              • {{ $picture->created_at->format('M d, Y') }}
              <span class="text-slate-300">({{ $picture->created_at->diffForHumans() }})</span>
            </span>
          </div>

          {{-- Main Body Section: Description Text Content --}}
          <p class="text-sm leading-relaxed text-slate-600 line-clamp-3">
            {{ $picture->description ?? 'No historical summary or description provided for this artwork asset.' }}
          </p>
        </div>
        <div class="flex items-center justify-end gap-2 pt-3 border-t border-slate-100">
          <x-button href="{{ route('pictures.show', $picture) }}" variant="secondary" tag="a">View</x-button>
          <x-button href="{{ route('pictures.edit', $picture) }}" variant="secondary" tag="a">Edit</x-button>
          </a>
          <form action="{{ route('pictures.destroy', $picture) }}" method="POST" onsubmit="return confirm('Move this piece to the trash?');">
            @csrf
            @method('DELETE')
            <x-button variant="danger" type="submit">Delete</x-button>
          </form>
        </div>
      </div>
    </div>
    @empty
    <div class="col-span-full text-center py-12 border border-dashed border-slate-200 rounded-xl">
      <p class="text-sm text-slate-400">No showcase uploads discovered in the database gallery.</p>
    </div>
    @endforelse
  </div>

  <div class="pt-4">
    {{ $pictures->links() }}
  </div>
</div>
@endsection