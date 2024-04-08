<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class DataBoard extends Model
{
    protected $fillable = ['board_id', 'ph_value', 'temp_value', 'level_value', 'relay_value'];
}
