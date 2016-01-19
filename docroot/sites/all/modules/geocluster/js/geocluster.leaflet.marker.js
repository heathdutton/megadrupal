(function ($) {

  // Numbered Markers in Leaflet
  // based on https://gist.github.com/2288108

  L.NumberedDivIcon = L.Icon.extend({
    options: {
      iconUrl: Drupal.settings.basePath + 'sites/all/modules/geocluster/images/marker_hole.png',
      number: '',
      shadowUrl: null,
      iconSize: new L.Point(25, 41),
      iconAnchor: new L.Point(13, 41),
      popupAnchor: new L.Point(0, -33),
      /*
       iconAnchor: (Point)
       popupAnchor: (Point)
       */
      className: 'leaflet-div-icon'
    },

    createIcon: function () {
      this.options['iconUrl'] = Drupal.settings.basePath + 'sites/all/modules/geocluster/images/marker_hole.png';

      var div = document.createElement('div');
      var img = this._createImg(this.options['iconUrl']);
      var numdiv = document.createElement('div');
      numdiv.setAttribute ( "class", "number" );
      numdiv.innerHTML = this.options['number'] || '';
      div.appendChild ( img );
      div.appendChild ( numdiv );
      this._setIconStyles(div, 'icon');
      return div;
    },

    //you could change this to add a shadow like in the normal marker if you really wanted
    createShadow: function () {
      return null;
    }
  });

  Drupal.leaflet._old_create_point = Drupal.leaflet.create_point;

  Drupal.leaflet.create_point = function(marker) {
    // var point = Drupal.leaflet._old_create_point(marker);
    // point.options.icon.options.number = marker.cluster_items || 10;
    // return point;

    var latLng = new L.LatLng(marker.lat, marker.lon);
    this.bounds.push(latLng);
    var lMarker;

    if (marker.clustered) {
      marker.icon = Object();
      marker.icon.clustered = true;
    }

    if (marker.icon) {
      if (marker.icon.clustered) {
        // var icon = new L.NumberedDivIcon({number: marker.cluster_items || 1});

          var c = ' marker-cluster-';
          if (marker.cluster_items < 10) {
              c += 'small';
          } else if (marker.cluster_items < 100) {
              c += 'medium';
          } else {
              c += 'large';
          }
        var icon = new L.DivIcon({ html: '<div><span>' + marker.cluster_items + '</span></div>', className: 'marker-cluster' + c, iconSize: new L.Point(40, 40) });

      }
      else {
        var icon = new L.Icon({iconUrl: marker.icon.iconUrl});
      }
      // override applicable marker defaults
      if (marker.icon.iconSize) {
        icon.options.iconSize = new L.Point(parseInt(marker.icon.iconSize.x), parseInt(marker.icon.iconSize.y));
      }
      if (marker.icon.iconAnchor) {
        icon.options.iconAnchor = new L.Point(parseFloat(marker.icon.iconAnchor.x), parseFloat(marker.icon.iconAnchor.y));
      }
      if (marker.icon.popupAnchor) {
        icon.options.popupAnchor = new L.Point(parseFloat(marker.icon.popupAnchor.x), parseFloat(marker.icon.popupAnchor.y));
      }
      if (marker.icon.shadowUrl !== undefined) {
        icon.options.shadowUrl = marker.icon.shadowUrl;
      }
      if (marker.icon.shadowSize) {
        icon.options.shadowSize = new L.Point(parseInt(marker.icon.shadowSize.x), parseInt(marker.icon.shadowSize.y));
      }

      lMarker = new L.Marker(latLng, {icon:icon});
    }
    else {
      lMarker = new L.Marker(latLng);
    }
    return lMarker;
  }

})(jQuery);
