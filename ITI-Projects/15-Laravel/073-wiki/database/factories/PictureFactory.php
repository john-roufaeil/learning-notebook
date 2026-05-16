<?php

namespace Database\Factories;

use App\Models\User;
use Illuminate\Database\Eloquent\Factories\Factory;

class PictureFactory extends Factory
{
    public function definition(): array
    {
        return [
            'title' => fake()->words(3, true),
            "image_path" => "pictures/placeholder.png",
            "description" => fake()->paragraph(),
            "artist_id" => User::inRandomOrder()->first()->id ?? User::factory()
        ];
    }
}
