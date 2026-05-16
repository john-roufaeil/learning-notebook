<?php

namespace App\Http\Controllers\Api;

use App\Http\Controllers\Controller;
use App\Http\Requests\StorePictureRequest;
use App\Http\Resources\PictureResource;
use App\Models\Picture;

class PictureController extends Controller
{
    public function index()
    {
        $per_page = request()->query('per_page', 10);
        $pictures = Picture::with('artist')->paginate($per_page);
        return response()->json(
            PictureResource::collection($pictures)->toResponse(request())->getData(),
            200,
            [],
            JSON_PRETTY_PRINT
        );
    }

    public function show(string $slug)
    {
        $picture = Picture::with('artist')->where('slug', $slug)->first();

        if (!$picture) {
            return response()->json([
                'status'  => 'failed',
                'message' => 'Picture not found',
            ], 404);
        }

        return response()->json(
            PictureResource::make($picture)->toResponse(request())->getData(),
            200,
            [],
            JSON_PRETTY_PRINT
        );
    }

    public function store(StorePictureRequest $request)
    {
        $path = $request->file('image')->store('pictures', 'public');
        $picture = Picture::create([
            ...$request->validated(),
            'image_path' => $path,
            'artist_id' => $request->user()->id
        ]);
        return response()->json(
            PictureResource::make($picture)->toResponse(request())->getData(),
            201,
            [],
            JSON_PRETTY_PRINT
        );
    }
}
