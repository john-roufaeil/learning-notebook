<!DOCTYPE html>
<html lang="{{ str_replace('_', '-', app()->getLocale()) }}">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="csrf-token" content="{{ csrf_token() }}">

    <title>{{ $title ?? config('app.name', 'Wiki') }}</title>

    <!-- Fonts -->
    <link rel="preconnect" href="https://fonts.bunny.net">
    <link href="https://fonts.bunny.net/css?family=figtree:400,500,600&display=swap" rel="stylesheet" />
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght=400;500;600&display=swap" rel="stylesheet">

    <!-- Scripts & Styles -->
    @vite(['resources/css/app.css', 'resources/js/app.js'])
</head>

<body class="font-sans antialiased bg-slate-50 text-slate-900 transition-all duration-400">
    <div class="min-h-screen">

        {{-- Main Application Navigation Layout --}}
        @if(view()->exists('layouts.navigation'))
        @include('layouts.navigation')
        @else
        <nav style="border-bottom: 1px solid var(--color-border); background: var(--color-surface);">
            <div class="container-app" style="display:flex; align-items:center; justify-content:space-between; height:56px;">
                <a href="/" style="font-weight:600; font-size:1rem; color:var(--color-text);" class="hover:bg-gray-200 px-2 py-1 rounded">
                    Wiki
                </a>
                <div style="display:flex; gap:1.5rem; align-items:center;">
                    <x-button href="{{ route('articles.index') }}" variant="ghost" class="px-0">Journal</x-button>
                    <x-button href="{{ route('pictures.index') }}" variant="ghost" class="px-0">Gallery</x-button>
                </div>
            </div>
        </nav>
        @endif

        {{-- Optional Page Header Component --}}
        @if (isset($header))
        <header class="bg-white shadow">
            <div class="max-w-7xl mx-auto py-6 px-4 sm:px-6 lg:px-8">
                {{ $header }}
            </div>
        </header>
        @endif

        {{-- Global Page Mount Point --}}
        <main class="container-app max-w-7xl mx-auto px-4 sm:px-6 lg:px-8" style="padding-top:2.5rem; padding-bottom:4rem;">

            {{-- Flash Session Success Messages --}}
            @if(session('success'))
            <div style="background:#f0fdf4; border: 1px solid #bbf7d0; color:#16a34a; padding: 0.75rem 1rem; border-radius: 0.5rem; margin-bottom:1.5rem; font-size: 0.875rem;">
                {{ session('success') }}
            </div>
            @endif

            {{-- Compatibility for both component slots and traditional inheritance templates --}}
            @if(isset($slot))
            {{ $slot }}
            @else
            @yield('content')
            @endif

        </main>
    </div>
</body>

</html>