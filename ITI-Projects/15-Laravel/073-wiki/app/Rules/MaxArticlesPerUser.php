<?php

namespace App\Rules;

use Closure;
use App\Models\Article;
use Illuminate\Contracts\Validation\ValidationRule;

class MaxArticlesPerUser implements ValidationRule {
    public function validate(string $attribute, mixed $value, Closure $fail): void {
        $count = Article::where('author_id', $value)->count();
        if ($count >= 50) {
            $fail('This author has reached the maximum number of articles to create.');
        }
    }
}
