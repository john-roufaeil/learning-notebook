<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>{{ $title ?? 'Wiki' }}</title>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600&display=swap" rel="stylesheet">
  @vite(['resources/css/app.css', 'resources/js/app.js'])
</head>
<body class="*:transition-all *:duration-400 bg-slate-50 text-slate-900">

  <nav style="border-bottom: 1px solid var(--color-border); background: var(--color-surface);">
    <div class="container-app" style="display:flex; align-items:center; justify-content:space-between; height:56px;">
      <a href="/" style="font-weight:600; font-size:1rem; color:var(--color-text);" class="hover:bg-gray-200 px-2 py-1 rounded">
        Wiki
      </a>
      <div style="display:flex; gap:1.5rem; align-items:center;">
        <a href="{{ route('articles.index') }}" style="font-size:0.875rem; color:var(--color-muted);" class="hover:text-slate-900 font-medium">Journal</a>
        <a href="{{ route('pictures.index') }}" style="font-size:0.875rem; color:var(--color-muted);" class="hover:text-slate-900 font-medium">Gallery</a>
      </div>
    </div>
  </nav>

  {{-- Global Page Mount Point --}}
  <main class="container-app" style="padding-top:2.5rem; padding-bottom:4rem;">
    @if(session('success'))
      <div style="background:#f0fdf4; border: 1px solid #bbf7d0; color:#16a34a; padding: 0.75rem 1rem; border-radius: 0.5rem; margin-bottom:1.5rem; font-size: 0.875rem;">
        {{ session('success') }}
      </div>
    @endif

    @yield('content')
  </main>

</body>
</html>