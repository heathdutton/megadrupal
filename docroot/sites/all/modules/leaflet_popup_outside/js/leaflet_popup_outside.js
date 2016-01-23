(function ($) {

  /**
   * For leafleat API docs, see:
   * http://leafletjs.com/reference.html
   **/

  var cnt = 0;
  // Attach to Leaflet object via its API
  $(document).bind('leaflet.feature', function(e, lFeature, feature) {
    // Remove default click event (infoWindow Popup) and add our own
    if (feature.label) {
      var contents = feature.contents;
      lFeature.on('click', function() {
        var img = $(lFeature._icon);
        img.addClass('active').siblings().removeClass('active');
        showInfoAnywhere(feature);
      });
      lFeature.unbindPopup();
      cnt++;
    }
    if (cnt == 1) {
      // Open first marker's info by default (I'd prefer to have raised a click() event but coulnd't figure it out)
      // Tip: Sort results by proximity ASC in its View to auto-open nearest location
      showInfoAnywhere(feature);
    }
  });

  function showInfoAnywhere(marker) {
    var el = $('#info-anywhere');
    var popup = marker.popup;
    if (el.length) {
      el.html(popup);
    } else {
      // Since #info-anywhere element does not yet exist, create it now
      $('#leaflet-map').after('<div id="info-anywhere">' + popup + '</div>');
    }
  }

})(jQuery);
