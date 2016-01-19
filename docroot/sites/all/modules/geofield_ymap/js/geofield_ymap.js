(function ($) {
  Drupal.behaviors.geofieldYmap = {
    attach: function (context, settings) {
      $('div.geofield-ymap', context).once('geofield-ymap', function () {
        var mapId = this.id;
        if (!mapId) {
          mapId = this.id = Drupal.geofieldYmap.generateMapId();
        }
        ymaps.ready(function () {
          Drupal.geofieldYmap.mapInit(mapId);
        });
      });
    }
  };

  Drupal.geofieldYmap = Drupal.geofieldYmap || {
    data: {},

    defaultSettings: {
      center: [0, 0],
      zoom: 0
    },

    /**
     * Initialize map
     */
    mapInit: function (mapId, settings) {
      var dataSettings = Drupal.geofieldYmap.getDataSettings(mapId);
      settings = $.extend({}, Drupal.geofieldYmap.defaultSettings, dataSettings, settings);

      // Create map
      var map = Drupal.geofieldYmap.createMap(mapId, settings);

      // Set geo objects global options
      var globalOptions = {strokeWidth: 4};
      if (Drupal.settings.geofieldYmap.preset) {
        globalOptions.preset = Drupal.settings.geofieldYmap.preset;
      }
      if (settings.objectPreset) {
        globalOptions.preset = settings.objectPreset;
      }
      map.geoObjects.options.set(globalOptions);

      // Add geo objects
      if (!settings.withoutObjects) {
        if (settings.objects) {
          Drupal.geofieldYmap.addObject(map, settings.objects, settings.editable, settings.clusterize);
        }
        else if (settings.editable) {
          var objects = $('#' + mapId + ' ~ input[name$="[objects]"]').val();
          Drupal.geofieldYmap.addObject(map, objects, settings.editable);
        }
      }

      // Auto centering
      if (settings.autoCentering || !settings.center || (settings.center[0] == 0 && settings.center[1] == 0)) {
        Drupal.geofieldYmap.autoCentering(map);
      }

      // Auto zooming
      if (settings.autoZooming || !settings.zoom) {
        Drupal.geofieldYmap.autoZooming(map);
      }

      // Editing features
      if (settings.editable) {
        Drupal.geofieldYmap.addEditButtons(map, settings.objectTypes);
        map.geoObjects.events.add('remove', Drupal.geofieldYmap.objectsChangeHandler);
        map.events.add('click', Drupal.geofieldYmap.mapClickHandler);
        map.events.add('boundschange', Drupal.geofieldYmap.mapBoundschangeHandler);

        // Select default edit button
        if (settings.selectedControl) {
          var edutButton = Drupal.geofieldYmap.getEditButton(map, settings.selectedControl);
          edutButton.events.fire('click');
          edutButton.select();
        }
      }

      // Tweak search control
      Drupal.geofieldYmap.tweakSearchControl(map);

      $('#' + mapId).trigger('yandexMapInit', [map]);

      return map;
    },

    /**
     * Create map
     */
    createMap: function (mapId, settings) {
      var mapState = Drupal.geofieldYmap.getMapStateFromSettings(settings);
      var map = new ymaps.Map(mapId, mapState, settings.options);
      map.mapId = mapId;

      Drupal.geofieldYmap.data[mapId] = {
        map: map,
        settings: settings,
        cursor: null,
        drawingMode: false
      };

      return map;
    },

    /**
     * Return map data-* settings
     */
    getDataSettings: function (mapId) {
      var $map = $('#' + mapId);
      var settings = {};
      var arrayTypeSettings = ['center', 'controls', 'behaviors', 'objectTypes'];

      $.each($map.data(), function (attributeKey, attributeValue) {
        var matches = attributeKey.match(/map(.+)/);
        if (matches) {
          var settingKey;

          // For jQuery 1.4.4-1.5
          if (matches[1][0] == '-') {
            settingKey = matches[1].substring(1).replace(/-(.)/g, function (str, match) {
              return match.toUpperCase();
            });
          }
          // For jQuery 1.6+
          else {
            settingKey = matches[1].substring(0, 1).toLowerCase() + matches[1].substring(1);
          }

          if ($.inArray(settingKey, arrayTypeSettings) != -1 && $.type(attributeValue) != 'array') {
            attributeValue = (attributeValue == '<none>') ? [] : attributeValue.split(',');
          }
          settings[settingKey] = attributeValue;
        }
      });

      return settings;
    },

    /**
     * Return default map state from settings
     */
    getMapStateFromSettings: function (settings) {
      var state = {};
      $.each(['behaviors', 'bounds', 'center', 'controls', 'type', 'zoom'], function (index, stateKey) {
        if (stateKey in settings) {
          state[stateKey] = settings[stateKey];
        }
      });
      return state;
    },

    /**
     * Add edit buttons
     */
    addEditButtons: function (map, buttonTypes) {
      if (!buttonTypes) return;

      var buttonTitles = {
        point:   Drupal.t('Add point', {}, {context: 'Geometry'}),
        line:    Drupal.t('Add line', {}, {context: 'Geometry'}),
        polygon: Drupal.t('Add polygone', {}, {context: 'Geometry'}),
      }

      $.each(buttonTypes, function (index, buttonType) {
        var button = new ymaps.control.Button({
          data: {
            image: Drupal.settings.geofieldYmap.modulePath + '/images/icon-' + buttonType + '.png',
            title: buttonTitles[buttonType],
            editButtonType: buttonType // Custom data property
          }
        });
        button.events.add('click', Drupal.geofieldYmap.editButtonClickHandler);
        map.controls.add(button);
      });
    },

    /**
     * Return selected edit button
     */
    getSelectedEditButton: function (map) {
      var button = null;
      map.controls.each(function (control) {
        if (control.data.get('editButtonType') && control.state.get('selected')) {
          button = control;
        }
      });
      return button;
    },

    /**
     * Return edit button by type
     */
    getEditButton: function (map, editButtonType) {
      var editButton;
      map.controls.each(function (control) {
        if (control.data.get('editButtonType') == editButtonType) {
          editButton = control;
        }
      });
      return editButton;
    },

    /**
     * Deselect controls
     */
    deselectControls: function (map) {
      // Deselect edit buttons
      map.controls.each(function (control) {
        if (control.data.get('editButtonType')) {
          control.deselect();
        }
      });

      // Deselect ruler
      var rulerControl = map.controls.get('rulerControl');
      if (rulerControl) {
        rulerControl.deselect();
      }
    },

    /**
     * Add geo object
     */
    addObject: function (map, object, editMode, clusterize) {
      if (!object) return;

      if ($.type(object) === 'string')  {
        object = JSON.parse(object);
      }

      var geoQueryResult = ymaps.geoQuery(object);

      // Clusterize placemarks
      if (clusterize) {
        var points = geoQueryResult.search('geometry.type = "Point"');
        var notPoints = geoQueryResult.search('geometry.type != "Point"');
        var clusterer = points.clusterize({
          hasHint: false,
          margin: 15,
          zoomMargin: 20
        });
        map.geoObjects.add(clusterer);
        notPoints.addToMap(map);
      }
      else {
        geoQueryResult.addToMap(map);
      }

      // Enable edit mode
      if (editMode) {
        geoQueryResult.addEvents(['mapchange', 'editorstatechange', 'dragend', 'geometrychange'], Drupal.geofieldYmap.objectsChangeHandler);
        geoQueryResult.addEvents('editorstatechange', Drupal.geofieldYmap.objectEditorStateChangeHandler);
        geoQueryResult.addEvents('dblclick', Drupal.geofieldYmap.objectsDblclickHandler);
        geoQueryResult.setOptions({draggable: true});
        geoQueryResult.each(function (object) {
          object.editor.startEditing();
        });
      }
    },

    /**
     * Add geo object by type
     */
    addObjectByType: function (map, objectType, geometry, editMode, startDrawing) {
      if (objectType == 'point') {
        var object = new ymaps.Placemark(geometry);
      }
      else if (objectType == 'line') {
        var object = new ymaps.Polyline(geometry);
      }
      else if (objectType == 'polygon') {
        var object = new ymaps.Polygon(geometry);
      }
      Drupal.geofieldYmap.addObject(map, object, editMode);

      if (startDrawing) {
        object.editor.startDrawing();
      }
    },

    /**
     * Return map.geoObjects in GeoJSON format
     */
    getObjectsInGeoJson: function (map) {
      var objects = {
        type: 'FeatureCollection',
        features: []
      };
      map.geoObjects.each(function (object) {
        var feature = {
          type: 'Feature',
          geometry: {
            type: object.geometry.getType(),
            coordinates: object.geometry.getCoordinates()
          }
        };
        objects.features.push(feature);
      });
      return JSON.stringify(objects);
    },

    /**
     * Auto centering map
     */
    autoCentering: function (map) {
      if (map.geoObjects.getLength() == 0) return;
      var centerAndZoom = ymaps.util.bounds.getCenterAndZoom(map.geoObjects.getBounds(), map.container.getSize());
      map.setCenter(centerAndZoom.center);
    },

    /**
     * Auto zooming map
     */
    autoZooming: function (map) {
      if (map.geoObjects.getLength() == 0) return;
      var mapSize = map.container.getSize();
      var centerAndZoom = ymaps.util.bounds.getCenterAndZoom(
        map.geoObjects.getBounds(),
        mapSize,
        ymaps.projection.wgs84Mercator,
        {margin: 30}
      );
      map.setZoom(centerAndZoom.zoom <= 16 ? centerAndZoom.zoom : 16);
    },

    /**
     * Return new map id
     */
    generateMapId: function (number) {
      if (!number) number = 1;
      var mapId = 'geofield-ymap-' + number;
      if ($('#' + mapId).length > 0) {
        return Drupal.geofieldYmap.generateMapId(number + 1);
      }
      return mapId;
    },

    /**
     * Return total bounds by bounds collection
     */
    getTotalBounds: function (boundsCollection) {
      var totalBounds;

      $.each(boundsCollection, function (index, bounds) {
        if (!bounds) {
          return;
        }
        if (!totalBounds) {
          totalBounds = bounds;
          return;
        }

        // Min
        if (totalBounds[0][0] > bounds[0][0]) totalBounds[0][0] = bounds[0][0];
        if (totalBounds[0][1] > bounds[0][1]) totalBounds[0][1] = bounds[0][1];
        // Max
        if (totalBounds[1][0] < bounds[1][0]) totalBounds[1][0] = bounds[1][0];
        if (totalBounds[1][1] < bounds[1][1]) totalBounds[1][1] = bounds[1][1];
      });

      return totalBounds;
    },

    /**
     * Tweak search control
     */
    tweakSearchControl: function (map) {
      var searchControl = map.controls.get('searchControl');
      if (searchControl) {
        // Remove search result placemark after close balloon
        searchControl.events.add('resultshow', function (event) {
          var resultIndex = event.get('index');
          var result = searchControl.getResultsArray()[resultIndex];
          result.balloon.events.add('close', function (event) {
            result.getParent().remove(result);
          });
        });
      }
    },

    /**
     * Change map cursor
     */
    changeCursor: function (map, cursor) {
      var cursorAccessor = Drupal.geofieldYmap.data[map.mapId].cursor;
      if (!cursorAccessor || !cursorAccessor.getKey(cursor)) {
        cursorAccessor = map.cursors.push(cursor);
      }
      else {
        cursorAccessor.setKey(cursor);
      }
    },

    /**
     * Edit button click handler
     */
    editButtonClickHandler: function (event) {
      var button = event.get('target');
      var map = button.getMap();

      if (!button.state.get('selected')) {
        Drupal.geofieldYmap.deselectControls(map);
        Drupal.geofieldYmap.changeCursor(map, 'arrow');
      }
      else {
        Drupal.geofieldYmap.changeCursor(map, 'grab');
      }
    },

    /**
     * Object editorstatechange handler
     */
    objectEditorStateChangeHandler: function (event) {
      var object = event.get('target');
      var map = object.getMap();
      if (map) {
        Drupal.geofieldYmap.data[map.mapId].drawingMode = object.editor.state.get('drawing');
      }
    },

    /**
     * Objects change handler
     */
    objectsChangeHandler: function (event) {
      var object = event.get('target');
      var map = object.getMap();
      if (map) {
        var $objectsInput = $('#' + map.mapId + ' ~ input[name$="[objects]"]');
        $objectsInput.val(Drupal.geofieldYmap.getObjectsInGeoJson(map));
      }
    },

    /**
     * Object dblclick handler
     */
    objectsDblclickHandler: function (event) {
      var object = event.get('target');
      var map = object.getMap();
      map.geoObjects.remove(object);
      event.stopPropagation();
    },

    /**
     * Map click handler
     */
    mapClickHandler: function (event) {
      var map = event.get('target');
      var settings = Drupal.geofieldYmap.data[map.mapId].settings;
      var selectedButton = Drupal.geofieldYmap.getSelectedEditButton(map);

      if (selectedButton && !Drupal.geofieldYmap.data[map.mapId].drawingMode) {
        if (!settings.multiple) {
          map.geoObjects.removeAll();
        }

        var selectedButtonType = selectedButton.data.get('editButtonType');
        var geometry = event.get('coords');
        var startDrawing = false;

        if (selectedButtonType == 'line') {
          geometry = [geometry];
          startDrawing = true;
        }
        else if (selectedButtonType == 'polygon') {
          geometry = [[geometry]];
          startDrawing = true;
        }

        Drupal.geofieldYmap.addObjectByType(map, selectedButtonType, geometry, true, startDrawing);
      }
    },

    /**
     * Map boundschange handler
     */
    mapBoundschangeHandler: function (event) {
      var map = event.get('target');
      if (map) {
        $('#' + map.mapId + ' ~ input[name$="[center]"]').val(map.getCenter().join(','));
        $('#' + map.mapId + ' ~ input[name$="[zoom]"]').val(map.getZoom());
      }
    }
  };
})(jQuery);
