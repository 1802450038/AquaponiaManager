<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateBoardsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('boards', function (Blueprint $table) {
            $table->id();
            $table->string('api_key');
            $table->string('ssid_wifi');
            $table->int('ph_max');
            $table->int('ph_min');
            $table->float('ph_calibration');
            $table->int('temp_max');
            $table->int('temp_min');
            $table->float('temp_calibration');
            $table->int('level_max');
            $table->int('level_min');
            $table->float('level_calibration');
            $table->String('relay_time_on');
            $table->String('relay_time_off');
            $table->boolean('relay_default_state');
            $table->int('relay_ph_trigger');
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('boards');
    }
}
