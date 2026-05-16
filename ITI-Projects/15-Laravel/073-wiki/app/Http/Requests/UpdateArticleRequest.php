<?php

namespace App\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class UpdateArticleRequest extends FormRequest {
    public function authorize(): bool {
        return true;
    }

    public function rules(): array {
        return [
            'title'=> ['required', 'string', 'unique:articles,title,'.$this->route('article')->id, 'min:3'],
            'content'=> ['required', 'string', 'min:10'],
            'author_id'=> ['required', 'string', 'exists:users,id']
        ];
    }
}
