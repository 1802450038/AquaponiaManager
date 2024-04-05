<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateDataBoardsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('data_boards', function (Blueprint $table) {
            $table->id();
            $table->bigIncrements('id');
            $table->foreign("board_id")->references("id")->on("boards");
            $table->float("ph_value");
            $table->float("temp_value");
            $table->float("level_value");
            $table->float("relay_value");
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
        Schema::dropIfExists('data_boards');
    }
}
