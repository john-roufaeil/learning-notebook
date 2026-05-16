@extends('layouts.app')

@section('content')
<div class="max-w-xl mx-auto space-y-8">
    <div class="text-center space-y-2">
        <h1 class="text-3xl font-bold tracking-tight">Welcome to Wiki</h1>
        <p class="text-sm text-slate-500">Explore our shared journal of ideas or browse the creative media gallery.</p>
    </div>

    <div class="flex justify-center gap-4">
        <a href="{{ route('articles.index') }}" class="px-4 py-2 bg-slate-900 text-white rounded-lg text-sm font-medium hover:bg-slate-800">
            View Journal
        </a>
        <a href="{{ route('pictures.index') }}" class="px-4 py-2 bg-white border border-slate-200 text-slate-700 rounded-lg text-sm font-medium hover:bg-slate-50">
            Open Gallery
        </a>
    </div>

    <hr class="border-slate-200">

    <!-- Seed Test User Form Component Area -->
    <div class="bg-white p-6 rounded-xl border border-slate-200 shadow-sm space-y-4">
        <h2 class="text-sm font-semibold text-slate-900">Create a Test User Account</h2>
        <p class="text-xs text-slate-500">Quickly add a dummy author/artist record to sign database relations during development workflows.</p>

        <form action="/users" method="POST" class="space-y-3">
            @csrf
            <div>
                <label class="block text-xs font-medium text-slate-600 mb-1">Full Name</label>
                <input type="text" name="name" placeholder="John Doe" required
                    class="w-full text-sm p-2 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-slate-900/10">
            </div>
            <div>
                <label class="block text-xs font-medium text-slate-600 mb-1">Email Address</label>
                <input type="email" name="email" placeholder="john@example.com" required
                    class="w-full text-sm p-2 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-slate-900/10">
            </div>
            <button type="submit" class="w-full py-2 bg-slate-100 hover:bg-slate-200 text-slate-800 font-medium text-xs rounded-lg transition">
                Save User Instance
            </button>
        </form>
    </div>
</div>
@endsection