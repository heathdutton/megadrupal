
(function ($) {

  $(document).bind('leaflet.feature', function(e, lFeature, feature) {
    if (feature.label) {
      var isMarker = lFeature.options && lFeature.options.icon;
      var options = {
        className: isMarker ? 'leaflet-label-marker' : 'leaflet-label-other',
        direction: 'auto'
      };
      if (isMarker) {
        // lFeature.options.icon.options.iconsize = [25, 41] (std. Leaflet marker)
        // lFeature.options.icon.options.iconsize = [32, 42] (IPGV&M marker)
        var x = 23;
        var y = -42;
        options.offset = [x, y];
      }
      options.noHide = false;
      options.pane = 'popupPane';
      lFeature.bindLabel(feature.label, options);
    }
  });

})(jQuery);
