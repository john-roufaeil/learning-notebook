<?php

namespace App\Models;

use Cviebrock\EloquentSluggable\Sluggable;
use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Relations\BelongsTo;
use Illuminate\Database\Eloquent\SoftDeletes;
use Illuminate\Database\Eloquent\Relations\MorphMany;

class Article extends Model
{
    use HasFactory;
    use Sluggable;
    use SoftDeletes;

    protected $fillable = ['title', 'content', 'author_id'];

    public function author(): BelongsTo {
        return $this->belongsTo(User::class, 'author_id');
    }

    public function sluggable(): array {
        return ['slug' => [
            'source' => 'title',
            'onUpdate'=> true
        ]];
    }

    public function getRouteKeyName(): string {
        return 'slug';
    }

    public function comments(): MorphMany {
        return $this->morphMany(Comment::class, 'commentable');
    }
}
