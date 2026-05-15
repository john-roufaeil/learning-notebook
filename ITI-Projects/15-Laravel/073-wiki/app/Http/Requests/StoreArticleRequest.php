<?php

namespace App\Http\Requests;

use App\Rules\MaxArticlesPerUser;
use Illuminate\Foundation\Http\FormRequest;

class StoreArticleRequest extends FormRequest {
    public function authorize(): bool {
        return true;
    }

    public function rules(): array {
        return [
            'title'=> ['required', 'string', 'unique:articles,title', 'min:3'],
            'content'=> ['required', 'string', 'min:10'],
            'author_id'=> ['required', 'string', 'exists:users,id', new MaxArticlesPerUser]
        ];
    }
}
