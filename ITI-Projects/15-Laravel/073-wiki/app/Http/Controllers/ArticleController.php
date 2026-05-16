<?php

namespace App\Http\Controllers;

use App\Models\Article;
use App\Models\User;
use App\Http\Requests\StoreArticleRequest;
use App\Http\Requests\UpdateArticleRequest;

class ArticleController extends Controller
{
    public function index()
    {
        $articles = Article::with('author')->paginate(10);
        return view('articles.index', compact('articles'));
    }

    public function create()
    {
        $users = User::pluck('name', 'id');
        return view('articles.create', compact('users'));
    }

    public function store(StoreArticleRequest $request)
    {
        Article::create($request->validated());
        return redirect()->route('articles.index');
    }

    public function show(Article $article)
    {
        $article->load(['author', 'comments.author']);
        return view('articles.show', compact('article'));
    }

    public function edit(Article $article)
    {
        $users = User::pluck('name', 'id');
        return view('articles.edit', compact('article', 'users'));
    }

    public function update(UpdateArticleRequest $request, Article $article)
    {
        $article->update($request->validated());
        return redirect()->route('articles.index');
    }

    public function destroy(Article $article)
    {
        $article->delete();
        return redirect()->route('articles.index');
    }

    public function trashed()
    {
        $trashedArticles = Article::onlyTrashed()->paginate(10);
        return view('articles.trashed', compact('trashedArticles'));
    }

    public function restore(Article $article)
    {
        $article->restore();
        return redirect()->route('articles.trashed');
    }

    public function restoreAll()
    {
        Article::onlyTrashed()->restore();
        return redirect()->route('articles.trashed');
    }

    public function forceDelete(Article $article)
    {
        $article->comments()->delete();
        $article->forceDelete();
        return redirect()->route('articles.trashed');
    }

    public function forceDeleteAll()
    {
        Article::onlyTrashed()->each(function ($article) {
            $article->comments()->delete();
        });
        Article::onlyTrashed()->forceDelete();
        return redirect()->route('articles.trashed');
    }
}
