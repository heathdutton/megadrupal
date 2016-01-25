/**
 * @file
 * Stylemap Behavior
 */
(function($) {
  /**
   * Behavior for Stylemaps
   */
  Drupal.behaviors.openlayers_behavior_stylemap = {
    'attach': function(context, settings) {

      // Start behavior process
      var data = $(context).data('openlayers');
      if (data && data.map.behaviors['openlayers_behavior_stylemap']) {
        var style = [{
          'featureType': "all", 
          'elementType': "all", 
          'stylers': [ 
            { 'visibility': data.map.behaviors['openlayers_behavior_stylemap'].all[0].visibility },
            { 'invert_lightness': data.map.behaviors['openlayers_behavior_stylemap'].all[0].invert_lightness },
            { 'hue': data.map.behaviors['openlayers_behavior_stylemap'].all[0].hue },
            { 'lightness': data.map.behaviors['openlayers_behavior_stylemap'].all[0].lightness },
            { 'saturation': data.map.behaviors['openlayers_behavior_stylemap'].all[0].saturation },
            { 'gamma': data.map.behaviors['openlayers_behavior_stylemap'].all[0].gamma }
          ] 
        }];
        if (data.map.behaviors['openlayers_behavior_stylemap'].mode == 'specific') {
          try { 
            style = jQuery.parseJSON(data.map.behaviors['openlayers_behavior_stylemap'].specific[0].json);
          }
          catch (e) { /* error */ }
        }
        data.openlayers.baseLayer.mapObject.setOptions({styles: style});
      }
    

    }
  };
})(jQuery);
