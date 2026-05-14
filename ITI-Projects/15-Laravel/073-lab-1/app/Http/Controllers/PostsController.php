<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;

class PostsController extends Controller
{
  private static function getPosts()
  {
    return session('posts', [
      ['id' => 1, 'title' => 'Learn PHP', 'description' => 'PHP basics', 'user' => 'Ahmed'],
      ['id' => 2, 'title' => 'Solid Principles', 'description' => 'SOLID in OOP', 'user' => 'Mohamed'],
      ['id' => 3, 'title' => 'Design Patterns', 'description' => 'Common patterns', 'user' => 'Ali']
    ]);
  }

  public function index()
  {
    $posts = self::getPosts();
    return view('posts.index', compact('posts'));
  }

  public function create()
  {
    $users = ['Ahmed', 'Mohamed', 'Ali'];
    return view('posts.create', compact('users'));
  }

  public function store(Request $request)
  {
    $posts = self::getPosts();
    $newId = count($posts) > 0 ? max(array_column($posts, 'id')) + 1 : 1;
    $posts[] = [
      'id' => $newId,
      'title' => $request->title,
      'description' => $request->description,
      'user' => $request->user
    ];
    session(['posts' => $posts]);
    return redirect()->route('posts.index');
  }

  public function show($id)
  {
    $post = collect(self::getPosts())->firstWhere('id', $id);
    return view('posts.show', compact('post'));
  }

  public function edit($id)
  {
    $post  = collect(self::getPosts())->firstWhere('id', $id);
    $users = ['Ahmed', 'Mohamed', 'Ali'];
    return view('posts.edit', compact('post', 'users'));
  }

  public function update(Request $request, $id)
  {
    $posts = self::getPosts();
    foreach ($posts as &$post) {
      if ($post['id'] == $id) {
        $post['title'] = $request->title;
        $post['description'] = $request->description;
        $post['user'] = $request->user;
        break;
      }
    }
    session(['posts' => $posts]);
    return redirect()->route('posts.index');
  }

  public function destroy($id)
  {
    $posts = array_values(array_filter(self::getPosts(), fn($p) => $p['id'] != $id));
    session(['posts' => $posts]);
    return redirect()->route('posts.index');
  }
}