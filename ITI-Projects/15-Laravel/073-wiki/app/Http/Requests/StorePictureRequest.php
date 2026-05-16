<?php

namespace App\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class StorePictureRequest extends FormRequest
{
    public function authorize(): bool
    {
        return true;
    }

    public function rules(): array
    {
        return [
            'title' => ['required', 'string', 'unique:pictures,title', 'min:3'],
            'description' => ['required', 'string', 'min:10'],
            'image' => ['required', 'image', 'mimes:jpeg,png,jpg', 'max:2048']
        ];
    }
}
