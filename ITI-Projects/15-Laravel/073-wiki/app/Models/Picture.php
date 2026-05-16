<?php

namespace App\Models;

use Cviebrock\EloquentSluggable\Sluggable;
use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Relations\BelongsTo;
use Illuminate\Database\Eloquent\SoftDeletes;
use Illuminate\Database\Eloquent\Relations\MorphMany;
use Illuminate\Database\Eloquent\Casts\Attribute;

class Picture extends Model
{
    use HasFactory;
    use Sluggable;
    use SoftDeletes;

    protected $fillable = ['title', 'description', 'image_path', 'artist_id'];

    public function sluggable(): array
    {
        return ['slug' => [
            'source' => 'title',
            'onUpdate' => true
        ]];
    }

    public function getRouteKeyName(): string
    {
        return 'slug';
    }

    public function artist(): BelongsTo
    {
        return $this->belongsTo(User::class, 'artist_id');
    }

    public function comments(): MorphMany
    {
        return $this->morphMany(Comment::class, 'commentable');
    }

    protected function title(): Attribute
    {
        return Attribute::make(
            get: fn($value) => ucwords($value),
            set: fn($value) => strtolower(trim($value))
        );
    }

    protected function description(): Attribute
    {
        return Attribute::make(
            set: fn($value) => trim($value)
        );
    }

    protected function imagePath(): Attribute
    {
        return Attribute::make(
            set: fn($value) => ltrim(str_replace('storage/', '', $value), '/')
        );
    }

    protected function imageUrl(): Attribute
    {
        return Attribute::make(
            get: fn() => $this->image_path
                ? asset('storage/' . $this->image_path)
                : asset('storage/pictures/placeholder.png'),
        );
    }

    protected function isDeletableImage(): Attribute
    {
        return Attribute::make(
            get: fn() => (bool) $this->image_path
                && basename($this->image_path) !== 'placeholder.png',
        );
    }
}
