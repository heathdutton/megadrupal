(function($) {


 // this is an ugly control based on LayerSwitch
 // I will have to do it right one day, but for
 // now I am kind of sick of dealing with OpenLayers
OpenLayers.Control.EditControl =
  OpenLayers.Class(OpenLayers.Control, {
    roundedCorner: false,
    roundedCornerColor: "darkblue",
    dataLbl: null,
    minimizeDiv: null,
    maximizeDiv: null,
    ascending: true,
    initialize: function(options) {
        OpenLayers.Control.prototype.initialize.apply(this, arguments);
        this.layerStates = [];

        if(this.roundedCorner) {
            OpenLayers.Console.warn('roundedCorner option is deprecated');
        }
    },

    setMap: function(map) {
        OpenLayers.Control.prototype.setMap.apply(this, arguments);

        this.map.events.on({
            addlayer: this.redraw,
            changelayer: this.redraw,
            removelayer: this.redraw,
            changebaselayer: this.redraw,
            scope: this
        });
        if (this.outsideViewport) {
            this.events.attachToElement(this.div);
            this.events.register("buttonclick", this, this.onButtonClick);
        } else {
            this.map.events.register("buttonclick", this, this.onButtonClick);
        }
    },

    draw: function() {
        OpenLayers.Control.prototype.draw.apply(this);

        // create layout divs
        this.loadContents();

        // set mode to minimize
        if(!this.outsideViewport) {
            this.minimizeControl();
        }

        // populate div with current info
        this.redraw();    

        return this.div;
    },

    onButtonClick: function(evt) {
        var button = evt.buttonElement;
        if (button === this.minimizeDiv) {
            this.minimizeControl();
        } else if (button === this.maximizeDiv) {
            this.maximizeControl();
        }
    },

    redraw: function() {
        return this.div;
    },

    maximizeControl: function(e) {
        this.div.style.width = "";
        this.div.style.height = "";

        this.showControls(false);

        if (e != null) {
            OpenLayers.Event.stop(e);
        }
    },
    minimizeControl: function(e) {
        this.div.style.width = "0px";
        this.div.style.height = "0px";

        this.showControls(true);

        if (e != null) {
            OpenLayers.Event.stop(e);
        }
    },

    showControls: function(minimize) {
        this.maximizeDiv.style.display = minimize ? "" : "none";
        this.minimizeDiv.style.display = minimize ? "none" : "";
    },

    onChange: function(evt) {
    },

    loadContents: function() {

        var container = $('<div />').attr('id', this.id + '_layersDiv').addClass('layersDiv');
        var list = $('<ul />');
        var element = $('<li />');
        element.append(
          $('<input />')
            .attr('type', 'radio')
            .attr('id', this.id + '_action_move')
            .attr('name', this.id + '_action')
            .attr('checked', 'checked')
            .val('move')
            .change(this.onChange)
        );
        element.append(
          $('<label />')
            .attr('for', this.id + '_action_move')
            .html('move')
        );
        list.append(element);

        var element = $('<li />');
        element.append(
          $('<input />')
            .attr('type', 'radio')
            .attr('id', this.id + '_action_delete')
            .attr('name', this.id + '_action')
            .val('delete')
            .change(this.onChange)
        );
        element.append(
          $('<label />')
            .attr('for', this.id + '_action_delete')
            .html('delete')
        );
        list.append(element);
        container.append(list);

        this.div.appendChild(container.get(0));

        if(this.roundedCorner) {
            OpenLayers.Rico.Corner.round(this.div, {
                corners: "tl bl",
                bgColor: "transparent",
                color: this.roundedCornerColor,
                blend: false
            });
            OpenLayers.Rico.Corner.changeOpacity(this.layersDiv, 0.75);
        }

        // maximize button div
        var img = OpenLayers.Util.getImageLocation('layer-switcher-maximize.png');
        this.maximizeDiv = OpenLayers.Util.createAlphaImageDiv(
                                    "OpenLayers_Control_MaximizeDiv", 
                                    null, 
                                    null, 
                                    img, 
                                    "absolute");
        OpenLayers.Element.addClass(this.maximizeDiv, "maximizeDiv olButton");
        this.maximizeDiv.style.display = "none";

        this.div.appendChild(this.maximizeDiv);

        // minimize button div
        var img = OpenLayers.Util.getImageLocation('layer-switcher-minimize.png');
        this.minimizeDiv = OpenLayers.Util.createAlphaImageDiv(
                                    "OpenLayers_Control_MinimizeDiv", 
                                    null, 
                                    null, 
                                    img, 
                                    "absolute");
        OpenLayers.Element.addClass(this.minimizeDiv, "minimizeDiv olButton");
        this.minimizeDiv.style.display = "none";

        this.div.appendChild(this.minimizeDiv);
    },

    CLASS_NAME: "OpenLayers.Control.EditControl"
});

Drupal.galerie_geoloc.features = [];

Drupal.galerie_geoloc.updateLocation = function(nid, iid, coordinates) {
  var data = coordinates ? {lon: coordinates.x, lat: coordinates.y} : {delete: true};
  $.ajax({
    url: Drupal.settings.galerie['galerie-' + nid].updateGeolocationUrl + '/' + iid,
    data: data
  });
};

Drupal.behaviors.galerie_geoloc = {
  attach: function (context) {
    $('.galerie-geoloc').each(function() {
      var nid = $(this).attr('id').replace(/^node-/, '');
      var settings = Drupal.settings.galerie['galerie-' + nid];
      settings.editMode = 'move';
      var galerie_element = $(this);

      settings.featureSelected = function(evt) {
        if (settings.editMode == 'delete') {
          var iid = evt.feature.cluster[0].data.image['id'];
          Drupal.galerie_geoloc.updateLocation(nid, iid);
          Drupal.galerie_geoloc.strategy.deactivate();
          layer.removeFeatures([evt.feature.cluster[0]]);
          Drupal.galerie_geoloc.strategy.activate();
        } else {
          var newBounds = new OpenLayers.Bounds();
          for (var i in evt.feature.cluster) {
            newBounds.extend(evt.feature.cluster[i].geometry);
          }
          map.zoomToExtent(newBounds);
        }
      };
      settings.featureHighlighted = function(evt) {
        for (var i in evt.feature.cluster) {
          $('a[data-id=' + evt.feature.cluster[i].id + ']').addClass('galerie-geoloc-highlight');
        }
      };
      settings.featureUnhighlighted = function(evt) {
        $('a.galerie-geoloc-highlight').removeClass('galerie-geoloc-highlight');
      };
      settings.moveEnd = function(evt) {
        Drupal.galerie_geoloc.strategy.deactivate();
        Drupal.galerie_geoloc.strategy.activate();
      };

      var map = Drupal.galerie_geoloc.createMap('galerie-geoloc-map', settings);
      var layer = Drupal.galerie_geoloc.createLayer(map, settings);
      layer.styleMap.styles.default.defaultStyle.cursor = 'move';
      var mousePosition = new OpenLayers.Control.MousePosition();
      map.addControl(mousePosition);
      mousePosition.activate();

      var dragControl = new OpenLayers.Control.DragFeature(layer);
      map.addControl(dragControl);
      dragControl.activate();
      dragControl.onStart = function(feature, pixel) {
        Drupal.galerie_geoloc.strategy.deactivate();
        layer.styleMap.styles.default.defaultStyle.fontColor = 'transparent';
        layer.styleMap.styles.default.defaultStyle.labelOutlineColor = 'transparent';
      };

      dragControl.onComplete = function(feature, pixel) {
        var extent = map.getExtent();
        var lonlat = map.getLonLatFromPixel(pixel);

        if (extent.containsLonLat(lonlat)) {
          feature.cluster[0].geometry = feature.geometry;
          var coordinates = new OpenLayers.Geometry.Point(lonlat.lon, lonlat.lat);
          coordinates.transform(map.getProjectionObject(), new OpenLayers.Projection("EPSG:4326"));

          var iid = feature.cluster[0].data.image['id'];
          Drupal.galerie_geoloc.updateLocation(nid, iid, coordinates);

          layer.styleMap.styles.default.defaultStyle.fontColor = 'black';
          layer.styleMap.styles.default.defaultStyle.labelOutlineColor = 'white';

          Drupal.galerie_geoloc.strategy.deactivate();
          Drupal.galerie_geoloc.strategy.activate();
        }
      };

      var editControl = new OpenLayers.Control.EditControl();
      editControl.onChange = function(evt) {
        switch ($(this).val()) {
          case 'move':
            settings.editMode = 'move';
            layer.styleMap.styles.default.defaultStyle.cursor = 'move';
            dragControl.activate();
            break;
          case 'delete':
            settings.editMode = 'delete';
            layer.styleMap.styles.default.defaultStyle.cursor = 'crosshair';
            dragControl.deactivate();
            break;
          default:
            return;
        }
      };
      map.addControl(editControl);

      var div = $('#galerie-geoloc-map-preview .galerie-browser');
      var qsdiv = $('#galerie-geoloc-map-preview-quicksand');

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
        } else {
          var element = $('<img />').attr('src', image['thumb-src']);
          element.attr('alt', image['title']).attr('title', image['title']);
          element.attr('data-image-id', id);
          var a = $('<a />').attr('href', image['thumb-link']).append(element);
          a.attr('data-id', id);
          qsdiv.append(a);
        }
      }

      var placeFeature = function(image, layer, element) {
        return function(event, ui) {
          var lonlat = map.getLonLatFromPixel(mousePosition.lastXy);
          var point = new OpenLayers.Geometry.Point(lonlat.lon, lonlat.lat);
          var feature = new OpenLayers.Feature.Vector(point, {index: 1, image: image});

          var coordinates = new OpenLayers.Geometry.Point(lonlat.lon, lonlat.lat);
          coordinates.transform(map.getProjectionObject(), new OpenLayers.Projection("EPSG:4326"));

          Drupal.galerie_geoloc.strategy.deactivate();
          layer.addFeatures([feature]);
          Drupal.galerie_geoloc.strategy.activate();

          var id = element.attr('data-id');


          Drupal.galerie_geoloc.updateLocation(nid, id, coordinates);

          $('#galerie-geoloc-map-preview-quicksand a[data-id=' + id + ']').remove();
          div.quicksand(qsdiv.find('a'), {}, quicksandCallback);
        }
      };

      var quicksandCallback = function() {
        $('.galerie-browser-wrapper .galerie-browser a').each(function() {
          var element = $(this);
          var image = Drupal.settings.galerie['galerie-' + nid].images[element.attr('data-id')];
          element.find('img').draggable({
            containment: '.galerie-geoloc',
            appendTo: 'body',
            helper: 'clone',
            cursorAt: {
              top: 56,
              left: 23,
            },
            stop: placeFeature(image, layer, element)
          });
        });
      };

      div.quicksand(qsdiv.find('a'), {}, quicksandCallback);

      layer.addFeatures(features);

      map.zoomToMaxExtent();
    });
  }
};


})(jQuery);

