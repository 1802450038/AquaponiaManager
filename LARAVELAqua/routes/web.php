<?php

use Illuminate\Support\Facades\Route;

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

// Route::get('/', function () {
//     return view('welcome');
// });

Route::get('/', 'BoardController@home')->name('home');

// Route::get('/dataLog', 'DataBoardController@getDataLog');


Route::get('/datalog/{board_id}/{ph_value}/{temp_value}/{level_value}/{relay_value}', 'DataBoardController@getDataLog');
// Route::get('/datalog/{board_id}/{ph_value}/{temp_value}/{level_value}/{relay_value}', function(int $board_id, float $ph_value, float $temp_value, float $level_value, int $relay_value){
    
// });

Route::get('/settings', 'BoardController@settings')->name('settings');

