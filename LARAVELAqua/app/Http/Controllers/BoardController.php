<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;

class BoardController extends Controller
{
    function home() {
        return view('site.board');
    }

    function settings() {
        return view('site.settings');
    }
}
