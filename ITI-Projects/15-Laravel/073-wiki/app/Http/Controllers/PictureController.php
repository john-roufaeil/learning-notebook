<?php

namespace App\Http\Controllers;

use App\Http\Requests\StorePictureRequest;
use App\Http\Requests\UpdatePictureRequest;
use App\Models\Picture;
use App\Models\User;
use Illuminate\Support\Facades\Storage;

class PictureController extends Controller
{
  public function index()
  {
    $pictures = Picture::with('artist')->paginate(10);
    return view('pictures.index', compact('pictures'));
  }

  public function create()
  {
    $users = User::pluck('name', 'id');
    return view('pictures.create', compact('users'));
  }

  public function store(StorePictureRequest $request)
  {
    $data = $request->validated();
    if ($request->hasFile('image')) {
      $data['image_path'] = $request->file('image')->store('pictures', 'public');
    }
    $data['artist_id'] = auth()->id() ?? User::first()?->id;
    Picture::create($data);
    return redirect()->route('pictures.index');
  }

  public function show(Picture $picture)
  {
    $picture->load(['artist', 'comments.author']);
    return view('pictures.show', compact('picture'));
  }

  public function edit(Picture $picture)
  {
    $users = User::pluck('name', 'id');
    return view('pictures.edit', compact('picture', 'users'));
  }

  public function update(UpdatePictureRequest $request, Picture $picture)
  {
    $data = $request->validated();
    if ($request->hasFile('image')) {
      if ($picture->image_path && basename($picture->image_path) !== 'placeholder.png') {
        Storage::disk('public')->delete($picture->image_path);
      }
      $data['image_path'] = $request->file('image')->store('pictures', 'public');
    }
    $picture->update($data);
    return redirect()->route('pictures.index');
  }

  public function destroy(Picture $picture)
  {
    $picture->delete();
    return redirect()->route('pictures.index');
  }

  public function trashed()
  {
    $trashedPictures = Picture::onlyTrashed()->paginate(10);
    return view('pictures.trashed', compact('trashedPictures'));
  }

  public function restore($picture)
  {
    $model = Picture::onlyTrashed()->where('id', $picture)->orWhere('slug', $picture)->firstOrFail();
    $model->restore();
    return redirect()->route('pictures.trashed');
  }

  public function restoreAll()
  {
    Picture::onlyTrashed()->restore();
    return redirect()->route('pictures.trashed');
  }

  public function forceDelete($picture)
  {
    $model = Picture::onlyTrashed()->where('id', $picture)->orWhere('slug', $picture)->firstOrFail();
    if ($model->image_path && basename($model->image_path) !== 'placeholder.png') {
      Storage::disk('public')->delete($model->image_path);
    }
    $model->forceDelete();
    return redirect()->route('pictures.trashed');
  }

  public function forceDeleteAll()
  {
    $trashedPictures = Picture::onlyTrashed()->get();
    foreach ($trashedPictures as $picture) {
      if ($picture->image_path && basename($picture->image_path) !== 'placeholder.png') {
        Storage::disk('public')->delete($picture->image_path);
      }
      $picture->forceDelete();
    }
    return redirect()->route('pictures.trashed');
  }
}
