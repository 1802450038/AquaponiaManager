let liveUpdate = true;
// --primary-l : #73a3fa;
// --primary : #6095f7;
// --primary-d : #3377f7;
// --primary-l : #fa73d8;
// --primary : #f760e3;
// --primary-d : #f733e7;
// --primary-l : #7efa73;
// --primary : #60f760;
// --primary-d : #5af733;
// --primary-l : #fa7373;
// --primary : #f76060;
// --primary-d : #f73333;

function loadDate() {
  let dateForm = document.getElementById("date-log");
  let hourStartForm = document.getElementById("hour-log-start");
  let hourEndForm = document.getElementById("hour-log-end");
  const DateLocale = new Date().toLocaleString("pt-BR").split(", ");
  const DateDay = DateLocale[0].split("/").reverse();
  const DateTime = DateLocale[1].split(":");
  dateForm.value = DateDay[0] + "-" + DateDay[1] + "-" + DateDay[2];

  if (DateTime[0] < 3) {
    hourStartForm.value = "00" + ":" + "00";
    hourEndForm.value = DateTime[0] + ":" + DateTime[1];
  } else {
    hourStartForm.value = DateTime[0] - 3 + ":" + DateTime[1];
    hourEndForm.value = DateTime[0] + ":" + DateTime[1];
  }

  return DateTime + " " + DateDay;
}
loadDate();

const ctx = document.getElementById("myChart");
let last_response;
let labels_graph = [];
let data_graph = [];
let api_key = "tPmAT5Ab3j7F9";
let sensor = $("#sensor").val();
let date_log = $("#date-log").val();
let hour_log_start = $("#hour-log-start").val();
let hour_log_end = $("#hour-log-end").val();
let quantity = calcTimeDiffMin(hour_log_start, hour_log_end);

chart = new Chart(ctx, {
  type: "line",
  data: {
    labels: [],
    datasets: [
      {
        label: "Temp",
        data: [],
        borderWidth: 1,
      },
    ],
  },
  options: {
    scales: {
      y: {
        beginAtZero: true,
      },
    },
  },
});

$("#slider-level").roundSlider({
  sliderType: "min-range",
  circleShape: "pie",
  startAngle: "315",
  lineCap: "round",
  radius: 80,
  width: 20,

  min: 0,
  max: 250,

  svgMode: true,
  pathColor: "#a1a1a1",
  borderWidth: 0,

  startValue: 0,

  valueChange: function (e) {
    var color = e.value > e.options["max"] / 2 ? "#e65744" : "#3377f7";
    $("#slider-level").roundSlider({ rangeColor: color, tooltipColor: color });
  },
});

$("#slider-ph").roundSlider({
  sliderType: "min-range",
  circleShape: "pie",
  startAngle: "315",
  lineCap: "round",
  radius: 80,
  width: 20,

  min: 0,
  max: 14,

  svgMode: true,
  pathColor: "#a1a1a1",
  borderWidth: 0,

  startValue: 0,

  valueChange: function (e) {
    var color = e.value > e.options["max"] / 2 ? "#e65744" : "#3377f7";
    $("#slider-ph").roundSlider({ rangeColor: color, tooltipColor: color });
  },
});

$("#slider-relay").roundSlider({
  sliderType: "min-range",
  circleShape: "pie",
  startAngle: "315",
  lineCap: "round",
  radius: 80,
  width: 20,

  min: 0,
  max: 1,

  svgMode: true,
  pathColor: "#a1a1a1",
  borderWidth: 0,
  startValue: 0,
  rangeColor: "#e65744",
  tooltipColor: "#a1a1a1"
});

$("#slider-temp").roundSlider({
  sliderType: "min-range",
  circleShape: "pie",
  startAngle: "315",
  lineCap: "round",
  radius: 80,
  width: 20,
  min: 0,
  max: 100,
  svgMode: true,
  pathColor: "#a1a1a1",
  borderWidth: 0,
  startValue: 0,
  rangeColor: "#e65744",
  tooltipColor: "#a1a1a1"
  // valueChange: function (e) {
  //   var color = e.value > e.options["max"] / 2 ? "#e65744" : "#3377f7";
  //   $("#slider-temp").roundSlider({ rangeColor: color, tooltipColor: color });
  // },
});

var sliderPh = $("#slider-ph").data("roundSlider");
var sliderLevel = $("#slider-level").data("roundSlider");
var sliderRelay = $("#slider-relay").data("roundSlider");
var sliderTemp = $("#slider-temp").data("roundSlider");

sliderPh.setValue(7);
sliderLevel.setValue(75);
sliderRelay.setValue(1);
sliderTemp.setValue(23);

function toggleFloating(elem) {
  if (elem.parentElement.parentElement.classList.contains("active")) {
    elem.parentElement.parentElement.classList.remove("active");
  } else {
    elem.parentElement.parentElement.classList.add("active");
  }
}

function getSubField(value, icon) {
  return (
    // "<span class='log-value'>" + value + " <i class='" + icon + "'></i></span>"
    "<span class='log-value'>" + value + " "
  );
}

