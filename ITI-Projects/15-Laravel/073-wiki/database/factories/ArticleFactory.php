<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use \App\Models\User;

class ArticleFactory extends Factory
{
    public function definition(): array
    {
        return [
            'title' => fake()->words(3, true),
            'content' => fake()->paragraphs(8, true),
            'author_id' => User::inRandomOrder()->first()?->id ?? User::factory(),
        ];
    }
}
