<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use \App\Models\User;

class ArticleFactory extends Factory {
    public function definition(): array {
        return [
            'title' => fake()->sentence(),
            'content'=> fake()->paragraph(),
            'author_id'=> User::inRandomOrder()->first()?->id ?? User::factory(),
        ];
    }
}
