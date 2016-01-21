(function ($) {

    CreateGmap = function (sett) {
        var mapId = sett.canvas_settings.gmap_id;

        var options = {
            zoom: sett.zoom,
            zoomControl: true,
            zoomControlOptions: {
                style: google.maps.ZoomControlStyle.LARGE,
                position: google.maps.ControlPosition.LEFT_CENTER
            },
            mapTypeId: google.maps.MapTypeId[sett.maptype]
        }

        var map = new google.maps.Map(document.getElementById(mapId), options);

        //  Create a new viewpoint bound
        var bounds = new google.maps.LatLngBounds ();

        var markers = new Array();
        for (var i = 0; i < sett.markers.length; i++) {

            if(parseFloat(sett.markers[i].lat) || parseFloat(sett.markers[i].lng)) {
                var loc = new google.maps.LatLng(parseFloat(sett.markers[i].lat), parseFloat(sett.markers[i].lng))
                map.setCenter(loc);
                markers[i] = new google.maps.Marker({
                    map: map,
                    position: loc,
                    icon: sett.markers[i].icon,
                    opacity: sett.markers[i].opacity
                });
            }
            else {
                var geocoder = new google.maps.Geocoder();
                geocoder.geocode({'address': sett.address}, function (results, status) {
                    if (status == google.maps.GeocoderStatus.OK) {
                        var loc = results[0].geometry.location;
                        map.setCenter();
                        var marker = new google.maps.Marker({
                            map: map,
                            position: loc
                        });
                    }
                });
            }

            // Show infoWindow.
            if(typeof sett.markers[i].content != 'undefined') {
                markers[i].info = new google.maps.InfoWindow({
                    content: '' + sett.markers[i].content + '',
                    position: markers[i].getPosition(),
                    maxWidth: 230
                });
                google.maps.event.addListener(markers[i], 'click', function() {
                    this.info.open(map, markers[i]);
                });
            }

            bounds.extend (loc);
        }

        //  Center the map
        map.fitBounds (bounds);

        var markerCluster = new MarkerClusterer(map, markers, options);

    }

    Drupal.behaviors.realestateSimpleGMap = {
        attach: function (context, settings) {
            CreateGmap(settings.simplegmap);
        }
    }

})(jQuery);
