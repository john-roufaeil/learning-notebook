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

    <!-- 2 Column Layout Grid -->
    <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
        @forelse($pictures as $picture)
            <div class="bg-white border border-slate-200 rounded-xl overflow-hidden shadow-sm flex flex-col">
                <img src="{{ asset('storage/' . $picture->image_path) }}" alt="{{ $picture->title }}" class="w-full h-64 object-cover bg-slate-100">
                <div class="p-5 flex flex-col grow justify-between space-y-4">
                    <div>
                        <h2 class="font-semibold text-lg text-slate-900 leading-tight">
                            {{ $picture->title }}
                        </h2>
                        <p class="text-xs text-slate-400 mt-1">By {{ $picture->artist->name ?? 'Unknown Artist' }}</p>
                    </div>
                    
                    <!-- Action Control Buttons -->
                    <div class="flex items-center justify-end gap-2 pt-2 border-t border-slate-100">
                        <a href="{{ route('pictures.show', $picture) }}" class="btn btn-secondary" style="padding:0.3rem 0.7rem; font-size:0.75rem;">
                            View
                        </a>
                        <a href="{{ route('pictures.edit', $picture) }}" class="btn btn-secondary" style="padding:0.3rem 0.7rem; font-size:0.75rem;">
                            Edit
                        </a>
                        <form action="{{ route('pictures.destroy', $picture) }}" method="POST" onsubmit="return confirm('Move this piece to the trash?');">
                            @csrf
                            @method('DELETE')
                            <button type="submit" class="btn btn-danger" style="padding:0.3rem 0.7rem; font-size:0.75rem;">
                                Delete
                            </button>
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

    <!-- Centered Unified Pagination Links Container -->
    <div class="pt-4">
        {{ $pictures->links() }}
    </div>
</div>
@endsection