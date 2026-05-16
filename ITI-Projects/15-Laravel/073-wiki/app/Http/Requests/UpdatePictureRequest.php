<?php

namespace App\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class UpdatePictureRequest extends FormRequest {
    public function authorize(): bool {
        return true;
    }

    public function rules(): array {
        return [
            'title'=> ['required', 'string', 'unique:pictures,title,'.$this->route('picture')->id, 'min:3'],
            'description'=> ['required', 'string', 'min:10'],
            'image'=> ['sometimes', 'image', 'mimes:jpeg,png,jpg', 'max:2048']
        ];
    }
}
