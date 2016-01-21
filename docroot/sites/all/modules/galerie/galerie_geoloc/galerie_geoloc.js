(function($) {

Drupal.galerie_geoloc = Drupal.galerie_geoloc || {};

Drupal.galerie_geoloc.createMap = function(div, settings) {
  var map = new OpenLayers.Map({
    div: div,
    allOverlays: true,
    controls: [
      new OpenLayers.Control.Navigation(
        {dragPanOptions: {enableKinetic: true}}
      ),
      new OpenLayers.Control.PanZoomBar()
    ]
  });

  var gmap = new OpenLayers.Layer.Google(
    "Google Hybrid",
    {type: google.maps.MapTypeId.HYBRID}
    );
  map.addLayers([gmap]);

  var gmap = new OpenLayers.Layer.Google(
    "Google Streets"
    );
  map.addLayers([gmap]);

  var gmap = new OpenLayers.Layer.Google(
    "Google Terrain",
    {type: google.maps.MapTypeId.TERRAIN}
    );
  map.addLayers([gmap]);

  return map;
};

Drupal.galerie_geoloc.strategy = {};

Drupal.galerie_geoloc.feature = function(feature) {
  if (feature.cluster) {
    return feature.cluster[0];
  }

  return feature;
};

Drupal.galerie_geoloc.createLayer = function(map, settings) {
  var style = new OpenLayers.Style({
    label: "${label}",
    labelYOffset: 35,
    labelOutlineColor: "white",
    labelOutlineWidth: 3,
    fontColor: "black",
    fontWeight: "bold",
    fontSize: "24px",
    externalGraphic: "${thumb}",
    backgroundGraphic: settings.markerUrl,
    backgroundHeight: 56,
    backgroundWidth: 46,
    graphicHeight: 40,
    graphicWidth: 40,
    backgroundGraphicZIndex: "${bgZIndex}",
    graphicZIndex: "${fgZIndex}",
    backgroundYOffset: -55,
    graphicXOffset: -20,
    graphicYOffset: -52,
    graphicTitle: "${title}",
    cursor: 'pointer'
  }, {
    context: {
      title: function(feature) {
        var titles = {};
        for (var i in feature.cluster) {
          titles[feature.cluster[i].data.image.title] = feature.cluster[i].data.image.title;
        }
        var a = [];
        for (var i in titles) {
          a.push(titles[i]);
        }
        return a.join("\n");
      },
      fgZIndex: function(feature) {return (Math.floor(-(Drupal.galerie_geoloc.feature(feature).geometry.y))) + 2;},
      bgZIndex: function(feature) {return (Math.floor(-(Drupal.galerie_geoloc.feature(feature).geometry.y))) + 1;},
      thumb: function(feature) {return Drupal.galerie_geoloc.feature(feature).data.image['thumb-src'];},
      label: function(feature) {return (feature.cluster && feature.cluster.length > 1) ? feature.cluster.length : '';},
    }
  });

  Drupal.galerie_geoloc.strategy = new OpenLayers.Strategy.Cluster();
  Drupal.galerie_geoloc.strategy.distance = 40;
  Drupal.galerie_geoloc.strategy.deactivate = function() {
    var deactivated = OpenLayers.Strategy.prototype.deactivate.call(this);
    if (deactivated) {
      var features = [];
      var clusters = this.layer.features;
      for (var i=0; i<clusters.length; i++) {
        var cluster = clusters[i];
        if (cluster.cluster) {
          for (var j=0; j<cluster.cluster.length; j++) {
            features.push(cluster.cluster[j]);
          }
        } else {
          features.push(cluster);
        }
      }
      this.layer.removeAllFeatures();
      this.layer.events.un({
        "beforefeaturesadded": this.cacheFeatures,
        "moveend": this.cluster,
        scope: this
      });
      this.layer.addFeatures(features);
      this.clearCache();
    }
    return deactivated;
  };
  Drupal.galerie_geoloc.strategy.activate = function() {
    var activated = OpenLayers.Strategy.prototype.activate.call(this);
    if (activated) {
      var features = [];
      var clusters = this.layer.features;
      for (var i=0; i<clusters.length; i++) {
        var cluster = clusters[i];
        if (cluster.cluster) {
          for (var j=0; 
              j<cluster.cluster.length; j++) {
            features.push(cluster.cluster[j]);
          }
        } else {
          features.push(cluster);
        }
      }
      this.layer.removeAllFeatures();
      this.layer.events.on({
        "beforefeaturesadded": this.cacheFeatures,
        "moveend": this.cluster,
        scope: this
      });
      this.layer.addFeatures(features);
      this.features = features;
    }
    return activated;
  };

  var vector = new OpenLayers.Layer.Vector(
    "Galerie images", {
      rendererOptions: {zIndexing: true},
      strategies: [
        Drupal.galerie_geoloc.strategy
      ],
      styleMap: new OpenLayers.StyleMap({
        "default": style,
        "hover": {
          backgroundGraphicZIndex: Number.MAX_VALUE - 1,
          graphicZIndex: Number.MAX_VALUE,
          backgroundGraphic: settings.markerHighlightUrl
        },
        "select": {
          backgroundGraphicZIndex: Number.MAX_VALUE - 1,
          graphicZIndex: Number.MAX_VALUE,
          backgroundGraphic: settings.markerHighlightUrl
        }
      })
    }
  );

  map.addLayers([vector]);

  vector.events.on({
    'featureselected': settings.featureSelected,
    'featureunselected': settings.featureUnselected
  });

  map.addControl(new OpenLayers.Control.LayerSwitcher());

  var hoverCtrl = new OpenLayers.Control.SelectFeature(vector, {
    hover: true,
    highlightOnly: true,
    renderIntent: 'hover'
  });
  map.addControl(hoverCtrl);
  hoverCtrl.activate();
  hoverCtrl.events.on({
    'featurehighlighted': settings.featureHighlighted,
    'featureunhighlighted': settings.featureUnhighlighted
  });

  var selectCtrl = new OpenLayers.Control.SelectFeature(vector, {
    hover: false,
    clickout: true,
    multiple: false,
    toggle: false,
    renderIntent: 'select'
  });
  map.addControl(selectCtrl);
  selectCtrl.activate();

  if (undefined == settings.moveEnd) {
    settings.moveEnd = (function(div, qsdiv) {
      return function(evt) {
        var bounds = vector.getExtent();
        qsdiv.html('');
        var images = [];
        var ids = [];
        for (var i in vector.features) {
          for (var j in vector.features[i].cluster) {
            if (bounds.containsPixel(vector.features[i].cluster[j].geometry)) {
              ids.push(vector.features[i].cluster[j].attributes.image.id);
              images.push({
                image: vector.features[i].cluster[j].attributes.image,
                feature: vector.features[i].cluster[j],
                cluster: vector.features[i]
              });
            }
          }
        }

        images.sort(function(a, b) {
          return b.image.date - a.image.date;
        });

        if (undefined != settings.displayedImages &&
              (settings.displayedImages.length == ids.length ||
                  ($(settings.displayedImages).not(ids).length == 0 &&
                   $(ids).not(settings.displayedImages).length == 0)
              )) {
          return;
        } else {
          settings.displayedImages = ids;
        }

        for (var i in images) {
          var element = $('<img />').attr('src', images[i].image['thumb-src']);
          element.attr('alt', images[i].image['title']).attr('title', images[i].image['title']);
          var a = $('<a />').attr('href', images[i].image['thumb-link']).append(element);
          a.attr('data-id', images[i].feature.id);
          a.attr('data-cluster-id', images[i].cluster.id);
          qsdiv.append(a);
        }

        div.quicksand(qsdiv.find('a'), {
            attribute: 'data-id'
          },
          function() {
            var highlight = function() {hoverCtrl.highlight(vector.getFeatureById($(this).attr('data-cluster-id')));};
            var unhighlight = function() {hoverCtrl.unhighlight(vector.getFeatureById($(this).attr('data-cluster-id')));};
            $('.galerie-browser-wrapper .galerie-browser a').hover(highlight, unhighlight);
          }
        );
      };
    })($('#galerie-geoloc-map-preview .galerie-browser'), $('#galerie-geoloc-map-preview-quicksand'));
  }

  map.events.on({
    'moveend': settings.moveEnd
  });

  return vector;
};

Drupal.behaviors.galerie_geoloc = {
  attach: function (context) {
    $('.galerie-geoloc').each(function() {
      var nid = $(this).attr('id').replace(/^node-/, '');
      var settings = Drupal.settings.galerie['galerie-' + nid];

      settings.featureSelected = function(evt) {
        var newBounds = new OpenLayers.Bounds();
        for (var i in evt.feature.cluster) {
          newBounds.extend(evt.feature.cluster[i].geometry);
        }
        map.zoomToExtent(newBounds);
      };
      settings.featureHighlighted = function(evt) {
        for (var i in evt.feature.cluster) {
          $('a[data-id=' + evt.feature.cluster[i].id + ']').addClass('galerie-geoloc-highlight');
        }
      };
      settings.featureUnhighlighted = function(evt) {
        $('a.galerie-geoloc-highlight').removeClass('galerie-geoloc-highlight');
      };
      var map = Drupal.galerie_geoloc.createMap('galerie-geoloc-map', settings);
      var layer = Drupal.galerie_geoloc.createLayer(map, settings);

      var bounds = new OpenLayers.Bounds();
      var features = [];
      var n = 0;
      for (id in Drupal.settings.galerie['galerie-' + nid].images) {
        n++;
        var image = Drupal.settings.galerie['galerie-' + nid].images[id];

        if (image.geolocation && image.geolocation.longitude && image.geolocation.latitude) {
          var point = new OpenLayers.Geometry.Point(image.geolocation.longitude, image.geolocation.latitude);
          var feature = new OpenLayers.Feature.Vector(point, {index: n, image: image});

          feature.geometry.transform(new OpenLayers.Projection("EPSG:4326"), map.getProjectionObject());
          features.push(feature);
          bounds.extend(point);
        }
      }

      layer.addFeatures(features);

      map.zoomToExtent(bounds);
    });
  }
};


})(jQuery);

