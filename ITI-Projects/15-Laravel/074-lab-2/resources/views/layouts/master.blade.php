<!DOCTYPE html>
<html>
<head>
    @vite(['resources/css/app.css', 'resources/js/app.js'])
    <meta charset="UTF-8">
    <title>ITI Blogs</title>
</head>
<body>
    <header>
        <h1>ITI Blogs</h1>
        <br />
    </header>
    <main>
        @yield('content')
    </main>
</body>
</html>