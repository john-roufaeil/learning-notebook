<?php

use App\Http\Controllers\PostController;
use App\Http\Controllers\UserController;
use Illuminate\Support\Facades\Route;

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider and all of them will
| be assigned to the "web" middleware group. Make something great!
|
*/
Route::get('/', function () {
    return redirect()->route('posts.index');
});
Route::get('/posts/trashed', [PostController::class, 'trashed'])->name('posts.trashed');
Route::patch('/posts/{id}/restore', [PostController::class, 'restore'])->name('posts.restore');
Route::resource('posts', PostController::class);
Route::resource('users', UserController::class);