<?php

namespace App\Models;

use Cviebrock\EloquentSluggable\Sluggable;
use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Relations\BelongsTo;

class Post extends Model
{
    use HasFactory;
    use Sluggable;

    protected $fillable = ['title', 'content', 'author_id'];

    public function author(): BelongsTo
    {
        return $this->belongsTo(User::class, 'author_id');
    }

    public function sluggable(): array {
        return ['slug' => [
            'source' => 'title',
            'onUpdate'=> true
        ]];
    }
}
