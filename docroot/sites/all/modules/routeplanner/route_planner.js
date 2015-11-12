(function ($) {
    Drupal.behaviors.routeplanner = {
        attach: function (context, settings) {
            $('body', context).once('routeplanner', function () {
                if (!Drupal.myRoutePlanner) {
                    Drupal.myRoutePlanner = new Drupal.RoutePlanner();
                }
            });
        }
    };

    Drupal.RoutePlanner = function () {
        this.directionsService = new google.maps.DirectionsService();
        this.map = null;

        // initialize the map
        this.route();
    }

    Drupal.RoutePlanner.prototype.route = function () {
        directionsDisplay = new google.maps.DirectionsRenderer();
        geocoder = new google.maps.Geocoder();
        var latLng;
        var stylearray = Drupal.settings.routePlanner['style'];
        var mapstyle = [];
        if (stylearray) {
            try {
                mapstyle = eval(stylearray);
            } catch (e) {
                if (e instanceof SyntaxError) {
                    console.log(e.message);
                    // Error on parsing string. Using default.
                    mapstyle = [];
                }
            }
        }
        if (geocoder) {
            var end = Drupal.settings.routePlanner['end'];
            if (document.getElementById("edit-end")) {
                var end = document.getElementById("edit-end").value;
            }
            geocoder.geocode({ "address": end}, function (results, status) {
                if (status == google.maps.GeocoderStatus.OK) {
                    latLng = new String(results[0].geometry.location);
                    latLng = latLng.substr(1, (latLng.length - 2));
                    latLng = latLng.split(",");
                    var location = new google.maps.LatLng(latLng[0], latLng[1]);
                    var myOptions = {
                        zoom: Number(Drupal.settings.routePlanner['zoomlevel']),
                        scrollwheel: Drupal.settings.routePlanner['scrollwheel'],
                        mapTypeControl: Drupal.settings.routePlanner['mapTypeControl'],
                        scaleControl: Drupal.settings.routePlanner['scaleControl'],
                        draggable: Drupal.settings.routePlanner['draggable'],
                        zoomControl: Drupal.settings.routePlanner['zoomcontrol'],
                        disableDoubleClickZoom: Drupal.settings.routePlanner['doubbleclick'],
                        streetViewControl: Drupal.settings.routePlanner['streetviewcontrol'],
                        overviewMapControl: Drupal.settings.routePlanner['overviewmapcontrol'],
                        disableDefaultUI: Drupal.settings.routePlanner['defaultui'],
                        mapTypeId: google.maps.MapTypeId.ROADMAP,
                        center: location,
                        styles: mapstyle
                    }
                    this.map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
                    var marker = new google.maps.Marker({
                        map: this.map,
                        position: results[0].geometry.location
                    });
                    directionsDisplay.setMap(this.map);
                }
                else {
                    alert(Drupal.t("Geocode was not successful for the following reason: ") + status);
                }
            });
        }
        return false;
    }

    Drupal.RoutePlanner.prototype.calcRoute = function () {
        var start = document.getElementById("edit-start").value;
        var end = Drupal.settings.routePlanner['end'];
        if (document.getElementById("edit-end")) {
            var end = document.getElementById("edit-end").value;
        }
        if (Number(Drupal.settings.routePlanner['unitSystem']) == 1) {
            var unit = google.maps.UnitSystem.IMPERIAL;
        } else {
            var unit = google.maps.UnitSystem.METRIC;
        }

        var tMode = google.maps.DirectionsTravelMode.DRIVING;
        if(Number(Drupal.settings.routePlanner['travelMode']) == 1){
          var radios = document.getElementsByName('travel_mode');
          for (var i = 0, length = radios.length; i < length; i++) {
            if (radios[i].checked) {
              switch (Number(radios[i].value)) {
                case 0:
                  tMode = google.maps.DirectionsTravelMode.DRIVING;
                  break;
                case 1:
                  tMode = google.maps.DirectionsTravelMode.BICYCLING;
                  break;
                case 2:
                  tMode = google.maps.DirectionsTravelMode.TRANSIT;
                  break;
                case 3:
                  tMode = google.maps.DirectionsTravelMode.WALKING;
                  break;
              }
              break;
            }
          }
        }

        var request = {
            origin: start,
            destination: end,
            travelMode: tMode,
            unitSystem: unit
        };
        this.directionsService.route(request, function (response, status) {
            if (status == google.maps.DirectionsStatus.OK) {
                this.directionsDisplay.setDirections(response);
                distance = response.routes[0].legs[0].distance.text;
                time = response.routes[0].legs[0].duration.text;
                document.getElementById("edit-time").value = time;
                document.getElementById("edit-distance").value = distance;
            }
        });
        return false;
    }

})(jQuery);
