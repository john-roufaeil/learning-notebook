<?php

use App\Http\Controllers\ArticleController;
use App\Http\Controllers\UserController;
use Illuminate\Support\Facades\Route;

Route::get('/', function () {
    return redirect()->route('posts.index');
});
Route::get('/articles/trashed', [ArticleController::class, 'trashed'])->name('articles.trashed');
Route::patch('/articles/{id}/restore', [ArticleController::class, 'restore'])->name('articles.restore');
Route::resource('articles', ArticleController::class);
Route::resource('users', UserController::class);