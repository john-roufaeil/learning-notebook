<x-app-layout>
    <x-slot name="header">
        <h2 class="font-semibold text-xl text-gray-800 leading-tight">
            {{ __('Dashboard') }}
        </h2>
    </x-slot>

    <div class="py-12">
        <div class="max-w-7xl mx-auto sm:px-6 lg:px-8 space-y-6">

            <div class="text-center space-y-2">
                <h1 class="text-3xl font-bold tracking-tight">Welcome to Wiki</h1>
                <p class="text-sm text-gray-500">Explore our shared journal of ideas or browse the creative gallery.</p>
            </div>

            <div class="flex justify-center gap-4">
                <a href="{{ route('articles.index') }}"
                    class="px-4 py-2 bg-gray-900 text-white rounded-lg text-sm font-medium hover:bg-gray-800">
                    View Journal
                </a>
                <a href="{{ route('pictures.index') }}"
                    class="px-4 py-2 bg-white border border-gray-200 text-gray-700 rounded-lg text-sm font-medium hover:bg-gray-50">
                    Open Gallery
                </a>
            </div>

        </div>
    </div>
</x-app-layout>