
/**
 * @file
 * Javascript functions for getdirections module
 *
 * @author Bob Hutchinson http://drupal.org/user/52366
 * code derived from gmap_direx module
 * this is for googlemaps API version 3
 * with adaptations from econym.org.uk
*/
(function ($) {
  Drupal.getdirections = {};

  Drupal.getdirections_map = [];

  function getdirections_init() {
    // each map has its own settings
    $.each(Drupal.settings.getdirections, function(key, settings) {

      // functions
      function handleState() {

        var e;
        var point;
        if (! todone) {
          e = $("input[name=to_" + key + "]").val();
          if (e && e.match(llpatt)) {
            arr = e.split(",");
            point = new google.maps.LatLng(arr[0], arr[1]);
            createMarker(point, endpoint, 'end');
            path[key][endpoint] = point;
            if (donemarkers[key][startpoint] == false) {
              Drupal.getdirections_map[key].panTo(path[key][endpoint]);
            }
            todone = true;
          }
        }

        if (! fromdone) {
          e = $("input[name=from_" + key + "]").val();
          if (e && e.match(llpatt)) {
            arr = e.split(",");
            point = new google.maps.LatLng(arr[0], arr[1]);
            createMarker(point, startpoint, 'start');
            path[key][startpoint] = point;
            if (donemarkers[key][endpoint] == false) {
              Drupal.getdirections_map[key].panTo(path[key][startpoint]);
            }
            fromdone = true;
          }
        }

        if (state == 0) {
          if (fromdone) {
            state = 1;
          }
          else {
            $("#getdirections_start_" + key).show();
            $("#getdirections_end_" + key).hide();
            $("#getdirections_btn_" + key).hide();
            $("#getdirections_help_" + key).hide();
            $("#autocomplete_via_wrapper_" + key).hide();
            //$("#getdirections_nextbtn_" + key).hide();
          }
        }
        if (state == 1) {
          if (todone) {
            state = 2;
          }
          else {
            $("#getdirections_start_" + key).hide();
            $("#getdirections_end_" + key).show();
            $("#getdirections_btn_" + key).hide();
            $("#getdirections_help_" + key).hide();
            $("#getdirections_nextbtn_" + key).show();
            if (addresses[key][startpoint]) {
              $("#getdirections_start_info_" + key).html('<b>' + Drupal.t('From') + '</b> ' + addresses[key][startpoint]);
            }
          }
        }
        if (state == 2) {
          if (todone) {
            dovias();
            setendbounds();
          }
          $("#getdirections_start_" + key).hide();
          $("#getdirections_end_" + key).hide();
          $("#getdirections_btn_" + key).show();
          $("#getdirections_nextbtn_" + key).hide();
          if (addresses[key][endpoint]) {
            $("#getdirections_end_info_" + key).html('<b>' + Drupal.t('To') + '</b> ' + addresses[key][endpoint]);
          }
          if (waypoints) {
            $("#getdirections_help_" + key).show();
            $("#autocomplete_via_wrapper_" + key).show();
          }
        }
      } // end handleState

      // t is type eg start, end, via or pause
      function createMarker(point, i, t) {
        // stop these from being recreated
        if ( (t == 'start' && donemarkers[key][startpoint] == true) || (t == 'end' && donemarkers[key][endpoint] == true)) {
          return;
        }

        var marker;
        if (t == 'via') {
          // via marker has special needs
          marker = new google.maps.Marker({
            position: point,
            map: Drupal.getdirections_map[key],
            title: 'Via ' +i,
            icon: icon4,
            shadow: shadow4,
            shape: shape4,
            draggable: true
          });
        }
        else {
          marker = new google.maps.Marker({
            position: point,
            map: Drupal.getdirections_map[key],
            title: (t == 'start' ? 'From' : (t == 'end' ? 'To'  : 'via ' + i)),
            icon:  (t == 'start' ? icon1  : (t == 'end' ? icon3 : icon2)),
            shadow: shadow1,
            shape: shape1,
            draggable: true
          });
        }

        gmarkers[key][i] = marker;
        google.maps.event.addListener(marker, "dragend", function() {
          path[key][i] = marker.getPosition();
          Drupal.getdirections_map[key].panTo(path[key][i]);
          if (! active[key][i]) {
            swapMarkers(i);
            active[key][i] = true;
          }
          addresses[key][i] = "";
        });

        marker.setMap(Drupal.getdirections_map[key]);

        // mark as done
        if (t == 'start') {
          donemarkers[key][startpoint] = true;
        }
        else if (t == 'end') {
          donemarkers[key][endpoint] = true;
        }
      } // end createMarker

      function swapMarkers(i) {
        gmarkers[key][i].setMap(null);
        createMarker(path[key][i], i, 'pause');
      }

      function doStart(point) {
        createMarker(point, startpoint, 'start');
        path[key][startpoint] = point;
        state = 1;
        handleState();
      }

      function doEnd(point) {
        createMarker(point, endpoint, 'end');
        path[key][endpoint] = point;
        state = 2;
        handleState();
        dovias();
        setendbounds();
        if (waypoints) {
          $("#getdirections_help_" + key).show();
        }
      }

      function dovias() {
        for (i = 1; i < endpoint; i++) {
          lat = (path[key][startpoint].lat()*(endpoint-i) + path[key][endpoint].lat()*i)/endpoint;
          lng = (path[key][startpoint].lng()*(endpoint-i) + path[key][endpoint].lng()*i)/endpoint;
          p = new google.maps.LatLng(lat,lng);
          createMarker(p, i, 'via');
          path[key][i] = p;

          if (settings.advanced_autocomplete_via) {
            if ($("#edit-via-autocomplete-" + key2 + '-' + i).is("input.form-text")) {
              auto = new google.maps.places.Autocomplete(document.getElementById('edit-via-autocomplete-' + key2 + '-' + i));
              if (settings.advanced_autocomplete_bias) {
                auto.bindTo('bounds', Drupal.getdirections_map[key]);
              }
              do_listen(i, auto);
            }
          }
        }
      }

      function do_listen(num, a) {
        google.maps.event.addListener(a, 'place_changed', function() {
          pl = a.getPlace();
          adrs = pl.formatted_address;
          $("#edit-via-autocomplete-" + key2 + '-' + num).val(adrs);
          geocoder.geocode({address: adrs}, function (results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
              // get the point
              p = results[0].geometry.location;
              if (p) {
                gmarkers[key][num].setPosition(p);
                path[key][num] = p;
                active[key][num] = true;
                swapMarkers(num);
              }
            }
          });
        });
      }

      function setendbounds() {
        bounds[key].extend(path[key][startpoint]);
        bounds[key].extend(path[key][endpoint]);
        Drupal.getdirections_map[key].fitBounds(bounds[key]);
      }

      // Geocoding
      function showAddress() {
        var s;
        if (state == 0) {
          if ($("#edit-from-" + key2).is('input')) {
            s = $("#edit-from-" + key2).val();
            if ($("#edit-country-from-" + key2).val()) {
              s += ', ' + $("#edit-country-from-" + key2).val();
            }
          }
          addresses[key][startpoint] = s;
        }
        if (state == 1) {
          if ($("#edit-to-" + key2).is('input')) {
            s = $("#edit-to-" + key2).val();
            if ($("#edit-country-to-" + key2).val()) {
              s += ', ' + $("#edit-country-to-" + key2).val();
            }
          }
          addresses[key][endpoint] = s;
        }
        var r = {address: s};
        geocoder = new google.maps.Geocoder();
        geocoder.geocode(r, function (results, status) {
          if (status == google.maps.GeocoderStatus.OK) {
            // get the point
            point = results[0].geometry.location;
            if (point) {
              if (state == 1) {
                doEnd(point);
              }
              if (state == 0) {
                doStart(point);
                if (! todone) {
                  Drupal.getdirections_map[key].panTo(point);
                }
              }
            }
          } else {
              alert(Drupal.t('Geocode for @address was not successful for the following reason: @reason', {'@address':s, '@reason':Drupal.getdirections.getgeoerrcode(status)}));
          }
        });
      }

      function doGeocode() {
        var statusdiv = '#getdirections_geolocation_status_from_' + key;
        var statusmsg = '';
        $(statusdiv).html(statusmsg);
        navigator.geolocation.getCurrentPosition(
          function(position) {
            lat = position.coords.latitude;
            lng = position.coords.longitude;
            point = new google.maps.LatLng(parseFloat(lat), parseFloat(lng));
            if (point && state == 0) {
              doStart(point);
              if (! todone) {
                Drupal.getdirections_map[key].panTo(point);
              }
              doRGeocode(point);
            }
          },
          function(error) {
            statusmsg = Drupal.t("Sorry, I couldn't find your location using the browser") + ' ' + Drupal.getdirections.getdirectionserrcode(error.code) + ".";
            $(statusdiv).html(statusmsg);
          }, {maximumAge:10000}
        );
      }
      // reverse geocoding
      function doRGeocode(pt) {
        var input_ll = {'latLng': pt};
        // Create a Client Geocoder
        var geocoder = new google.maps.Geocoder();
        geocoder.geocode(input_ll, function (results, status) {
          if (status == google.maps.GeocoderStatus.OK) {
            if (results[0]) {
              if ($("#edit-from-" + key2).is('input')) {
                $("#edit-from-" + key2).val(results[0].formatted_address);
              }
            }
          }
          else {
            var prm = {'!b': Drupal.getdirections.getgeoerrcode(status) };
            var msg = Drupal.t('Geocode was not successful for the following reason: !b', prm);
            alert(msg);
          }
        });
      }

      // Total distance and duration
      function computeTotals(result) {
        var meters = 0;
        var seconds = 0;
        var myroute = result.routes[0];
        for (i = 0; i < myroute.legs.length; i++) {
          meters += myroute.legs[i].distance.value;
          seconds += myroute.legs[i].duration.value;
        }

        if (settings.show_distance) {
          distance = meters * 0.001;
          if (unitsys == 'imperial') {
            distance = distance * 0.6214;
            distance = distance.toFixed(2) + ' ' + Drupal.t('Miles');
          }
          else {
            distance = distance.toFixed(2) + ' ' + Drupal.t('Kilometers');
          }
          $("#getdirections_show_distance_" + key).html(settings.show_distance + ': ' + distance);
        }

        if (settings.show_duration) {
          mins = seconds * 0.016666667;
          minutes = mins.toFixed(0);
          // hours
          hours = 0;
          while (minutes >= 60 ) {
            minutes = minutes - 60;
            hours++;
          }
          // days
          days = 0;
          while (hours >= 24) {
            hours = hours - 24;
            days++;
          }
          duration = '';
          if (days > 0) {
            duration += Drupal.formatPlural(days, '1 day', '@count days') + ' ';
          }
          if (hours > 0) {
            //duration += hours + ' ' + (hours > 1 ? 'hours' : 'hour') + ' ';
            duration += Drupal.formatPlural(hours, '1 hour', '@count hours') + ' ';
         }
          if (minutes > 0) {
            //duration += minutes + ' ' + (minutes > 1 ? 'minutes' : 'minute');
            duration += Drupal.formatPlural(minutes, '1 minute', '@count minutes');
         }
          if (seconds < 60) {
            duration = Drupal.t('About 1 minute');
          }
          $("#getdirections_show_duration_" + key).html(settings.show_duration + ': ' + duration );
        }
      }

      // convert lat,lon into LatLng object
      function makell(ll) {
        if (ll.match(llpatt)) {
          var arr = ll.split(",");
          var d = new google.maps.LatLng(parseFloat(arr[0]), parseFloat(arr[1]));
          return d;
        }
        return false;
      }

      function renderdirections(request) {
        dirservice.route(request, function(response, status) {
          if (status == google.maps.DirectionsStatus.OK) {
            dirrenderer.setDirections(response);
          } else {
            alert(Drupal.t('Error') + ': ' + Drupal.getdirections.getdirectionserrcode(status));
          }
        });
      }

      function getRequest(fromAddress, toAddress, waypts) {
        var trmode;
        var request = {
          origin: fromAddress,
          destination: toAddress
        };

        var tmode = $("#edit-travelmode-" + key2).val();
        if (tmode == 'walking') { trmode = google.maps.DirectionsTravelMode.WALKING; }
        else if (tmode == 'bicycling') { trmode = google.maps.DirectionsTravelMode.BICYCLING; }
        else if (tmode == 'transit') {
          trmode = google.maps.DirectionsTravelMode.TRANSIT;
          // transit dates
          if ($("#edit-transit-date-select-" + key2).is('select')) {
            d = $("#edit-transit-dates-" + key2 +"-datepicker-popup-0").val();
            t = $("#edit-transit-dates-" + key2 + "-timeEntry-popup-1").val();
            if (d && t) {
              if (settings.transit_date_format == 'int') {
                d1 = d.split(' ');
                dmy = d1[0];
                d2 = dmy.split('/');
                d3 = d2[1] +'/' + d2[0] + '/' + d2[2];
                s = d3 + ' ' + t;
              }
              else {
                s = d + ' ' + t;
              }
              dp = Date.parse(s);
              tstamp = new Date(dp);
              date_dir = $("#edit-transit-date-select-" + key2).val();
              tropts = '';
              if (date_dir == 'arrive') {
                tropts = {arrivalTime: tstamp};
              }
              else if (date_dir == 'depart') {
                tropts = {departureTime: tstamp};
              }
              if (tropts) {
                request.transitOptions = tropts;
              }
            }
          }
        }
        else { trmode = google.maps.DirectionsTravelMode.DRIVING; }
        request.travelMode = trmode;

        if (unitsys == 'imperial') { request.unitSystem = google.maps.DirectionsUnitSystem.IMPERIAL; }
        else { request.unitSystem = google.maps.DirectionsUnitSystem.METRIC; }

        var avoidh = false;
        if ($("#edit-travelextras-avoidhighways-" + key2).attr('checked')) { avoidh = true; }
        request.avoidHighways = avoidh;

        var avoidt = false;
        if ($("#edit-travelextras-avoidtolls-" + key2).attr('checked')) { avoidt = true; }
        request.avoidTolls = avoidt;

        var routealt = false;
        if ($("#edit-travelextras-altroute-" + key2).attr('checked')) { routealt = true; }
        request.provideRouteAlternatives = routealt;

        if (waypts) {
          request.waypoints = waypts;
          request.optimizeWaypoints = (settings.waypoints_optimise > 0 ? true : false);
        }

        return request;
      } // end getRequest

      // with waypoints
      function setDirectionsvia(lls) {
        var arr = lls.split('|');
        var len = arr.length;
        var wp = 0;
        var waypts = [];
        var from;
        var to;
        var via;
        for (var i = 0; i < len; i++) {
          if (i == 0) {
            from = makell(arr[i]);
          }
          else if (i == len-1) {
            to = makell(arr[i]);
          }
          else {
            wp++;
            if (wp < 24) {
              via = makell(arr[i]);
              waypts.push({
                location: via,
                stopover: true
              });
            }
          }
        }
        var request = getRequest(from, to, waypts);
        renderdirections(request);
      }

      function setDirectionsfromto(fromlatlon, tolatlon) {
        var from = makell(fromlatlon);
        var to = makell(tolatlon);
        var request = getRequest(from, to, '');
        renderdirections(request);
      }

      function updateCopyrights() {
        if(Drupal.getdirections_map[key].getMapTypeId() == "OSM") {
          copyrightNode.innerHTML = "OSM map data @<a target=\"_blank\" href=\"http://www.openstreetmap.org/\"> OpenStreetMap</a>-contributors,<a target=\"_blank\" href=\"http://creativecommons.org/licenses/by-sa/2.0/legalcode\"> CC BY-SA</a>";
          if (settings.trafficinfo) {
            $("#getdirections_toggleTraffic_" + key).attr('disabled', true);
          }
          if (settings.bicycleinfo) {
            $("#getdirections_toggleBicycle_" + key).attr('disabled', true);
          }
          if (settings.transitinfo) {
            $("#getdirections_toggleTransit_" + key).attr('disabled', true);
          }
        }
        else {
          copyrightNode.innerHTML = "";
          if (settings.trafficinfo) {
            $("#getdirections_toggleTraffic_" + key).attr('disabled', false);
          }
          if (settings.bicycleinfo) {
            $("#getdirections_toggleBicycle_" + key).attr('disabled', false);
          }
          if (settings.transitinfo) {
            $("#getdirections_toggleTransit_" + key).attr('disabled', false);
          }
        }
      }
      // end functions

      // is there really a map?
      if ($("#getdirections_map_canvas_" + key).is('div')) {

        var key2 = key.replace('_', '-');
        var trafficInfo = {};
        var traffictoggleState = [];    traffictoggleState[key] = true;
        var bicycleInfo = {};
        var bicycletoggleState = [];    bicycletoggleState[key] = true;
        var transitInfo = {};
        var transittoggleState = [];    transittoggleState[key] = true;
        var panoramioLayer = {};
        var panoramiotoggleState = [];  panoramiotoggleState[key] = true;
        var weatherLayer = {};
        var weathertoggleState = [];    weathertoggleState[key] = true;
        var cloudLayer = {};
        var cloudtoggleState = [];      cloudtoggleState[key] = true;
        var bounds = [];
        var path = [];                  path[key] = [];
        var gmarkers = [];              gmarkers[key] = [];
        var addresses = [];             addresses[key] = [];
        var active = [];                active[key] = [];
        var state = 0;
        var llpatt = /[0-9.\-],[0-9.\-]/;
        var fromdone = false;
        var todone = false;
        var dirservice;
        var dirrenderer;

        var lat = parseFloat(settings.lat);
        var lng = parseFloat(settings.lng);
        var selzoom = parseInt(settings.zoom);
        var controltype = settings.controltype;
        var pancontrol = settings.pancontrol;
        var scale = settings.scale;
        var overview = settings.overview;
        var overview_opened = settings.overview_opened;
        var streetview_show = settings.streetview_show;
        var scrollw = settings.scrollwheel;
        var drag = settings.draggable;
        var unitsys = settings.unitsystem;
        var maptype = (settings.maptype ? settings.maptype : '');
        var baselayers = (settings.baselayers ? settings.baselayers : '');
        var map_backgroundcolor = settings.map_backgroundcolor;
        var fromlatlon = (settings.fromlatlon ? settings.fromlatlon : '');
        var tolatlon = (settings.tolatlon ? settings.tolatlon : '');
        var useOpenStreetMap = false;
        var scheme = 'http';
        if (settings.use_https) {
          scheme = 'https';
        }
        var startIconUrl = scheme + "://www.google.com/mapfiles/dd-start.png";
        var endIconUrl = scheme + "://www.google.com/mapfiles/dd-end.png";
        var shadowIconUrl = scheme + "://www.google.com/mapfiles/shadow50.png";
        var pauseIconUrl = scheme + "://www.google.com/mapfiles/dd-pause.png";
        var viaIconUrl = scheme + "://labs.google.com/ridefinder/images/mm_20_";
        var viaIconColor = (settings.waypoint_color ? settings.waypoint_color : 'white');
        // pipe delim
        var latlons = (settings.latlons ? settings.latlons : '');
        var nokeyboard = (settings.nokeyboard ? true : false);
        var nodoubleclickzoom = (settings.nodoubleclickzoom ? true : false);

        var startpoint = 0;
        var endpoint = 1;
        var donemarkers = [];
        donemarkers[key] = [];

        var waypoints = (settings.waypoints ? settings.waypoints : 0);
        if (waypoints) {
          endpoint = waypoints + 1;
          active[key][startpoint] = true;
          donemarkers[key][startpoint] = false;
          for (var i = 1; i <= waypoints; i++) {
            active[key][i] = false;
            donemarkers[key][i] = false;
          }
          active[key][endpoint] = true;
          donemarkers[key][endpoint] = false;
        }
        else {
          active[key][startpoint] = true;
          active[key][endpoint] = true;
          donemarkers[key][startpoint] = false;
          donemarkers[key][endpoint] = false;
        }

        // menu type
        var mtc = settings.mtc;
        if (mtc == 'standard') { mtc = google.maps.MapTypeControlStyle.HORIZONTAL_BAR; }
        else if (mtc == 'menu' ) { mtc = google.maps.MapTypeControlStyle.DROPDOWN_MENU; }
        else { mtc = false; }
        // nav control type
        if (controltype == 'default') { controltype = google.maps.ZoomControlStyle.DEFAULT; }
        else if (controltype == 'small') { controltype = google.maps.ZoomControlStyle.SMALL; }
        else if (controltype == 'large') { controltype = google.maps.ZoomControlStyle.LARGE; }
        else { controltype = false; }
        // map type
        maptypes = [];
        if (maptype) {
          if (maptype == 'Map' && baselayers.Map) { maptype = google.maps.MapTypeId.ROADMAP; }
          if (maptype == 'Satellite' && baselayers.Satellite) { maptype = google.maps.MapTypeId.SATELLITE; }
          if (maptype == 'Hybrid' && baselayers.Hybrid) { maptype = google.maps.MapTypeId.HYBRID; }
          if (maptype == 'Physical' && baselayers.Physical) { maptype = google.maps.MapTypeId.TERRAIN; }
          if (baselayers.Map) { maptypes.push(google.maps.MapTypeId.ROADMAP); }
          if (baselayers.Satellite) { maptypes.push(google.maps.MapTypeId.SATELLITE); }
          if (baselayers.Hybrid) { maptypes.push(google.maps.MapTypeId.HYBRID); }
          if (baselayers.Physical) { maptypes.push(google.maps.MapTypeId.TERRAIN); }
          if (baselayers.OpenStreetMap) {
            maptypes.push("OSM");
            var copyrightNode = document.createElement('div');
            copyrightNode.id = 'copyright-control';
            copyrightNode.style.fontSize = '11px';
            copyrightNode.style.fontFamily = 'Arial, sans-serif';
            copyrightNode.style.margin = '0 2px 2px 0';
            copyrightNode.style.whiteSpace = 'nowrap';
            useOpenStreetMap = true;
          }
        }
        else {
          maptype = google.maps.MapTypeId.ROADMAP;
          maptypes.push(google.maps.MapTypeId.ROADMAP);
          maptypes.push(google.maps.MapTypeId.SATELLITE);
          maptypes.push(google.maps.MapTypeId.HYBRID);
          maptypes.push(google.maps.MapTypeId.TERRAIN);
        }

        var mapOpts = {
          zoom: selzoom,
          center: new google.maps.LatLng(lat, lng),
          mapTypeControl: (mtc ? true : false),
          mapTypeControlOptions: {style: mtc,  mapTypeIds: maptypes},
          zoomControl: (controltype ? true : false),
          zoomControlOptions: {style: controltype},
          panControl: (pancontrol ? true : false),
          mapTypeId: maptype,
          scrollwheel: (scrollw ? true : false),
          draggable: (drag ? true : false),
          overviewMapControl: (overview ? true : false),
          overviewMapControlOptions: {opened: (overview_opened ? true : false)},
          streetViewControl: (streetview_show ? true : false),
          scaleControl: (scale ? true : false),
          scaleControlOptions: {style: google.maps.ScaleControlStyle.DEFAULT},
          keyboardShortcuts: (nokeyboard ? false : true),
          disableDoubleClickZoom: nodoubleclickzoom
        };
        if (map_backgroundcolor) {
          mapOpts.backgroundColor = map_backgroundcolor;
        }

        Drupal.getdirections_map[key] = new google.maps.Map(document.getElementById("getdirections_map_canvas_" + key), mapOpts);

        // OpenStreetMap
        if (useOpenStreetMap) {
          var tle = Drupal.t("OpenStreetMap");
          if (settings.mtc == 'menu') {
            tle = Drupal.t("OSM map");
          }
          Drupal.getdirections_map[key].mapTypes.set("OSM", new google.maps.ImageMapType({
            getTileUrl: function(coord, zoom) {
              return "http://tile.openstreetmap.org/" + zoom + "/" + coord.x + "/" + coord.y + ".png";
            },
            tileSize: new google.maps.Size(256, 256),
            name: tle,
            maxZoom: 18
          }));
          google.maps.event.addListener(Drupal.getdirections_map[key], 'maptypeid_changed', updateCopyrights);
          Drupal.getdirections_map[key].controls[google.maps.ControlPosition.BOTTOM_RIGHT].push(copyrightNode);
        }

        // TrafficLayer
        if (settings.trafficinfo) {
          trafficInfo[key] = new google.maps.TrafficLayer();
          if (settings.trafficinfo_state > 0) {
            trafficInfo[key].setMap(Drupal.getdirections_map[key]);
            traffictoggleState[key] = true;
          }
          else {
            trafficInfo[key].setMap(null);
            traffictoggleState[key] = false;
          }
          $("#getdirections_toggleTraffic_" + key).click( function() {
            if (traffictoggleState[key]) {
              trafficInfo[key].setMap();
              traffictoggleState[key] = false;
              label = Drupal.t('Traffic Info On');
            }
            else {
              trafficInfo[key].setMap(Drupal.getdirections_map[key]);
              traffictoggleState[key] = true;
              label = Drupal.t('Traffic Info Off');
            }
            $(this).val(label);
          });
        }
        // BicyclingLayer
        if (settings.bicycleinfo) {
          bicycleInfo[key] = new google.maps.BicyclingLayer();
          if (settings.bicycleinfo_state > 0) {
            bicycleInfo[key].setMap(Drupal.getdirections_map[key]);
            bicycletoggleState[key] = true;
          }
          else {
            bicycleInfo[key].setMap(null);
            bicycletoggleState[key] = false;
          }
          $("#getdirections_toggleBicycle_" + key).click( function() {
            if (bicycletoggleState[key]) {
              bicycleInfo[key].setMap();
              bicycletoggleState[key] = false;
              label = Drupal.t('Bicycle Info On');
            }
            else {
              bicycleInfo[key].setMap(Drupal.getdirections_map[key]);
              bicycletoggleState[key] = true;
              label = Drupal.t('Bicycle Info Off');
            }
            $(this).val(label);
          });
        }
        // TransitLayer
        if (settings.transitinfo) {
          transitInfo[key] = new google.maps.TransitLayer();
          if (settings.transitinfo_state > 0) {
            transitInfo[key].setMap(Drupal.getdirections_map[key]);
            transittoggleState[key] = true;
          }
          else {
            transitInfo[key].setMap(null);
            transittoggleState[key] = false;
          }
          $("#getdirections_toggleTransit_" + key).click( function() {
            if (transittoggleState[key]) {
              transitInfo[key].setMap();
              transittoggleState[key] = false;
              label = Drupal.t('Transit Info On');
            }
            else {
              transitInfo[key].setMap(Drupal.getdirections_map[key]);
              transittoggleState[key] = true;
              label = Drupal.t('Transit Info Off');
            }
            $(this).val(label);
          });
        }
        // PanoramioLayer
        if (settings.panoramio_show) {
          panoramioLayer[key] = new google.maps.panoramio.PanoramioLayer();
          if (settings.panoramio_state > 0) {
            panoramioLayer[key].setMap(Drupal.getdirections_map[key]);
            panoramiotoggleState[key] = true;
          }
          else {
            panoramioLayer[key].setMap(null);
            panoramiotoggleState[key] = false;
          }
          $("#getdirections_togglePanoramio_" + key).click( function() {
            if (panoramiotoggleState[key]) {
              panoramioLayer[key].setMap();
              panoramiotoggleState[key] = false;
              label = Drupal.t('Panoramio On');
            }
            else {
              panoramioLayer[key].setMap(Drupal.getdirections_map[key]);
              panoramiotoggleState[key] = true;
              label = Drupal.t('Panoramio Off');
            }
            $(this).val(label);
          });
        }

        // weather layer
        if (settings.weather_use ) {
          if (settings.weather_show) {
            tu = google.maps.weather.TemperatureUnit.CELSIUS;
            if (settings.weather_temp == 2) {
              tu = google.maps.weather.TemperatureUnit.FAHRENHEIT;
            }
            sp = google.maps.weather.WindSpeedUnit.KILOMETERS_PER_HOUR;
            if (settings.weather_speed == 2) {
              sp = google.maps.weather.WindSpeedUnit.METERS_PER_SECOND;
            }
            else if (settings.weather_speed == 3) {
              sp = google.maps.weather.WindSpeedUnit.MILES_PER_HOUR;
            }
            var weatherOpts =  {
              temperatureUnits: tu,
              windSpeedUnits: sp,
              clickable: (settings.weather_clickable ? true : false),
              suppressInfoWindows: (settings.weather_info ? false : true)
            };
            if (settings.weather_label > 0) {
              weatherOpts.labelColor = google.maps.weather.LabelColor.BLACK;
              if (settings.weather_label == 2) {
                weatherOpts.labelColor = google.maps.weather.LabelColor.WHITE;
              }
            }
            weatherLayer[key] = new google.maps.weather.WeatherLayer(weatherOpts);
            if (settings.weather_state > 0) {
              weatherLayer[key].setMap(Drupal.getdirections_map[key]);
              weathertoggleState[key] = true;
            }
            else {
              weatherLayer[key].setMap(null);
              weathertoggleState[key] = false;
            }
            $("#getdirections_toggleWeather_" + key).click( function() {
              if (weathertoggleState[key]) {
                weatherLayer[key].setMap(null);
                weathertoggleState[key] = false;
                label = Drupal.t('Weather On');
              }
              else {
                weatherLayer[key].setMap(Drupal.getdirections_map[key]);
                weathertoggleState[key] = true;
                label = Drupal.t('Weather Off');
              }
              $(this).val(label);
            });
          }
          else {
            weatherLayer[key] = null;
          }
          if (settings.weather_cloud) {
            cloudLayer[key] = new google.maps.weather.CloudLayer();
            if (settings.weather_cloud_state > 0) {
              cloudLayer[key].setMap(Drupal.getdirections_map[key]);
              cloudtoggleState[key] = true;
            }
            else {
              cloudLayer[key].setMap(null);
              cloudtoggleState[key] = false;
            }
            $("#getdirections_toggleCloud_" + key).click( function() {
              if (cloudtoggleState[key]) {
                cloudLayer[key].setMap(null);
                cloudtoggleState[key] = false;
                label = Drupal.t('Clouds On');
              }
              else {
                cloudLayer[key].setMap(Drupal.getdirections_map[key]);
                cloudtoggleState[key] = true;
                label = Drupal.t('Clouds Off');
              }
            $(this).val(label);
            });
          }
          else {
            cloudLayer[key] = null;
          }
        }

        google.maps.event.addListener(Drupal.getdirections_map[key], 'click', function(event) {
          if (event.latLng) {
            point = new google.maps.LatLng(event.latLng.lat(), event.latLng.lng());
            if (state == 1) {
              doEnd(point);
            }
            if (state == 0) {
              doStart(point);
              if (! todone) {
                Drupal.getdirections_map[key].panTo(point);
              }
            }
          }
        });

        // define some icons
        var icon1 = new google.maps.MarkerImage(
          startIconUrl,
          new google.maps.Size(22, 34),
          // origin
          new google.maps.Point(0,0),
          // anchor
          new google.maps.Point(6, 20)
        );
        var icon2 = new google.maps.MarkerImage(
          pauseIconUrl,
          new google.maps.Size(22, 34),
          // origin
          new google.maps.Point(0,0),
          // anchor
          new google.maps.Point(6, 20)
        );
        var icon3 = new google.maps.MarkerImage(
          endIconUrl,
          new google.maps.Size(22, 34),
          // origin
          new google.maps.Point(0,0),
          // anchor
          new google.maps.Point(6, 20)
        );
        var shadow1 = new google.maps.MarkerImage(
          shadowIconUrl,
          new google.maps.Size(37, 34),
          // origin
          new google.maps.Point(0,0),
          // anchor
          new google.maps.Point(6, 20)
        );
        var shape1 = {coord: [1,1,22,34], type: 'rect'};
        var icon4 = new google.maps.MarkerImage(
          viaIconUrl + viaIconColor + '.png',
          new google.maps.Size(12, 20),
          // origin
          new google.maps.Point(0,0),
          // anchor
          new google.maps.Point(6, 20)
        );
        var shadow4 = new google.maps.MarkerImage(
          viaIconUrl + "shadow.png",
          new google.maps.Size(22, 20),
          // origin
          new google.maps.Point(0,0),
          // anchor
          new google.maps.Point(6, 20)
        );
        var shape4 = {coord: [1,1,12,20], type: 'rect'};

        dirrenderer = new google.maps.DirectionsRenderer();
        dirrenderer.setMap(Drupal.getdirections_map[key]);
        dirrenderer.setPanel(document.getElementById("getdirections_directions_" + key));

        if (settings.show_distance || settings.show_duration) {
          google.maps.event.addListener(dirrenderer, 'directions_changed', function() {
            computeTotals(dirrenderer.directions);
          });
        }

        dirservice = new google.maps.DirectionsService();

        // Bounding
        bounds[key] = new google.maps.LatLngBounds();

        handleState();

        // any initial markers?
        var vf =  $("input[name=from_" + key + "]").val();
        if (vf && vf.match(llpatt)) {
          // we have lat,lon
          vf = makell(vf);
          createMarker(vf, startpoint, 'start');
          if ( donemarkers[key][endpoint] == false) {
            Drupal.getdirections_map[key].setCenter(vf);
          }
        }
        var vt =  $("input[name=to_" + key + "]").val();
        if (vt && vt.match(llpatt)) {
          // we have lat,lon
          vt = makell(vt);
          createMarker(vt, endpoint, 'end');
          if ( donemarkers[key][startpoint] == false) {
            Drupal.getdirections_map[key].setCenter(vt);
          }
        }

        if (fromlatlon && tolatlon) {
          setDirectionsfromto(fromlatlon, tolatlon);
        }

        if (latlons) {
          setDirectionsvia(latlons);
        }

        // minding textfields
        if (settings.advanced_autocomplete) {
          if ($("#edit-from-" + key2).is("input.form-text")) {
            var input_from = document.getElementById('edit-from-' + key2);
            var ac_from = new google.maps.places.Autocomplete(input_from);
            if (settings.advanced_autocomplete_bias) {
              ac_from.bindTo('bounds', Drupal.getdirections_map[key]);
            }
            google.maps.event.addListener(ac_from, 'place_changed', function() {
              var place_from = ac_from.getPlace();
              $("#edit-from-" + key2).val(place_from.formatted_address);
              showAddress();
            });
          }
          if ($("#edit-to-" + key2).is("input.form-text")) {
            var input_to = document.getElementById('edit-to-' + key2);
            var ac_to = new google.maps.places.Autocomplete(input_to);
            if (settings.advanced_autocomplete_bias) {
              ac_to.bindTo('bounds', Drupal.getdirections_map[key]);
            }
            google.maps.event.addListener(ac_to, 'place_changed', function() {
              var place_to = ac_to.getPlace();
              $("#edit-to-" + key2).val(place_to.formatted_address);
              showAddress();
            });
          }
        }
        else {
          $("#edit-from-" + key2).change( function() {
            showAddress();
          });
          $("#edit-to-" + key2).change( function() {
            showAddress();
          });
        }

        // transit dates
        if ($("#edit-travelmode-" + key2).is('select') && $("#edit-transit-date-select-" +key2).is('select')) {
          if ( $("#edit-travelmode-" + key2).val() == 'transit') {
            $("#getdirections_transit_dates_wrapper_" + key).show();
          }
          else {
            $("#getdirections_transit_dates_wrapper_" + key).hide();
          }
          $("#edit-travelmode-" + key2).change( function() {
            if ( $("#edit-travelmode-" + key2).val() == 'transit') {
              $("#getdirections_transit_dates_wrapper_" + key).show();
            }
            else {
              $("#getdirections_transit_dates_wrapper_" + key).hide();
            }
          });
        }

        if (settings.geolocation_enable) {
          if (settings.geolocation_option == '1') {
            // html5
            if (navigator && navigator.geolocation) {
              $("#getdirections_geolocation_button_from_" + key).click( function () {
                doGeocode();
                return false;
              });
            }
          }
          else if (settings.geolocation_option == '2') {
            // smart_ip
            $("#getdirections_geolocation_button_from_" + key).click( function () {
              $.get(settings.smartip_callback_url, {}, function (loc) {
                if (loc) {
                  lat = loc.latitude;
                  lng = loc.longitude;
                  if (lat && lng) {
                    point = new google.maps.LatLng(parseFloat(lat), parseFloat(lng));
                    doStart(point);
                    if (! todone) {
                      Drupal.getdirections_map[key].panTo(point);
                    }
                    var adrs = '';
                    var adrsarr = [];
                    if (loc.route) {
                      adrsarr.push(loc.route);
                    }
                    if (loc.sublocality) {
                      adrsarr.push(loc.sublocality);
                    }
                    if (loc.locality) {
                      adrsarr.push(loc.locality);
                    }
                    if (loc.city) {
                      adrsarr.push(loc.city);
                    }
                    else if (loc.postal_town) {
                      adrsarr.push(loc.postal_town);
                    }
                    if (loc.region) {
                      adrsarr.push(loc.region);
                    }
                    if (loc.zip && loc.zip != '-') {
                      adrsarr.push(loc.zip);
                    }
                    else if (loc.postal_code_prefix) {
                      adrsarr.push(loc.postal_code_prefix);
                    }
                    if (loc.country) {
                      adrsarr.push(loc.country);
                    }
                    else if (loc.country_code) {
                      if (loc.country_code == 'UK') {
                        cc = 'GB';
                      }
                      else {
                        cc = loc.country_code;
                      }
                      adrsarr.push(cc);
                    }
                    adrs = adrsarr.join(', ');
                    if ($("#edit-from-" + key2).is("input.form-text")) {
                      if ($("#edit-from-" + key2).val() == '') {
                        $("#edit-from-" + key2).val(adrs);
                      }
                    }
                  }
                }
              });
              return false;
            });
          }
          else if (settings.geolocation_option == '3') {
            $("#getdirections_geolocation_button_from_" + key).click( function () {
              $.get(settings.ip_geoloc_callback_url, {}, function (loc) {
                if (loc) {
                  lat = loc.latitude;
                  lng = loc.longitude;
                  if (lat && lng) {
                    point = new google.maps.LatLng(parseFloat(lat), parseFloat(lng));
                    doStart(point);
                    if (! todone) {
                      Drupal.getdirections_map[key].panTo(point);
                    }
                    var adrs = '';
                    if (loc.formatted_address) {
                      adrs = loc.formatted_address;
                    }
                    if ($("#edit-from-" + key2).is("input.form-text")) {
                      if ($("#edit-from-" + key2).val() == '') {
                        $("#edit-from-" + key2).val(adrs);
                      }
                    }
                  }
                }
              });
              return false;
            });
          }
        }

        // get directions button
        $("#edit-submit-" + key2).click( function () {
          var from;
          var to;
          var i;
          var waypts = [];
          if (addresses[key][startpoint]) {
            from = addresses[key][startpoint] + "@" + path[key][startpoint].toUrlValue(6);
          }
          else {
            var from_address = $("input[name=from_address_" + key +"]").val();
            if (from_address && from_address.match(/,/)) {
              from = from_address + "@" + path[key][startpoint].toUrlValue(6);
            }
            else {
              from = path[key][startpoint].toUrlValue(6);
            }
          }
          if (addresses[key][endpoint]) {
            to = addresses[key][endpoint] + "@" + path[key][endpoint].toUrlValue(6);
          }
          else {
            var to_address = $("input[name=to_address_" + key +"]").val();
            if (to_address && to_address.match(/,/)) {
              to = to_address + "@" + path[key][endpoint].toUrlValue(6);
            }
            else {
              to = path[key][endpoint].toUrlValue(6);
            }
          }
          for (i = waypoints; i > 0; i--) {
            if (active[key][i]) {
              waypts.push({
                location: path[key][i].toUrlValue(6),
                stopover: true
              });
              gmarkers[key][i].setMap(null);
            }
          }
          // remove the moveable markers
          if (gmarkers[key][startpoint].getVisible()) {
            gmarkers[key][startpoint].setMap(null);
          }
          if (gmarkers[key][endpoint].getVisible()) {
            gmarkers[key][endpoint].setMap(null);
          }

          var request = getRequest(from, to, waypts);
          renderdirections(request);

          // disable to and from boxes
          $("#edit-from-" + key2).attr('disabled', true);
          $("#edit-to-" + key2).attr('disabled', true);
          return false;
        });

      }
    }); // end each
  } // end init

  Drupal.getdirections.nextbtn = function() {
    return;
  };

  // error codes
  Drupal.getdirections.getdirectionserrcode = function(errcode) {
    var errstr;
    if (errcode == google.maps.DirectionsStatus.INVALID_REQUEST) {
      errstr = Drupal.t("The DirectionsRequest provided was invalid.");
    }
    else if (errcode == google.maps.DirectionsStatus.MAX_WAYPOINTS_EXCEEDED) {
      errstr = Drupal.t("Too many DirectionsWaypoints were provided in the DirectionsRequest. The total allowed waypoints is 8, plus the origin, and destination.");
    }
    else if (errcode == google.maps.DirectionsStatus.NOT_FOUND) {
      errstr = Drupal.t("At least one of the origin, destination, or waypoints could not be geocoded.");
    }
    else if (errcode == google.maps.DirectionsStatus.OVER_QUERY_LIMIT) {
      errstr = Drupal.t("The webpage has gone over the requests limit in too short a period of time.");
    }
    else if (errcode == google.maps.DirectionsStatus.REQUEST_DENIED) {
      errstr = Drupal.t("The webpage is not allowed to use the directions service.");
    }
    else if (errcode == google.maps.DirectionsStatus.UNKNOWN_ERROR) {
      errstr = Drupal.t("A directions request could not be processed due to a server error. The request may succeed if you try again.");
    }
    else if (errcode == google.maps.DirectionsStatus.ZERO_RESULTS) {
      errstr = Drupal.t("No route could be found between the origin and destination.");
    }
    return errstr;
  }

  Drupal.getdirections.getgeoerrcode = function(errcode) {
    var errstr;
    if (errcode == google.maps.GeocoderStatus.ERROR) {
      errstr = Drupal.t("There was a problem contacting the Google servers.");
    }
    else if (errcode == google.maps.GeocoderStatus.INVALID_REQUEST) {
      errstr = Drupal.t("This GeocoderRequest was invalid.");
    }
    else if (errcode == google.maps.GeocoderStatus.OVER_QUERY_LIMIT) {
      errstr = Drupal.t("The webpage has gone over the requests limit in too short a period of time.");
    }
    else if (errcode == google.maps.GeocoderStatus.REQUEST_DENIED) {
      errstr = Drupal.t("The webpage is not allowed to use the geocoder.");
    }
    else if (errcode == google.maps.GeocoderStatus.UNKNOWN_ERROR) {
      errstr = Drupal.t("A geocoding request could not be processed due to a server error. The request may succeed if you try again.");
    }
    else if (errcode == google.maps.GeocoderStatus.ZERO_RESULTS) {
      errstr = Drupal.t("No result was found for this GeocoderRequest.");
    }
    return errstr;
  }

  // gogogo
  Drupal.behaviors.getdirections = {
    attach: function() {
      getdirections_init();
    }
  };

}(jQuery));
