@section('title','Ajustes')
@extends('site.layouts.mainframe')
   @section('content')
        <div class="body">
            <div class="body-content">
                <div class="row inputs">
                    <div class="col-100 ">
                        <div class="card">
                            <div class="card-title">
                                <h4>Parametros ajuste</h4>
                            </div>
                            <div class="row">
                                <form action="" class="form-settins">
                                    <div class="row">
                                        <div class="row-title">Ajustes PH</div>
                                        <div class="col-30">
                                            <div class="input-group">
                                                <label for="value">Ph minimo</label>
                                                <input type="text">
                                            </div>
                                        </div>
                                        <div class="col-30">

                                            <div class="input-group">
                                                <label for="value">Ph maximo</label>
                                                <input type="text">
                                            </div>
                                        </div>
                                        <div class="col-30">

                                            <div class="input-group">
                                                <label for="value">Fator ajuste ph</label>
                                                <input type="text">
                                            </div>
                                        </div>
                                    </div>
                                    <div class="row">
                                        <div class="row-title">Ajustes Temperatura</div>
                                        <div class="col-30">
                                            <div class="input-group">
                                                <label for="value">Temperatura minima</label>
                                                <input type="text">
                                            </div>
                                        </div>
                                        <div class="col-30">

                                            <div class="input-group">
                                                <label for="value">Temperatura maxima</label>
                                                <input type="text">
                                            </div>
                                        </div>
                                        <div class="col-30">

                                            <div class="input-group">
                                                <label for="value">Fator ajuste temperatura</label>
                                                <input type="text">
                                            </div>
                                        </div>
                                    </div>
                                    <div class="row">
                                        <div class="row-title">Ajustes nivel</div>
                                        <div class="col-30">
                                            <div class="input-group">
                                                <label for="value">Nivel minimo</label>
                                                <input type="text">
                                            </div>
                                        </div>
                                        <div class="col-30">

                                            <div class="input-group">
                                                <label for="value">Nivel maximo</label>
                                                <input type="text">
                                            </div>
                                        </div>
                                        <div class="col-30">
                                            <div class="input-group">
                                                <label for="value">Fator de ajuste nivel</label>
                                                <input type="text">
                                            </div>
                                        </div>
                                    </div>
                                    <div class="row">
                                        <div class="row-title">Ajustes relé</div>
                                        <div class="col-25">
                                            <div class="input-group">
                                                <label for="value">Hora acionamento</label>
                                                <input type="time">
                                            </div>
                                        </div>
                                        <div class="col-25">

                                            <div class="input-group">
                                                <label for="value">Hora desligamento</label>
                                                <input type="time">
                                            </div>
                                        </div>
                                        <div class="col-25">
                                            <div class="input-group">
                                                <div class="custom-select">
                                                    <label for="select-sensor">Estado padrão</label>
                                                    <select name="select-sensor" id="select-sensor">
                                                        <option value="temp">Desligado</option>
                                                        <option value="relay" selected>Ligado</option>
                                                    </select>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="col-25">
                                            <div class="input-group">
                                                <div class="custom-select">
                                                    <label for="select-sensor">Ph de acionamento</label>
                                                    <select name="select-sensor" id="select-sensor">
                                                        <option value="1" selected>Ph 1</option>
                                                        <option value="2">Ph 2</option>
                                                        <option value="3">Ph 3</option>
                                                        <option value="4">Ph 4</option>
                                                        <option value="5">Ph 5</option>
                                                        <option value="6">Ph 6</option>
                                                        <option value="7">Ph 7</option>
                                                        <option value="8">Ph 8</option>
                                                        <option value="9">Ph 9</option>
                                                        <option value="10">Ph 10</option>
                                                        <option value="11">Ph 11</option>
                                                        <option value="12">Ph 12</option>
                                                        <option value="13">Ph 13</option>
                                                        <option value="14">Ph 14</option>
                                                    </select>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="row">
                                        <div class="row-title">Ações</div>
                                        <div class="col-50">
                                            <div class="input-group">
                                                <button class="secondary">Valores padrões <i
                                                        class="fas fa-arrow-rotate-left"></i></button>
                                            </div>
                                        </div>
                                        <div class="col-50">

                                            <div class="input-group">
                                                <button>Salvar <i class="fas fa-save"></i></button>
                                            </div>
                                        </div>
                                    </div>
                                </form>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    @endsection
