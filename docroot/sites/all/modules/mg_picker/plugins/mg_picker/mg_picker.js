/**
 * @file
 *
 *
 *
 * @author Kálmán Hosszu - hosszu.kalman@gmail.com
 */

(function ($) {

Drupal.wysiwyg.plugins['mg_picker'] = {

  /**
   * Execute the button.
   */
  invoke: function(data, settings, instanceId) {
    Drupal.wysiwyg.instances[instanceId].openDialog(settings.dialog, data);
  }
}

})(jQuery);