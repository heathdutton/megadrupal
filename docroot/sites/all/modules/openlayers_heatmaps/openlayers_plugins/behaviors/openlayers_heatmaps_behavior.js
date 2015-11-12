/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

Drupal.openlayers.addBehavior('openlayers_heatmaps_behavior', function (context, options) {
  var map = context.openlayers;
  var layers = [];

  for (var i in options.layers) {
    layeroptions = options.layers[i];
    if (layeroptions.layer == 1) {
      var selectedLayer = map.getLayersBy('drupalID', i);
      if (typeof selectedLayer[0] != 'undefined') {
        layers.push(selectedLayer[0]);
      }
    }
  }

  // If no layer is selected, just return.
  if (layers.length < 1) {
    return;
  }

  for (var i in layers) {
    var layer = layers[i];

    var layersoptions = options['layers'];
    var drupalID = layer.drupalID;
    var layeroptions = layersoptions[drupalID]

    var radius = parseInt(layeroptions.radius, 10);
    var intensity = parseInt(layeroptions.intensity, 10);
    var opacity = parseInt(layeroptions.opacity, 10);
    var heatmapdata = { max:0, data:[] };
    var heatmap;

    layer.events.on({featuresadded:function (evt) {
      for (var i in evt.features) {
        var feature = evt.features[i];
        if (feature.geometry.CLASS_NAME == "OpenLayers.Geometry.Collection") {
          for (var j in feature.geometry.components) {
            var component = feature.geometry.components[j];
            var f = new OpenLayers.Feature.Vector(new OpenLayers.Geometry.Point(component.x, component.y));
            var ll = new OpenLayers.LonLat(f.geometry.x, f.geometry.y);
            heatmapdata.data.push({lonlat:ll, count:intensity});
          }
        } else {
          var f = new OpenLayers.Feature.Vector(new OpenLayers.Geometry.Point(feature.geometry.x, feature.geometry.y));
          var ll = new OpenLayers.LonLat(f.geometry.x, f.geometry.y);
          heatmapdata.data.push({lonlat:ll, count:intensity});
        }
      }

      // Set the original layer to invisible.
      evt.object.setVisibility(false);

      heatmapdata.max = heatmapdata.data.length

      heatmap = new OpenLayers.Layer.Heatmap(layer.drupalID + '_heatmap', map, evt.object,
          {visible:true, radius:radius},
          {isBaseLayer:false, opacity:opacity, projection: map.getProjectionObject()});

      map.addLayer(heatmap);
      map.zoomToMaxExtent();
      map.zoomIn();
      heatmap.setDataSet(heatmapdata);
    }});
  }


});
