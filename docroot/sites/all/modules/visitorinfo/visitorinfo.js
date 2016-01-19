
Drupal.behaviors.visitorinfo = function (context) {
  var map;
  var descrip;
  if (GBrowserIsCompatible()) {
    map = new GMap2(document.getElementById("map"));
    map.addControl(new GSmallMapControl());
    map.addControl(new GMenuMapTypeControl());
    if (Drupal.settings.disableScrollZoom) {
      map.disableScrollWheelZoom();
    } else {
      map.enableScrollWheelZoom();
    }
    map.setCenter(new GLatLng(Drupal.settings.centerLatitude, Drupal.settings.centerLongitude), parseInt(Drupal.settings.startZoom));
    for(id in visitors) {
     if (visitors[id].state) {
        descrip = "IP: " + visitors[id].ip + "<br>" + "City: " + visitors[id].city + "<br>" + "State: " + visitors[id].state + "<br>" + "Country: " + visitors[id].cname;
        addMarker(visitors[id].latitude, visitors[id].longitude, descrip);
      }  
    }
  }
  
  function addMarker(latitude, longitude, description) {
    var marker = new GMarker(new GLatLng(latitude, longitude));
    GEvent.addListener(marker, 'click',
      function() {
        marker.openInfoWindowHtml(description);
      }
    );
    map.addOverlay(marker);
  }

};
$(window).unload( function () {
  GUnload();
});
