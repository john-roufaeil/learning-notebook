<!-- @props([
    'href' => null,
    'variant' => 'primary',
    'type' => 'button'
])

@php
$base = 'rounded p-2 text-sm font-medium transition-colors cursor-pointer text-center';

$variants = [
    'primary' => 'bg-blue-600 text-white hover:bg-blue-600/90',
    'outline' => 'border border-blue-600 text-blue-600 hover:bg-blue-600/90 hover:text-white',
    'danger'  => 'text-red-600 hover:underline',
    'ghost'   => 'text-blue-600 hover:underline',
];

$classes = $base . ' ' . $variants[$variant];
@endphp

@if ($href)
    <a href="{{ $href }}" {{ $attributes->merge(['class' => $classes]) }}>
        {{ $slot }}
    </a>
@else
    <button type="{{ $type }}" {{ $attributes->merge(['class' => $classes]) }}>
        {{ $slot }}
    </button>
@endif -->
{{-- resources/views/components/button.blade.php --}}
@props([
    'variant' => 'primary', 
    'tag' => 'button', 
    'href' => null,
    'type' => 'submit'
])

@php
    // Map your custom variant names to their respective CSS class names
    $variantClass = match($variant) {
        'secondary' => 'btn-secondary',
        'ghost'     => 'btn-ghost',
        'danger'    => 'btn-danger',
        'success'   => 'btn-success',
        default     => 'btn-primary',
    };
@endphp

@if($tag === 'a' || $href)
    <a href="{{ $href }}" {{ $attributes->merge(['class' => "btn $variantClass"]) }}>
        {{ $slot }}
    </a>
@else
    <button type="{{ $type }}" {{ $attributes->merge(['class' => "btn $variantClass"]) }}>
        {{ $slot }}
    </button>
@endif