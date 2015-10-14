/*
 * @file
 * JavaScript for commons_bw_ui.
 */

(function($) {

  // Re-enable form elements that are disabled for non-ajax situations.
  Drupal.behaviors.enableFormItemsForAjaxForms = {
    attach: function() {
    // If ajax is enabled.
    if (Drupal.ajax) {
      $('.enabled-for-ajax').removeAttr('disabled');
      $('.enabled-for-ajax').removeClass('form-button-disabled');
    }
  }
  };

})(jQuery);
