/**
 * The google_elevation.js is used inside the Drupal module Route Elevation.
 */

(function() {
    var elevator;
    var map;
    var chart;
    var infowindow = new google.maps.InfoWindow();
    var polyline;

    var map_center = new google.maps.LatLng(39, 9);
    var map_path = [];
    var map_zoom = 10;

// Warning: Initialize my_locs here doesn't work.
// var my_locs = Drupal.settings.osm_route_directions.my_locs;
// my_locs contains coordinates in form of an array [lat, lon, lat, lon, ...]
    var my_locs;
    var s_elevation_popup;
    var s_elevation_yaxis;
    var s_elevation_xaxis;

// Load the Visualization API and the columnchart package.
    google.load('visualization', '1', {packages: ['columnchart']});

    function initialize() {
        // Inflating coordinated from the GPX file passed from Drupal.
        my_locs = Drupal.settings.route_elevation.my_locs;
        s_elevation_popup = Drupal.settings.route_elevation.label_popup;
        s_elevation_yaxis = Drupal.settings.route_elevation.label_xaxis;
        s_elevation_xaxis = Drupal.settings.route_elevation.label_yaxis;
        map_center = Drupal.settings.route_elevation.map_center;
        map_center = new google.maps.LatLng(map_center[0], map_center[1]);
        map_zoom = Drupal.settings.route_elevation.map_zoom;
        for (var i = 0; i < my_locs.length; i += 2) {
            map_path.push(new google.maps.LatLng(my_locs[i], my_locs[i + 1]));
        }

        var mapOptions = {
            zoom: map_zoom,
            center: map_center,
            mapTypeId: 'terrain'
        }
        map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);

        // Create an ElevationService.
        elevator = new google.maps.ElevationService();

        // Draw the path, using the Visualization API and the Elevation service.
        drawPath();
    }

    function drawPath() {

        // Create a new chart in the elevation_chart DIV.
        chart = new google.visualization.ColumnChart(document.getElementById('elevation_chart'));

        var pathRequest = {
            'path': map_path,
            'samples': 512
        }

        // Initiate the path request.
        elevator.getElevationAlongPath(pathRequest, plotElevation);
    }

    /**
     * Round a float number to the nearest ten.
     * @param {type} number
     * @returns {Number}
     */
    function round10(number) {
        return Math.round(Math.round(number / 10) * 10);
    }

    /**
     * Takes an array of ElevationResult objects, draws the path on the map
     * and plots the elevation profile on a Visualization API ColumnChart.
     *
     * @param {type} results
     * @param {type} status
     * @returns {undefined}
     */
    function plotElevation(results, status) {
        if (status !== google.maps.ElevationStatus.OK) {
            return;
        }

        var elevations = results;
        var elevationPath = [];
        for (var i = 0; i < results.length; i++) {
            elevationPath.push(elevations[i].location);
        }

        // Display a polyline of the elevation path.
        var pathOptions = {
            path: elevationPath,
            strokeColor: '#0000CC',
            opacity: 0.4,
            map: map
        }
        polyline = new google.maps.Polyline(pathOptions);

        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Sample');
        data.addColumn('number', s_elevation_popup);

        // /-{ Hack to retrieve distance from a Drupal field instead of calculating,
        // to represent it in the X axis labels. --- REMOVE if no needs.
        //
        // var m_total = jQuery('.field-name-field-length .field-item').html();
        // var m_total_meters = parseFloat(m_total.substring(0, m_total.length - 2));
        // if (m_total_meters < 150) m_total_meters *= 1000;
        // /-}
        var old_el = new google.maps.LatLng(elevations[0].location.lat(), elevations[0].location.lng());
        var dist_total = 0;

        for (var i = 0; i < results.length; i++) {
            var new_el = new google.maps.LatLng(elevations[i].location.lat(), elevations[i].location.lng());
            dist_total += google.maps.geometry.spherical.computeDistanceBetween(old_el, new_el);
            // /-{ Hack to retrieve distance from a Drupal field instead of calculating,
            // to represent it in the X axis labels. --- REMOVE if no needs.
            // dist_total = m_total_meters / results.length * i;
            // /-}
            data.addRow([round10(dist_total) + '', Math.round(elevations[i].elevation)]);
            old_el = new_el;
        }
        //
        // Draw the chart using the data within its DIV.
        document.getElementById('elevation_chart').style.display = 'block';
        chart.draw(data, {
            height: 150,
            legend: 'none',
            titleX: s_elevation_xaxis,
            titleY: s_elevation_yaxis,
            hAxis: {showTextEvery: 100},
            colors: ['#006CAB', '#FF0000'],
        });
    }
    jQuery(document).ready(function() {
        google.maps.event.addDomListener(window, 'load', initialize);
    });
}());
