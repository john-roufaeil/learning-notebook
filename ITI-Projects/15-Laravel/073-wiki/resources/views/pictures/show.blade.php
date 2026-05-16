@extends('layouts.app')

@section('content')
<div class="grid grid-cols-1 lg:grid-cols-3 gap-8 items-start">

  <!-- Left: Content Column -->
  <div class="lg:col-span-2 space-y-4">
    <div class="bg-white border border-slate-200 rounded-xl overflow-hidden shadow-sm">
      <img src="{{ asset('storage/' . $picture->image_path) }}" alt="{{ $picture->title }}" class="w-full object-contain bg-slate-900 max-h-[500px]">
      <div class="p-6 space-y-3">
        <div>
          <h1 class="text-2xl font-bold tracking-tight text-slate-900">{{ $picture->title }}</h1>
          <p class="text-xs text-slate-400 mt-0.5">By {{ $picture->artist->name ?? 'Unknown Artist' }} • {{ $picture->created_at->format('Y-m-d') }}</p>
        </div>
        @if($picture->description)
        <p class="text-sm text-slate-600 leading-relaxed pt-3 border-t border-slate-100">{{ $picture->description }}</p>
        @endif
      </div>
    </div>
  </div>

  <!-- Right: Comments Discussion Column -->
  <div class="space-y-6">
    <h2 class="text-lg font-bold text-slate-900">Discussion</h2>

    <!-- Comment Submission Form -->
    <form action="{{ route('comments.store') }}" method="POST" class="space-y-3">
      @csrf
      <input type="hidden" name="commentable_id" value="{{ $picture->id }}">
      <input type="hidden" name="commentable_type" value="{{ get_class($picture) }}">

      <textarea name="body" rows="3" placeholder="Add a comment on this picture..." required
        class="w-full text-sm p-3 border @error('body') border-red-500 @else border-slate-200 @enderror rounded-lg focus:outline-none">{{ old('body') }}</textarea>
      @error('body') <p class="text-xs text-red-600 mt-1" style="color: #dc2626;">{{ $message }}</p> @enderror

      <div class="flex justify-end">
        <button type="submit" class="btn btn-primary" style="padding:0.35rem 0.8rem; font-size:0.8rem;">
          Post Comment
        </button>
      </div>
    </form>

    <!-- Comments List -->
    <div class="space-y-3">
      @forelse($picture->comments as $comment)
      <div class="bg-white p-4 rounded-xl border border-slate-200 shadow-sm space-y-2">
        <div class="flex items-center justify-between">
          <div>
            <p class="text-xs font-semibold text-slate-800">{{ $comment->author->name ?? 'System User' }}</p>
            <p class="text-[10px] text-slate-400">{{ $comment->created_at->format('Y-m-d') }}</p>
          </div>

          <!-- Delete Form Context Bypassing Auth -->
          <form action="{{ route('comments.destroy', $comment) }}" method="POST" onsubmit="return confirm('Remove this comment?');">
            @csrf
            @method('DELETE')
            <button type="submit" class="btn btn-danger" style="padding:0.2rem 0.5rem; font-size:0.7rem;">
              Delete
            </button>
          </form>
        </div>
        <p class="text-sm text-slate-600 leading-snug">{{ $comment->body }}</p>
      </div>
      @empty
      <p class="text-xs text-slate-400 text-center py-6">Be the first to share your thoughts on this display piece!</p>
      @endforelse
    </div>
  </div>
</div>
@endsection