<?php

use App\Http\Controllers\ArticleController;
use App\Http\Controllers\UserController;
use Illuminate\Support\Facades\Route;

Route::get('/', function () {
    return redirect()->route('articles.index');
});

Route::get('/articles/trashed', [ArticleController::class, 'trashed'])->name('articles.trashed');
Route::patch('/articles/{id}/restore', [ArticleController::class, 'restore'])->name('articles.restore');
Route::post('/articles/trashed/restore-all/', [ArticleController::class, 'restoreAll'])->name('articles.restore-all');
Route::delete('articles/{id}/force-delete', [ArticleController::class, 'forceDelete'])->name('articles.force-delete');
Route::delete('articles/trashed/force-delete-all', [ArticleController::class, 'forceDeleteAll'])->name('articles.force-delete-all');

Route::resource('articles', ArticleController::class);
Route::resource('users', UserController::class);