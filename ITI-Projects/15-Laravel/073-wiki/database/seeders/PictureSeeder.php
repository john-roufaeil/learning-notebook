<?php

namespace Database\Seeders;

use App\Models\Picture;
use Illuminate\Database\Seeder;

class PictureSeeder extends Seeder
{
  public function run(): void
  {
    Picture::factory(100)->create();
  }
}
