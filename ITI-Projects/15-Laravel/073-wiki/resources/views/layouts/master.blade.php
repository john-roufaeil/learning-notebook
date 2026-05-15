<!DOCTYPE html>
<html>
<head>
    @vite(['resources/css/app.css', 'resources/js/app.js'])
    <meta charset="UTF-8">
    <title>ITI Blogs</title>
</head>
<body class="bg-slate-100">
    <header class="p-4">
        <h1 class="text-3xl text-center mb-4">ITI Blogs</h1>
    </header>
    <main class="p-4">
        @yield('content')
    </main>
</body>
</html>