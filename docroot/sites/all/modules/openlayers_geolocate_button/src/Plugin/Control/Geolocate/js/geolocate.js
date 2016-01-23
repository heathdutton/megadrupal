Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Control:Geolocate',
  init: function(data) {
    return new ol.control.Geolocate(data.opt, data.map);
  }
});
