<?php

namespace App\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class StoreCommentRequest extends FormRequest {
    public function authorize(): bool {
        return true;
    }

    public function rules(): array {
        return [
            'body' => ['required', 'min:3'],
            'commentable_id' => ['required', 'integer'],
            'commentable_type' => ['required', 'string']
    ];
    }
}
