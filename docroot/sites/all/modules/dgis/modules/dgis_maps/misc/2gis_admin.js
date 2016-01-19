(function($) {

  /**
   * Main object for control the map.
   */
  mapAdmin = {

    maps : {},

    /**
     * Initialization.
     */
    initMap: function(mapKey) {
      var map = mapAdmin.maps[mapKey].map;
      mapAdmin.setMapState(map);

      // Element to add the new markers.
      var MyControl = DG.Control.extend({
        options: {
          position: 'bottomleft'
        },

        onAdd: function (map) {
          var container = DG.DomUtil.create('div', 'dgis-add-marker dgis-add-marker-' + mapKey);
          return container;
        }
      });

      map.addControl(new MyControl());

      // Bind click event for control.
      $('.dgis-add-marker-' + mapKey).click(function() {
        mapAdmin.maps[mapKey].control_mode = (mapAdmin.maps[mapKey].control_mode == 'marker') ? null : 'marker';
        $(this).toggleClass('active');
        return false;
      });

      // Click event for map.
      map.on('click', function(event) {
        mapAdmin.clickOnMap(event, mapKey);
      });

      // Take markers and apply them.
      var markers = mapAdmin.getMapMarkers(mapKey);
      if (markers) {
        $.each(markers, function(id, markerInfo) {
          var marker = DG.marker(markerInfo.point).addTo(map);
          var popupHeader = markerInfo.popup.header;
          var popupContent = markerInfo.popup.content;
          mapAdminPopup.processPopup(mapKey, marker, id, popupHeader, popupContent);

          // Add each marker into global array.
          mapAdmin.maps[mapKey]['markers'][id] = marker;
        });
      }
    },

    /**
     * Action, when user clicked on map.
     */
    clickOnMap: function(event, mapKey) {
      if (mapAdmin.maps[mapKey].control_mode == 'marker') {
        var id = Object.keys(mapAdmin.maps[mapKey].markers).length;
        var marker = DG.marker(event.latlng);
        marker.addTo(mapAdmin.maps[mapKey].map);

        mapAdminPopup.processPopup(mapKey, marker, id, '', '');
        mapAdmin.maps[mapKey].markers[id] = marker;
        mapAdmin.saveData();
      }
    },

    /**
     * Get current state of map from the text fields.
     */
    getMapState: function(map) {
      // Get center.
      var center = $('#' + map).parent().find('.edit-dgis-map-center').val().split(',');
      // Get zoom.
      var zoom = $('#' + map).parent().find('.edit-dgis-map-zoom').val();
      return {center: center, zoom: zoom};
    },

    /**
     * Get markers.
     */
    getMapMarkers: function(map) {
      // Get markers.
      var markers = $('#' + map).parent().find('.edit-dgis-map-markers').val();
      if (markers.length) {
        return $.parseJSON(markers);
      }
      return false;
    },

    /**
     * Save current state of map into text fields.
     */
    setMapState: function(map) {
      // Set center.
      var center = map.getCenter();
      $(map._container).parent().find('.edit-dgis-map-center').val(center.lat + ',' + center.lng);
      // Set zoom.
      var zoom = map.getZoom();
      $(map._container).parent().find('.edit-dgis-map-zoom').val(zoom);
    },

    /**
     * Save data about markers.
     */
    saveData: function() {
      $.each(mapAdmin.maps, function(key, data) {
        var finalData = [];
        $.each(data.markers, function(markerKey, markerData) {
          // Get new values for changed popup.
          var popupHeaderValue = $(markerData._popup._wrapper).find('input').val();
          var popupContentValue = $(markerData._popup._wrapper).find('textarea').val();
          // Get old value if we don't changed this popup.
          popupHeaderValue = (typeof popupHeaderValue == 'undefined') ? $(markerData._popup.getHeaderContent()).filter('input').val() : popupHeaderValue;
          popupContentValue = (typeof popupContentValue == 'undefined') ? $(markerData._popup.getContent()).filter('textarea').val() : popupContentValue;
          finalData[markerKey] = {point: markerData._latlng, popup: {header: popupHeaderValue, content: popupContentValue}}
        });
        $(data.map._container).parent().find('.edit-dgis-map-markers').val(JSON.stringify(finalData));
      });
    }
  };

  /**
   * Object to work with popups.
   */
  mapAdminPopup = {

    /**
     * Process popup for marker.
     */
    processPopup: function(mapKey, marker, id, popupHeader, popupContent) {
      marker.bindPopup(mapAdminPopup.getPopupElementContent(popupContent));
      marker._popup.setHeaderContent(mapAdminPopup.getPopupElementHeader(popupHeader));
      marker._popup.setFooterContent(mapAdminPopup.getPopupElementFooter(mapKey, id));
    },

    /**
     * Save popup for the marker.
     */
    savePopup: function(event, mapKey, id) {
      var markers = mapAdmin.maps[mapKey].markers;
      var newHeader = $(markers[id]._popup._wrapper).find('input').val();
      var newContent = $(markers[id]._popup._wrapper).find('textarea').val();
      markers[id]._popup.setHeaderContent(mapAdminPopup.getPopupElementHeader(newHeader));
      markers[id]._popup.setContent(mapAdminPopup.getPopupElementContent(newContent));
      markers[id]._popup._closeButton.click();
      event.preventDefault();
      mapAdmin.saveData();
    },

    /**
     * Remove marker.
     */
    removeMarker: function(event, mapKey, id) {
      mapAdmin.maps[mapKey].markers[id].remove();
      delete mapAdmin.maps[mapKey].markers[id];
      // Recreate markers.
      var newMarkers = [];
      $.each(mapAdmin.maps[mapKey].markers, function(key, val) {
        if (typeof val != 'undefined') {
          newMarkers[newMarkers.length] = val;
        }
      });
      mapAdmin.maps[mapKey].markers = newMarkers;

      event.preventDefault();
      mapAdmin.saveData();
    },

    /**
     * Returns HTML items for header.
     */
    getPopupElementHeader: function(value) {
      return '<br /><input type="text" placeholder="' + Drupal.t('Title') + '" value="' + value + '" />';
    },

    /**
     * Returns HTML items for content part.
     */
    getPopupElementContent: function(value) {
      return '<br /><textarea placeholder="' + Drupal.t('Description') + '">' + value + '</textarea>';
    },

    /**
     * Returns HTML items for footer.
     */
    getPopupElementFooter: function(mapKey, id) {
      return '<br /><button onclick="mapAdminPopup.savePopup(event, \'' + mapKey + '\', ' + id + ')">' + Drupal.t('Save') + '</button>'
        + '<button onclick="mapAdminPopup.removeMarker(event, \'' + mapKey + '\', ' + id + ')">' + Drupal.t('Remove') + '</button>';
    }

  };

})(jQuery);

DG.then(function () {
  Drupal.behaviors.dgisInitMaps = {
    attach: function(context, settings) {
      // Clear maps if they were initialized.
      jQuery.each(mapAdmin.maps, function(key, mapObj) {
        mapObj.map.remove();
      });
      // Initialize maps.
      mapAdmin.maps = {};
      jQuery('.dgis-maps-container-edit').each(function(data) {
        var key = jQuery(this).attr('id');
        var state = mapAdmin.getMapState(key);
        mapAdmin.maps[key] = {'map': {}, 'markers': [], control_mode: ''};
        mapAdmin.maps[key]['map'] = DG.map(key, {
          center: state.center,
          zoom: state.zoom
        });

        mapAdmin.initMap(key);
        mapAdmin.maps[key]['map'].on('move', function() {
          mapAdmin.setMapState(mapAdmin.maps[key]['map']);
        });
      });
    }
  }

  Drupal.behaviors.dgisInitMaps.attach();
});
