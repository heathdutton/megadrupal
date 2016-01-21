DG.then(function () {
  (function($) {
    $('.dgis-maps-container-view').each(function(key, mapContainer) {
      var mapData = $(mapContainer).next();
      mapData = $.parseJSON($(mapData).html());
      var mapId = $(this).attr('id');
      var map = DG.map(mapId, {
        center: mapData.center.split(','),
        zoom: mapData.zoom
      });

      if (mapData.markers) {
        var markers = $.parseJSON(mapData.markers);

        var marker = {};
        $.each(markers, function(id, markerData) {
          marker = DG.marker(markerData.point).addTo(map);
          if (markerData.popup.header || markerData.popup.content) {
            marker.bindPopup(markerData.popup.content, {minWidth: 200});
            marker._popup.setHeaderContent('<h3 class="dg-popup__header-title">' + markerData.popup.header + '</h3>');
          }
        });
      }
    });
  })(jQuery);
});
