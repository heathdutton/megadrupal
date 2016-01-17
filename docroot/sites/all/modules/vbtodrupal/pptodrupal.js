
/**
 * @file
 * Photopost javascript behaviours.
 */

(function ($) {
  Drupal.behaviors.pptodrupal = {
    attach: function (context, settings) {
      $('.photopost-forum-code').focus(function (e) {
        this.select();
      });

      $('.photopost-forum-code').mouseup(function (e) {
        return false;
      });
    }
  };
}) (jQuery);
