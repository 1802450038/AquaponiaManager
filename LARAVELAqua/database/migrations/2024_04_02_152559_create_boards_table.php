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
            $table->string('pass_wifi');
            $table->integer('ph_max');
            $table->integer('ph_min');
            $table->float('ph_calibration');
            $table->integer('temp_max');
            $table->integer('temp_min');
            $table->float('temp_calibration');
            $table->integer('level_max');
            $table->integer('level_min');
            $table->float('level_calibration');
            $table->String('relay_time_on');
            $table->String('relay_time_off');
            $table->boolean('relay_default_state');
            $table->integer('relay_ph_trigger');
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
