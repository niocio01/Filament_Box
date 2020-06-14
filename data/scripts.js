window.onload = function () {
    tempGraph();
    updateSensorData();
}

function tempGraph() {
    var ctx_live = document.getElementById("mycanvas");
    window.myChart = new Chart(ctx_live, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: 'Temperature',
                borderColor: '#00c0ef',
                data: []
                 }, {
                label: 'Humidity',
                borderColor: '#FF0000',
                data: []
                }]
        },
        options: {
            responsive: true,
            title: {
                display: true,
                text: "Chart.js - Dynamically Update Chart Via Ajax Requests",
            },
            legend: {
                display: false
            },
            scales: {
                yAxes: [{
                    ticks: {
                        beginAtZero: true,
                    }
                }]
            }
        }
    });
}


function addData(chart, label, temp, hum) {
    chart.data.labels.push(label);
    chart.data.datasets[0].data.push(temp);
    chart.data.datasets[1].data.push(hum);

    chart.update();
}

var noOfPoints = 0;

Number.prototype.pad = function(size) {
    var s = String(this);
    while (s.length < (size || 2)) {s = "0" + s;}
    return s;
}

function time()
{
    var hours = (Math.floor(Math.round(performance.now()/1000)/3600)).pad(2)
    var minutes = (Math.floor(Math.round(performance.now()/1000)/60)).pad(2)
    var seconds = (Math.round(performance.now()/1000)%60).pad(2)

    var time = hours + ":" + minutes + ":" + seconds
    return time
}

function updateSensorData() {
    setInterval(requestData, 1000);
    function requestData() {
        $.get("/sensors")
            .done(function (data) {
                console.log(data); //debuging
                if (data) {
                    $("#temp").text(data.temp);
                    $("#hum").text(data.hum);
                    addData(myChart, time() , data.temp, data.hum);
                } else {
                    $("#temp").text("?");
                    $("#hum").text("?");
                }
            }).fail(function () {
                console.log("The was a problem retrieving the data.");
            });
    }
};



