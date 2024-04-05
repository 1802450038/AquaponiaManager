   
   @extends('site.layouts.mainframe')
   @section('title', 'Leituras')
   @section('content')
   <div class="body">
        <div class="body-content">
            <div class="row inputs">
                <div class="col-100 ">
                    <div class="card">
                        <div class="card-title">
                            <h4>Leitura atual</h4>
                        </div>
                        <div class="row">
                            <div class="col-25">
                                <div class="slider">
                                    <div id="slider-ph"></div>
                                </div>
                                <div class="slider-title">
                                    <span>Ph</span>
                                </div>
                            </div>
                            <div class="col-25">
                                <div class="slider">
                                    <div id="slider-level"></div>
                                </div>
                                <div class="slider-title">
                                    <span>Nivel</span>
                                </div>
                            </div>
                            <div class="col-25">
                                <div class="slider">
                                    <div id="slider-temp"></div>
                                </div>
                                <div class="slider-title">
                                    <span>Temperatura</span>
                                </div>
                            </div>
                            <div class="col-25">
                                <div class="slider">
                                    <div id="slider-relay"></div>
                                </div>
                                <div class="slider-title">
                                    <span>Relé</span>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="row graph">
                <div class="col-100">
                    <div class="card">
                        <div class="card-title">
                            <h4>Grafico</h4>
                            <div class="card-options">
                            </div>
                        </div>
                        <div class="graph-body">
                            <canvas id="myChart"></canvas>
                        </div>
                    </div>
                </div>
            </div>
            <div class="row log">
                <div class="col-100">
                    <div class="card">
                        <div class="card-title">
                            <h4>Logs</h4>
                            <div class="card-options">
                                <form method="post" class="inline-form" name="log-options" id="log-options">
                                    <div class="input-group">
                                        <div class="custom-select">
                                            <label for="sensor">Sensor</label>
                                            <select name="sensor" id="sensor">
                                                <option value="temp" selected>Temperatura</option>
                                                <option value="relay">Relé</option>
                                                <option value="ph">Ph</option>
                                                <option value="level">Nivel</option>
                                            </select>
                                        </div>
                                    </div>
                                    
                                    <div class="input-group">
                                        <label for="date-log">Data</label>
                                        <input name="date-log" id="date-log" type="date">
                                    </div>
                                    <div class="input-group">
                                        <label for="hour-log-start">Hora inicio</label>
                                        <input name="hour-log-start" id="hour-log-start" type="time" value="12:00">
                                    </div>
                                    <div class="input-group">
                                        <label for="hour-log-end">Hora final</label>
                                        <input name="hour-log-end" id="hour-log-end" type="time" value="13:00">
                                    </div>
                              
                                    <div class="input-group">
                                        <button id="send-log" type="button"> <i class="fas fa-arrow-right"></i></button>
                                    </div>
                                    <div class="input-group">
                                        <button id="auto-update" type="button" class="green"> <i class="fas fa-arrow-rotate-right"></i></button>
                                    </div>
                                </form>
                            </div>
                        </div>
                        <div class="col-100">
                            <div class="logs-body">
                                <ul class="last-logs" id="last-logs">
                                   
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

        </div>
    </div>
 @endsection