<?php

namespace App\Http\Resources;

use Illuminate\Http\Request;
use Illuminate\Http\Resources\Json\JsonResource;

class PictureResource extends JsonResource
{
    /**
     * Transform the resource into an array.
     *
     * @return array<string, mixed>
     */
    public function toArray(Request $request): array
    {
        return [
            'title' => $this->title,
            'description' => $this->description,
            'image_url' => $this->image_url,
            'artist' => new UserResource($this->artist),
            'created_at' => $this->created_at->format('Y-m-d H:i:s'),
        ];
    }
}
