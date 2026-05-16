<?php

namespace App\Http\Controllers\Api;

use App\Http\Controllers\Controller;
use App\Http\Requests\StoreArticleRequest;
use App\Http\Resources\ArticlesResource;
use App\Models\Article;

class ArticleController extends Controller
{
    public function index()
    {
        $per_page = request()->query('per_page', 10);
        $articles = Article::with('author')->paginate($per_page);
        return response()->json(
            ArticlesResource::collection($articles)->toResponse(request())->getData(),
            200,
            [],
            JSON_PRETTY_PRINT
        );
    }

    public function show(string $slug)
    {
        $article = Article::with('author')->where('slug', $slug)->first();

        if (!$article) {
            return response()->json([
                "status" => "failed",
                "message" => "Article not found"
            ], 404);
        }

        return response()->json(
            ArticlesResource::make($article)->toResponse(request())->getData(),
            200,
            [],
            JSON_PRETTY_PRINT
        );
    }

    public function store(StoreArticleRequest $request)
    {
        $article = Article::create([
            ...$request->validated(),
            'author_id' => $request->user()->id
        ]);
        return response()->json(
            ArticlesResource::make($article)->toResponse(request())->getData(),
            200,
            [],
            JSON_PRETTY_PRINT
        );
    }
}
