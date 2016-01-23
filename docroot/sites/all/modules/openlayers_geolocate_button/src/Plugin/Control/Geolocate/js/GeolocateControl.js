/**
 * Geolocate control
 *
 * Largely copied from Openlayers AutoZoom control and Geolocation component
 * July 30th, 2015.
 */
ol.control.Geolocate = function(opt_options, map) {
  var options = opt_options || {};
  var className = options.className || 'ol-geolocate';

  var this_ = this;
  var handleClick_ = function(e) {
    this_.handleClick_(e);
  };

  var geolocateLabel = options.geolocateLabel || '◎';
  var geolocateTipLabel = options.geolocateTipLabel || 'Geolocate';

  var button = document.createElement('button');
  button.innerHTML = geolocateLabel;
  button.title = geolocateTipLabel;
  button.className = className + '-geolocate';
  button.type = 'button';

  var element = document.createElement('div');
  element.className = className + ' ol-unselectable ol-control';
  element.appendChild(button);

  ol.control.Control.call(this, {
    element: element,
    target: options.target
  });

  // Create a new geolocation object.
  this.geolocation = new ol.Geolocation({
    projection: map.getView().getProjection()
  });

  // Create a position and accuracy feature.
  this.positionFeature = new ol.Feature();
  this.accuracyFeature = new ol.Feature();

  // Draw the position and accuracy features on the map.
  new ol.layer.Vector({
    map: map,
    source: new ol.source.Vector({
      features: [this.positionFeature, this.accuracyFeature]
    })
  });

  button.addEventListener('click', handleClick_, false);
  this.options = options;
};
ol.inherits(ol.control.Geolocate, ol.control.Control);

/**
 * @param {event} event Browser event.
 */
ol.control.Geolocate.prototype.handleClick_ = function(event) {
  event.preventDefault();
  var map = this.getMap();
  var options = this.options;
  var geolocation = this.geolocation;
  var positionFeature = this.positionFeature;
  var accuracyFeature = this.accuracyFeature;

  // Turn on geo tracking.
  geolocation.setTracking(true);

  // When the position changes...
  geolocation.on('change:position', function () {

    // Get the geolocated coordinates.
    var coordinates = geolocation.getPosition();

    // Center on the position.
    map.getView().setCenter(coordinates);

    // Set the zoom based on the options.
    map.getView().setZoom(options.zoom);

    // Set the geometry of the position feature.
    positionFeature.setGeometry(coordinates ? new ol.geom.Point(coordinates) : null);

    // Turn off geo tracking.
    geolocation.setTracking(false);
  });

  // When the accuracy changes...
  geolocation.on('change:accuracyGeometry', function() {
    accuracyFeature.setGeometry(geolocation.getAccuracyGeometry());
  });
};
