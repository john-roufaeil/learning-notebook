<?php

namespace App\Http\Controllers;

use App\Models\Post;
use App\Models\User;
use Illuminate\Http\Request;

class PostController extends Controller
{
    /**
     * Display a listing of the resource.
     */
    public function index()
    {
        $posts = Post::with('author')->paginate(10);
        return view('posts.index', compact('posts'));
    }

    /**
     * Show the form for creating a new resource.
     */
    public function create()
    {
        $users = User::pluck('name', 'id');
        return view('posts.create', compact('users'));
    }

    /**
     * Store a newly created resource in storage.
     */
    public function store(Request $request)
    {
        $request->validate([
            'title'=> 'required|string',
            'content'=> 'required|string',
            'author_id'=> 'required|exists:users,id'
        ]);
        $post = Post::create($request->only('title', 'content', 'author_id'));
        return redirect()->route('posts.index');
    }

    /**
     * Display the specified resource.
     */
    public function show(int $id)
    {
        $post = Post::with('author')->findOrFail($id);
        return view('posts.show', compact('post'));
    }

    /**
     * Show the form for editing the specified resource.
     */
    public function edit(int $id)
    {
        $post = Post::with('author')->findOrFail($id);
        $users = User::pluck('name','id');
        return view('posts.edit', compact('post', 'users'));
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, int $id)
    {
        $request->validate([
            'title' => 'required|string',
            'content' => 'required|string',
            'author_id' => 'required|exists:users,id',
        ]);
        $post = Post::findOrFail($id);
        $post->update($request->only('title', 'content', 'author_id'));
        return redirect()->route('posts.index');
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(int $id)
    {
        $post = Post::findOrFail($id);
        $post->delete();
        return redirect()->route('posts.index');
    }
}
