@props([
'variant' => 'primary',
'tag' => 'button',
'href' => null,
'type' => 'submit'
])

@php
// Map your custom variant names to their respective CSS class names
$variantClass = match($variant) {
'secondary' => 'bg-white border border-gray-200 text-gray-700 hover:bg-gray-50',
'ghost' => 'text-gray-600 hover:text-gray-900 hover:bg-gray-100',
'danger' => 'bg-red-600 text-white hover:bg-red-700',
'success' => 'bg-green-600 text-white hover:bg-green-700',
default => 'bg-gray-900 text-white hover:bg-gray-800',
};
@endphp

@if($tag === 'a' || $href)
<a href="{{ $href }}" {{ $attributes->merge(['class' => "inline-flex items-center px-4 py-2 rounded-lg text-sm font-medium transition $variantClass"]) }}>
    {{ $slot }}
</a>
@else
<button type="{{ $type }}" {{ $attributes->merge(['class' => "inline-flex items-center px-4 py-2 rounded-lg text-sm font-medium transition $variantClass"]) }}>
    {{ $slot }}
</button>
@endif