function appendResult(response) {
  let resultsHtml = document.getElementById("last-logs");
  let result = response.split("|");

  if (JSON.parse(result[0]).id != last_response) {
    
    last_response = JSON.parse(result[0]).id;    
    console.log(last_response);

    sliderPh.setValue(JSON.parse(result[0]).ph_value);
    sliderLevel.setValue(JSON.parse(result[0]).level_value);
    sliderRelay.setValue(JSON.parse(result[0]).relay_value);
    sliderTemp.setValue(JSON.parse(result[0]).temp_value);
    resultsHtml.innerHTML = "";
    data_graph = [];
    labels_graph = [];
    result.forEach((res) => {
      if (res.length > 3) {
        let finalResult = JSON.parse(res);
        labels_graph.push(
          finalResult.date.split(" ")[1].split(":")[0] +
            ":" +
            finalResult.date.split(" ")[1].split(":")[1]
        );

        if (sensor == "relay") {
          data_graph.push(finalResult.relay_value);
        } else if (sensor == "temp") {
          data_graph.push(finalResult.temp_value);
        } else if (sensor == "ph") {
          data_graph.push(finalResult.ph_value);
        } else if (sensor == "level") {
          data_graph.push(finalResult.level_value);
        }
        resultsHtml.innerHTML +=
          "<li class='log-content'>" +
          getSubField(finalResult.id, "fas fa-barcode") +
          getSubField(finalResult.ph_value, "fas fa-ruler") +
          getSubField(finalResult.temp_value, "fas fa-ruler") +
          getSubField(finalResult.level_value, "fas fa-ruler") +
          getSubField(finalResult.realy_value, "fas fa-ruler") +
          getSubField(finalResult.date, "fas fa-calendar");
        ("</li>");
      }
    });
    chart.data.datasets[0].data = data_graph.reverse();
    chart.data.labels = labels_graph.reverse();
    chart.data.datasets[0].label = getLabelAndColor(sensor)[0];
    chart.data.datasets[0].backgroundColor = getLabelAndColor(sensor)[1];
    chart.data.datasets[0].borderColor = getLabelAndColor(sensor)[2];
    console.log("Something new");
  } else {
    console.log("Old data");
  }
}

function sendRequest(function_name) {
  var urlData =
    "&api_key=" +
    api_key +
    "&function_name=" +
    function_name +
    "&sensor=" +
    sensor +
    "&date-log=" +
    date_log +
    "&hour-log-start=" +
    hour_log_start +
    "&hour-log-end=" +
    hour_log_end +
    "&quantity=" +
    quantity;

  $.ajax({
    type: "POST" /* tipo post */,
    url: "http://177.36.44.91/aquatest/read_logs_new.php" /* endereço do script PHP */,
    async: true,
    data: urlData /* informa Url */,
    success: function (data) {
      appendResult(data);
    },
    beforeSend: function () {},
    complete: function () {
      chart.update();
    },
    fail: function () {
      console.log("Falha");
    },
  });
}

function hourToMinutes(hour) {
  return parseInt(hour.split(":")[0] * 60) + parseInt(hour.split(":")[1]);
}

function calcTimeDiffMin(timeStart, timeEnd) {
  return parseInt(hourToMinutes(timeEnd) - hourToMinutes(timeStart));
}

function calcTimeDiffHour(timeStart, timeEnd) {
  return parseInt((hourToMinutes(timeEnd) - hourToMinutes(timeStart)) / 60);
}

function getLabelAndColor(sensor) {
  switch (sensor) {
    case "ph":
      return ["PH", "#7efa73", "#5af733"];
      break;
    case "level":
      return ["NIVEL", "#73a3fa", "#3377f7"];
      break;
    case "temp":
      return ["TEMPERATURA", "#fa7373", "#f73333"];
      break;
    case "relay":
      return ["RELÉ", "#fa73d8", "#f733e7"];
      break;
    default:
      return ["NENHUM", "#646464", "#222222"];
      break;
  }
}



function loadValuesFromForm() {
  sensor = $("#sensor").val();
  date_log = $("#date-log").val();
  hour_log_start = $("#hour-log-start").val();
  hour_log_end = $("#hour-log-end").val();
  quantity = calcTimeDiffMin(hour_log_start, hour_log_end);
}

$(document).ready(function () {
  $("#send-log").click(function () {
    loadValuesFromForm();
    if (calcTimeDiffHour(hour_log_start, hour_log_end) > 6) {
      alert("O intervalo de horas não pode ser maior que 6 Horas");
      return false;
    } else {
      sendRequest("getLogParam");
    }
  });
});

function toggleLiveUpdate() {
  $("#auto-update").toggleClass("inactive");
  liveUpdate = !liveUpdate;
}

function disableLiveUpdate() {
  console.log("From disable");
  $("#auto-update").addClass("inactive");
  liveUpdate = false;
}
$(document).ready(function () {
  $("#hour-log-start").click(function () {
    disableLiveUpdate();
  });
  $("#hour-log-end").click(function () {
    disableLiveUpdate();
  });
});

$(document).ready(function () {
  $("#auto-update").click(function () {
    toggleLiveUpdate();
  });
});

setInterval(function () {
  if (liveUpdate == true) {
    loadDate();  
    loadValuesFromForm();
    sendRequest("getLogParam");
  } else {
    loadValuesFromForm();
  }
}, 5000);

window.onload = sendRequest("getLogParam");
