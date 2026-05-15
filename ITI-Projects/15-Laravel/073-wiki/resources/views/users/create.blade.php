@extends('layouts.master')

@section('content')
    <div class="max-w-xl mx-auto mt-6">
        <h2 class="text-2xl font-bold tracking-tight text-slate-900 mb-6">Create New User</h2>
        
        <form action="{{ route('users.store') }}" method="POST" 
              class="card p-6 rounded-xl border border-slate-200 bg-white shadow-sm space-y-5">
            @csrf

            {{-- Name Field --}}
            <div class="flex flex-col gap-1.5">
                <label class="text-sm font-semibold text-slate-700">Name</label>
                <input type="text" name="name" value="{{ old('name') }}"
                       class="w-full px-3 py-2 border border-slate-200 rounded-lg text-sm text-slate-900 focus:outline-none focus:ring-2 focus:ring-blue-600/20 focus:border-blue-600 transition-shadow">
                @error('name')
                    <p class="text-xs font-medium text-red-600 mt-1">{{ $message }}</p>
                @enderror
            </div>

            {{-- Email Field --}}
            <div class="flex flex-col gap-1.5">
                <label class="text-sm font-semibold text-slate-700">Email Address</label>
                <input type="email" name="email" value="{{ old('email') }}"
                       class="w-full px-3 py-2 border border-slate-200 rounded-lg text-sm text-slate-900 focus:outline-none focus:ring-2 focus:ring-blue-600/20 focus:border-blue-600 transition-shadow">
                @error('email')
                    <p class="text-xs font-medium text-red-600 mt-1">{{ $message }}</p>
                @enderror
            </div>

            {{-- Password Field --}}
            <div class="flex flex-col gap-1.5">
                <label class="text-sm font-semibold text-slate-700">Password</label>
                <input type="password" name="password"
                       class="w-full px-3 py-2 border border-slate-200 rounded-lg text-sm text-slate-900 focus:outline-none focus:ring-2 focus:ring-blue-600/20 focus:border-blue-600 transition-shadow">
                @error('password')
                    <p class="text-xs font-medium text-red-600 mt-1">{{ $message }}</p>
                @enderror
            </div>

            {{-- Form Control Buttons --}}
            <div class="flex items-center gap-3 pt-2 border-t border-slate-100">
                <x-button type="submit">Create User</x-button>
                <x-button variant="secondary" href="{{ route('articles.index') }}" tag="a">Cancel</x-button>
            </div>
        </form>
    </div>
@endsection