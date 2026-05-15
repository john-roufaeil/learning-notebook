<?php

namespace App\Http\Controllers;

use App\Models\Post;
use App\Models\User;
use App\Http\Requests\StorePostRequest;
use App\Http\Requests\UpdatePostRequest;

class PostController extends Controller
{
    public function index() {
        $posts = Post::with('author')->paginate(10);
        return view('posts.index', compact('posts'));
    }

    public function create() {
        $users = User::pluck('name', 'id');
        return view('posts.create', compact('users'));
    }

    public function store(StorePostRequest $request) {
        Post::create($request->validated());
        return redirect()->route('posts.index');
    }

    public function show(int $id) {
        $post = Post::with('author')->findOrFail($id);
        return view('posts.show', compact('post'));
    }

    public function edit(int $id) {
        $post = Post::with('author')->findOrFail($id);
        $users = User::pluck('name','id');
        return view('posts.edit', compact('post', 'users'));
    }

    public function update(UpdatePostRequest $request, int $id) {
        $post = Post::findOrFail($id);
        $post->update($request->validated());
        return redirect()->route('posts.index');
    }
    
    public function destroy(int $id) {
        $post = Post::findOrFail($id);
        $post->delete();
        return redirect()->route('posts.index');
    }

    public function trashed() {
        $posts = Post::onlyTrashed()->paginate(10);
        return view('posts.trashed', compact('posts'));
    }
}
