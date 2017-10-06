google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(drawChart);

function drawNewChart(id, data, options) {
    var chart = new google.visualization.LineChart(document.getElementById(id));

    chart.draw(data, options);

    function resize(){
        chart.draw(data, options);
    }

    if (window.addEventListener)
        window.addEventListener('resize', resize);
    else
        window.attachEvent('onresize', resize);
}

function drawChart() {
    var data = google.visualization.arrayToDataTable([
        ['Year', 'Sales', 'Expenses'],
        ['2004', 1000, 400],
        ['2005', 1170, 460],
        ['2006', 660, 1120],
        ['2007', 1030, 540]
    ]);

    var options = {
        title: 'Company Performance',
        curveType: 'function',
        legend: {position: 'bottom'}
    };

    drawNewChart('c1', data, options);
    drawNewChart('c2', data, options);
    drawNewChart('c3', data, options);
    drawNewChart('c4', data, options);


    var sliderC = new Slider("#ex12c", { id: "slider12c", min: 0, max: 100, range: true, value: [15, 25] });

}

