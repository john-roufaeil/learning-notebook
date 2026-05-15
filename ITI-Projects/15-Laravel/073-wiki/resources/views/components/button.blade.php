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