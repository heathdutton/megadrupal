/**
 * Callbacks for plupload.
 */

(function($) { 
  Drupal.filedepot_multiupload = Drupal.filedepot_multiupload || {};
  Drupal.filedepot_multiupload.stateChangedCallback = function(up, files) {
    if (up.state === 2) {
      var folder = jQuery("#edit-filedepot-folder").val();
      if (folder <= 0) {
        up.stop();
        jQuery("#edit-filedepot-folder").addClass("error");
        alert(Drupal.t("You must select a valid folder"));
      }
    }
  };
})(jQuery);
