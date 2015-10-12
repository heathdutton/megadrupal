var map;

DG.then(function () {
  jQuery.each(Drupal.settings.dgis_map_data, function(key, mapData) {
    map = DG.map(mapData.mapId, {
      center: mapData.center,
      zoom: mapData.zoom
    });

    if (mapData.markers) {
      var markers = jQuery.parseJSON(mapData.markers);

      var marker = {};
      jQuery.each(markers, function(id, markerData) {
        marker = DG.marker(markerData.point).addTo(map);
        if (markerData.popup.header || markerData.popup.content) {
          marker.bindPopup(markerData.popup.content, {minWidth: 200});
          marker._popup.setHeaderContent('<h3 class="dg-popup__header-title">' + markerData.popup.header + '</h3>');
        }
      });
    }
  });
});
