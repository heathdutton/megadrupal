(function ($) {

  Drupal.behaviors.geocluster_test = {

    onMapLoad: function(event) {
      var map = this;
      Drupal.behaviors.geocluster_test.map = map;

      Drupal.behaviors.geocluster_test.hashGroup = new L.LayerGroup();
      Drupal.behaviors.geocluster_test.hashGroup.addTo(map);

      map.on('moveend', Drupal.behaviors.geocluster_test.moveEnd);
      Drupal.behaviors.geocluster_test.moveEnd();
    },

    moveEnd: function(e) {
      var map = Drupal.behaviors.geocluster_test.map;
      Drupal.behaviors.geocluster_test.makeHashGrid(map);
    },

    makeHashGrid: function(map) {
      Drupal.behaviors.geocluster_test.hashGroup.clearLayers();

      var center = map.getCenter();
      var bounds = map.getBounds();

      var sw = bounds.getSouthWest();
      var ne = bounds.getNorthEast();

      var height = ne.lat - sw.lat;
      var width = ne.lng - sw.lng;

      var length = Math.max(1, Geohash.get_key_length(height, width));


      var hash = Geohash.get_key(center.lat, center.lng, length);
      var bounds_geohash = Geohash.bbox(hash);

      Drupal.behaviors.geocluster_test.drawHashRect(hash, map, "#00ff00", 0.1);

      length += 1;
      console.log("length: " + length);
      var hash = Geohash.get_key(center.lat, center.lng, length);

      // zoom the map to the rectangle bounds
      // map.fitBounds(bounds);

      var hash = Geohash.get_key(center.lat, center.lng, length);

      Drupal.behaviors.geocluster_test.hashChildren(hash, map, bounds_geohash, "#ff0000", 0.5, true);

      length += 1;
      var hash = Geohash.get_key(center.lat, center.lng, length);

      Drupal.behaviors.geocluster_test.hashChildren(hash, map, bounds_geohash, "#00ff00", 0.1);
    },

    hashChildren: function(hash, map, bounds_geohash, color, opacity, drawLabel) {
      var keys = new Object;
      Geohash.fill_bbox(hash, keys, bounds_geohash);
      $.each(keys, function(key) {
        Drupal.behaviors.geocluster_test.drawHashRect(key, map, color, opacity, drawLabel);
      });
    },

    drawHashRect: function(hash, map, color, opacity, drawLabel) {
      color = typeof color !== 'undefined' ? color : "#ff7800";
      opacity = typeof opacity !== 'undefined' ? opacity : 0.5;
      drawLabel = typeof drawLabel !== 'undefined' ? drawLabel : false;

      var bounds_geohash = Geohash.bbox(hash);

      var southWest = new L.LatLng(bounds_geohash[3], bounds_geohash[0]),
        northEast = new L.LatLng(bounds_geohash[1], bounds_geohash[2]),
        bounds = new L.LatLngBounds(southWest, northEast);

      // convert bounds
      // var bounds = [[bounds_geohash[3], bounds_geohash[0]], [bounds_geohash[1], bounds_geohash[2]]];

      // create an orange rectangle
      var rect = L.rectangle(bounds, {color: color, weight: 1, opacity: opacity, fillOpacity: 0.0});

      if (drawLabel) {
        var myIcon = L.divIcon({className: 'label', html: hash, iconSize: new L.Point(40, 12)});
        var label = L.marker(bounds.getCenter(), {icon: myIcon});
        Drupal.behaviors.geocluster_test.hashGroup.addLayer(label);
      }

      Drupal.behaviors.geocluster_test.hashGroup.addLayer(rect);
    }

  };

  var _geocluster_test_old_leaflet_initialize = L.Map.prototype.initialize;

  L.Map.include({

    initialize: function(/*HTMLElement or String*/ id, /*Object*/ options) {
      _geocluster_test_old_leaflet_initialize.apply(this, [id, options]);
      this.on('load', Drupal.behaviors.geocluster_test.onMapLoad, this);
    }

  });

})(jQuery);
