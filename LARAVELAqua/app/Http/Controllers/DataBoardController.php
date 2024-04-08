<?php

namespace App\Http\Controllers;

use App\DataBoard;
use Illuminate\Http\Request;

class DataBoardController extends Controller
{
   


    function getDataLog(int $board_id, float $ph_value, float $temp_value, float $level_value, int $relay_value)
    {
        $values = [
            'board_id' => $board_id,
            'ph_value' => $ph_value,
            'temp_value' => $temp_value,
            'level_value' => $level_value,
            'relay_value' => $relay_value
        ];
        DataBoard::create($values);

        return DataBoard::all();
    }
}
