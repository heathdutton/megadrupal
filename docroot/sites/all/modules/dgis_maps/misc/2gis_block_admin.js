(function($) {

  /**
   * Main object for control the map.
   */
  mapAdmin = {

    map : {},
    markers : [],
    control_mode : '',

    /**
     * Initialization.
     */
    init: function() {
      mapAdmin.setMapState();

      // Element to add the new markers.
      var MyControl = DG.Control.extend({
        options: {
          position: 'bottomleft'
        },

        onAdd: function (map) {
          var container = DG.DomUtil.create('div', 'dgis-add-marker');
          return container;
        }
      });

      mapAdmin.map.addControl(new MyControl());

      // Bind click event for control.
      $('.dgis-add-marker').click(function() {
        mapAdmin.control_mode = (mapAdmin.control_mode == 'marker') ? null : 'marker';
        $(this).toggleClass('active');
        return false;
      });

      // Click event for map.
      mapAdmin.map.on('click', mapAdmin.clickOnMap);

      // Extract existing markers from Drupal settings.
      if (Drupal.settings.dgis_map_data.markers) {
        var markers = $.parseJSON(Drupal.settings.dgis_map_data.markers);

        $.each(markers, function(id, markerInfo) {
          var marker = DG.marker(markerInfo.point).addTo(mapAdmin.map);
          var popupHeader = markerInfo.popup.header;
          var popupContent = markerInfo.popup.content;
          mapAdminPopup.processPopup(marker, id, popupHeader, popupContent);

          // Add each marker into global array.
          mapAdmin.markers[id] = marker;
        });
      }
    },

    /**
     * Action, when user clicked on map.
     */
    clickOnMap: function(event) {
      if (mapAdmin.control_mode == 'marker') {
        var id = Object.keys(mapAdmin.markers).length;
        var marker = DG.marker(event.latlng);
        marker.addTo(mapAdmin.map);

        mapAdminPopup.processPopup(marker, id, '', '');
        mapAdmin.markers[id] = marker;
        mapAdmin.saveData();
      }
    },

    /**
     * Save current state of map into text fields.
     */
    setMapState: function() {
      // Set center.
      var center = mapAdmin.map.getCenter();
      $('#edit-center').val(center.lat + ',' + center.lng);
      // Set zoom.
      var zoom = mapAdmin.map.getZoom();
      $('#edit-zoom').val(zoom);
    },

    /**
     * Save data about markers.
     */
    saveData: function() {
      var data = [];
      $.each(mapAdmin.markers, function(key, val) {
        // Get new values for changed popup.
        var popupHeaderValue = $(val._popup._wrapper).find('input').val();
        var popupContentValue = $(val._popup._wrapper).find('textarea').val();
        // Get old value if we don't changed this popup.
        popupHeaderValue = (typeof popupHeaderValue == 'undefined') ? $(val._popup.getHeaderContent()).filter('input').val() : popupHeaderValue;
        popupContentValue = (typeof popupContentValue == 'undefined') ? $(val._popup.getContent()).filter('textarea').val() : popupContentValue;
        data[key] = {point: val._latlng, popup: {header: popupHeaderValue, content: popupContentValue}}
      });
      $('[name="markers"]').val(JSON.stringify(data));
    }
  };

  /**
   * Object to work with popups.
   */
  mapAdminPopup = {

    /**
     * Process popup for marker.
     */
    processPopup: function(marker, id, popupHeader, popupContent) {
      marker.bindPopup(mapAdminPopup.getPopupElementContent(popupContent));
      marker._popup.setHeaderContent(mapAdminPopup.getPopupElementHeader(popupHeader));
      marker._popup.setFooterContent(mapAdminPopup.getPopupElementFooter(id));
    },

    /**
     * Save popup for the marker.
     */
    savePopup: function(event, id) {
      var newHeader = $(mapAdmin.markers[id]._popup._wrapper).find('input').val();
      var newContent = $(mapAdmin.markers[id]._popup._wrapper).find('textarea').val();
      mapAdmin.markers[id]._popup.setHeaderContent(mapAdminPopup.getPopupElementHeader(newHeader));
      mapAdmin.markers[id]._popup.setContent(mapAdminPopup.getPopupElementContent(newContent));
      mapAdmin.markers[id]._popup._closeButton.click();
      event.preventDefault();
      mapAdmin.saveData();
    },

    /**
     * Remove marker.
     */
    removeMarker: function(event, id) {
      mapAdmin.markers[id].remove();
      delete mapAdmin.markers[id];
      // Recreate markers.
      var newMarkers = [];
      $.each(mapAdmin.markers, function(key, val) {
        if (typeof val != 'undefined') {
          newMarkers[newMarkers.length] = val;
        }
      });
      mapAdmin.markers = newMarkers;

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
    getPopupElementFooter: function(id) {
      return '<br /><button onclick="mapAdminPopup.savePopup(event, ' + id + ')">' + Drupal.t('Save') + '</button>'
        + '<button onclick="mapAdminPopup.removeMarker(event, ' + id + ')">' + Drupal.t('Remove') + '</button>';
    }

  };

})(jQuery);

DG.then(function () {
  mapAdmin.map = DG.map('map', {
    center: Drupal.settings.dgis_map_data.center,
    zoom: Drupal.settings.dgis_map_data.zoom
  });

  mapAdmin.init();
  mapAdmin.map.on('move', mapAdmin.setMapState);
});
