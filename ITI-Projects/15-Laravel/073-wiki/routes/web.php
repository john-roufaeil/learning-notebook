<?php

use App\Http\Controllers\ArticleController;
use App\Http\Controllers\UserController;
use App\Http\Controllers\CommentController;
use Illuminate\Support\Facades\Route;

Route::get('/', function () {
    return redirect()->route('articles.index');
});

Route::get('/articles/trashed', [ArticleController::class, 'trashed'])->name('articles.trashed');
Route::post('/articles/trashed/restore-all/', [ArticleController::class, 'restoreAll'])->name('articles.restore-all');
Route::delete('articles/trashed/force-delete-all', [ArticleController::class, 'forceDeleteAll'])->name('articles.force-delete-all');
Route::patch('/articles/{article}/restore', [ArticleController::class, 'restore'])->name('articles.restore')->withTrashed();
Route::delete('articles/{article}/force-delete', [ArticleController::class, 'forceDelete'])->name('articles.force-delete')->withTrashed();

Route::post('/comments', [CommentController::class, 'store'])->name('comments.store');
Route::delete('/comments/{comment}', [CommentController::class, 'destroy'])->name('comments.destroy');

Route::resource('articles', ArticleController::class);
Route::resource('users', UserController::class);