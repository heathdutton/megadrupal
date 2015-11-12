
/**
 * @file
 * This file contains the javascript functions used by the baidu map field
 * widget.
 */

/**
 * Declare global variable by which to identify the map.
 */
var baidumap_field_map;

/**
 * Add code to generate the map on page load.
 */
(function ($) {
  Drupal.behaviors.baidumap_field = {
    attach: function (context) {
      // Get the settings for the map from the Drupal.settings object.
      var fname = Drupal.settings.gmf_widget_form['fname'];
      var lat = $("#edit-"+fname+"-und-0-lat").val();
      var lng = $("#edit-"+fname+"-und-0-lng").val();
      var zoom = parseInt($("#edit-"+fname+"-und-0-zoom").val());

      map = new BMap.Map('baidumap_picker');
      var point = new BMap.Point(lng, lat);
      map.centerAndZoom(point, zoom);
      var marker = new BMap.Marker(point);
      map.addOverlay(marker);
      map.addControl(new BMap.NavigationControl());
      map.enableScrollWheelZoom(true);

      map.addEventListener("click", function(e){
        map.clearOverlays();
        var point = new BMap.Point(e.point.lng, e.point.lat);
        var new_marker = new BMap.Marker(point);  // create new marker
        map.addOverlay(new_marker);

        document.getElementById("edit-"+fname+"-und-0-lat").value = e.point.lat;
        document.getElementById("edit-"+fname+"-und-0-lng").value = e.point.lng;
      });

      map.addEventListener("zoomend", function(){
        document.getElementById("edit-"+fname+"-und-0-zoom").value = this.getZoom();
      });
    }
  };
})(jQuery);

