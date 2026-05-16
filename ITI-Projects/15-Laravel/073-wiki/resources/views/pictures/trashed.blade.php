@extends('layouts.app')

@section('content')
<div class="space-y-6">
  <div class="flex items-center justify-between">
    <div>
      <h1 class="text-2xl font-bold tracking-tight text-slate-900">Trashed Artworks</h1>
      <p class="text-xs text-slate-500">Manage soft-deleted items or restore them back to the live show grid.</p>
    </div>

    <div class="flex gap-2">
      <x-button href="{{ route('pictures.index') }}" variant="secondary" tag="a">
        Back to Active Pictures
      </x-button>
      <form action="{{ route('pictures.restore-all') }}" method="POST">
        @csrf
        <x-button type="submit" variant="secondary" class="py-1.5">
          Restore All
        </x-button>
      </form>

      <form action="{{ route('pictures.force-delete-all') }}" method="POST"
        onsubmit="return confirm('CRITICAL WARNING: This will permanently delete EVERY picture in the trash. This action cannot be reversed. Proceed?')">
        @csrf
        @method('DELETE')
        <x-button type="submit" variant="danger" class="py-1.5">
          Empty Trash
        </x-button>
      </form>
    </div>
    <!-- <div class="flex gap-2">
      <form action="{{ route('pictures.restore-all') }}" method="POST">
        @csrf
        <button type="submit" class="btn btn-secondary" style="padding:0.35rem 0.8rem; font-size:0.8rem;">Restore All</button>
      </form>
      <form action="{{ route('pictures.force-delete-all') }}" method="POST" onsubmit="return confirm('Permanently wipe the entire trash can? This cannot be undone!');">
        @csrf
        @method('DELETE')
        <button type="submit" class="btn btn-danger" style="padding:0.35rem 0.8rem; font-size:0.8rem;">Empty Trash</button>
      </form>
    </div> -->
  </div>

  <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
    @forelse($trashedPictures as $picture)
    <div class="bg-white border border-slate-200 rounded-xl overflow-hidden shadow-sm flex flex-col opacity-75">
      <div class="p-5 flex flex-col flex-grow justify-between space-y-4">
        <div>
          <h2 class="font-semibold text-lg text-slate-700 leading-tight">{{ $picture->title }}</h2>
          <p class="text-xs text-slate-400 mt-1">Deleted {{ $picture->deleted_at }}</p>
        </div>

        <div class="flex items-center justify-end gap-2 pt-2 border-t border-slate-100">
          <form action="{{ route('pictures.restore', $picture) }}" method="POST">
            @csrf
            @method('PATCH')
            <button type="submit" class="btn btn-secondary" style="padding:0.3rem 0.7rem; font-size:0.75rem;">
              Restore
            </button>
          </form>
          <form action="{{ route('pictures.force-delete', $picture) }}" method="POST" onsubmit="return confirm('Permanently purge this image from the server storage?');">
            @csrf
            @method('DELETE')
            <button type="submit" class="btn btn-danger" style="padding:0.3rem 0.7rem; font-size:0.75rem;">
              Delete Forever
            </button>
          </form>
        </div>
      </div>
    </div>
    @empty
    <div class="col-span-full text-center py-12 border border-dashed border-slate-200 rounded-xl">
      <p class="text-sm text-slate-400">The trash bin is currently empty.</p>
    </div>
    @endforelse
  </div>

  <div class="pt-4">
    {{ $trashedPictures->links() }}
  </div>
</div>
@endsection