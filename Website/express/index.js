import secrets from './secrets.json' assert { type: 'json' };
import dict from './data/sensors.json' assert { type: 'json' };

mapboxgl.accessToken = secrets.map.accessToken;
const map = new mapboxgl.Map({
    container: 'map', // container ID
    style: secrets.map.style, // style URL
    center: [-74.02, 40.75], // starting position [lng, lat]
    zoom: 10 // starting zoom
});

dict.markers = [];

map.on('load', async () => { 
    map.addSource('trace', { type: 'geojson', data: "/data/sensors.geojson" });
    
    dict.sensors.forEach(addMarker);

    // on a regular basis, add more coordinates from the saved list and update the map
    const timer = setInterval(() => {
        const geojsonSource = map.getSource('trace');
        // Update the data after the GeoJSON source was created
        geojsonSource.setData({
            "type": "FeatureCollection",
            "features": [{
                "type": "Feature",
                "properties": {"name": "Null Island"},
                "geometry": {
                        "type": "Point",
                        "coordinates": [ 0, 0 ]
                    }
                }]
        });

        //update all markers
        updateMarkers();
    }, 10);
});

function addMarker(item) {
    // create a HTML element for each feature
   const elem = document.createElement('div');
   elem.className = 'marker';
    // make a marker for each feature and add to the map
   var marker = new mapboxgl.Marker(elem);
   marker.setLngLat(item.position).addTo(map);
   marker.setPopup(addPopup(marker));

   marker.info = item.id;
   dict.markers.push(marker);
}


function addPopup(marker) {
    const popup = new mapboxgl.Popup({className: 'my-class'});
    popup.marker = marker;

    popup.on('open', () => {
        var sensor = findByID(popup.marker.info);
        var status = (sensor.status < 1) ? "Inactive" : "Active";
        popup.setHTML('<div class="sensor-popup"><h5><b>' + sensor.name + '</b></h5><p><b>Status:</b> ' + status + '</p><p><b>Location:</b> ' + sensor.position.lat + ', ' + sensor.position.lng + '</p></div>');
        document.getElementById("mySidenav").style.width = "450px";
    })

    popup.on('close', () => {
        document.getElementById("mySidenav").style.width = "0px";
    })

    return popup;
}

function updateMarkers() {
    //document.getElementById("mySidenav").style.width = "250px";
    for (var marker of dict.markers){
    var stringVal = "blue";
    var stingVal = "blue";
    var found =  findByID(marker.info);
    if(found != null){
        switch (found.status) {
            case 0:
                stringVal = "#DCDCDC";
                stingVal = "#DCDCDC";
                break;
            case 1:
                stringVal = "#6495ED";
                stingVal = "#A4C0F4";
                break;
            case 2:
                stringVal = "#F08080";
                stingVal = "#FFB6C1";
                break;
            default:
                stringVal = "green";
                break;
        }
    }
    marker.getElement().style.setProperty('--sens-col', stringVal);
    marker.getElement().style.setProperty('--sens-bck', stingVal);           
    }
}

function findByID(id){
    for (var sens of dict.sensors){
        if (sens.id == id){
            return sens;
        }
    }
}


google.charts.load('current', {packages: ['corechart', 'line']});
google.charts.setOnLoadCallback(drawAxisTickColors);

function drawAxisTickColors() {
      var data = new google.visualization.DataTable();
      data.addColumn('number', 'X');
      data.addColumn('number', 'Acc');
      data.addColumn('number', 'Event Acc');

      data.addRows([
        [0, 0, 0],    [1, 10, 5],   [2, 23, 15],  [3, 17, 9],   [4, 18, 10],  [5, 9, 5],
        [6, 11, 3],   [7, 27, 19],  [8, 33, 25],  [9, 40, 32],  [10, 32, 24], [11, 35, 27],
        [12, 30, 22], [13, 40, 32], [14, 42, 34], [15, 47, 39], [16, 44, 36], [17, 48, 40],
        [18, 52, 44], [19, 54, 46], [20, 42, 34], [21, 55, 47], [22, 56, 48], [23, 57, 49],
        [24, 60, 52], [25, 50, 42], [26, 52, 44], [27, 51, 43], [28, 49, 41], [29, 53, 45],
        [30, 55, 47], [31, 60, 52], [32, 61, 53], [33, 59, 51], [34, 62, 54], [35, 65, 57],
        [36, 62, 54], [37, 58, 50], [38, 55, 47], [39, 61, 53], [40, 64, 56], [41, 65, 57],
        [42, 63, 55], [43, 66, 58], [44, 67, 59], [45, 69, 61], [46, 69, 61], [47, 70, 62],
        [48, 72, 64], [49, 68, 60], [50, 66, 58], [51, 65, 57], [52, 67, 59], [53, 70, 62],
        [54, 71, 63], [55, 72, 64], [56, 73, 65], [57, 75, 67], [58, 70, 62], [59, 68, 60],
        [60, 64, 56], [61, 60, 52], [62, 65, 57], [63, 67, 59], [64, 68, 60], [65, 69, 61],
        [66, 70, 62], [67, 72, 64], [68, 75, 67], [69, 80, 72]
      ]);

      var options = {
        hAxis: {
          title: 'Time',
          textStyle: {
            color: '#01579b',
            fontSize: 20,
            fontName: 'Arial',
            bold: true,
            italic: true
          },
          titleTextStyle: {
            color: '#01579b',
            fontSize: 16,
            fontName: 'Arial',
            bold: false,
            italic: true
          }
        },
        colors: ['#a52714', '#097138']
      };
      var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
      chart.draw(data, options);
    }