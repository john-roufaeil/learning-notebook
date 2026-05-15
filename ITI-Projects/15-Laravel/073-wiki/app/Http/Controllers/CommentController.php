<?php

namespace App\Http\Controllers;

use App\Models\User;
use App\Models\Comment;
use App\Http\Requests\StoreCommentRequest;

class CommentController extends Controller {
    public function store(StoreCommentRequest $request) {
        $authorId = auth()->id() ?? User::first()?->id ?? 1;
        Comment::create($request->validated() + ['author_id' => $authorId]);
        return back();
    }

    public function destroy(Comment $comment) {
        $comment->delete();
        return back();
    }
}
