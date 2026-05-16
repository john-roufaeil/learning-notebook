<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Relations\HasMany;
use Illuminate\Foundation\Auth\User as Authenticatable;
use Laravel\Sanctum\HasApiTokens;

class User extends Authenticatable
{
    use HasApiTokens, HasFactory;

    protected $guarded = ['id'];
    protected $hidden = ['password'];

    public function articles(): HasMany
    {
        return $this->hasMany(Article::class, 'author_id');
    }
}
