/**
 * Implements Drupal.behaviors
 */
Drupal.behaviors.gmap_image_field_pin = {
  attach : function(context, settings) {

    if (!settings['gmapImageFieldPinMaps']) {
      return;
    }

    for (i in settings.gmapImageFieldPinMaps) {
      if (settings.gmapImageFieldPinMaps[i]) {
        Drupal.settings.gmapImageFieldPinMaps[i].o = gmapImageField_createMap(settings.gmapImageFieldPinMaps[i]);
        Drupal.settings.gmapImageFieldPinMaps[i].o.markThePlace(settings.gmapImageFieldPinMaps[i].id + '-latlng-input', jQuery('#' + settings.gmapImageFieldPinMaps[i].id + '-icon-changer').val());
        jQuery('#' + settings.gmapImageFieldPinMaps[i].id + '-icon-changer')
            .data('i', i)
            .unbind('change')
            .bind('change', function() {
              Drupal.settings.gmapImageFieldPinMaps[jQuery(this).data('i')].o.markThePlace(Drupal.settings.gmapImageFieldPinMaps[jQuery(this).data('i')].id + '-latlng-input', jQuery(this).val());
            });
      }
    }

    // When focusing fieldset, resize the map.
    // This action is need because when the vertical tab is hidden,
    // the map is initialized with nullable dimensions, so we need to resize it.
    jQuery('.vertical-tab-button a').once().click(function() {
      for (i in settings.gmapImageFieldPinMaps) {
        if (settings.gmapImageFieldPinMaps[i]['o']['map']) {
          google.maps.event.trigger(Drupal.settings.gmapImageFieldPinMaps[i].o.map, 'resize');
        }
      }
    });

  }
}
