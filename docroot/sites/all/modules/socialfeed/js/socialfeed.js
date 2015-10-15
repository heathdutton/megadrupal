/**
 * @file
 * Javascript and jQuery functions which are useful for the socialfeed module.
 */

(function($) {
  Drupal.behaviors.socialfeed = {
    attach: function (context, settings) {
      console.log(Drupal.settings.socialfeed.form_state);
      $("#instagram").jqinstapics({
        "user_id": Drupal.settings.socialfeed.instagram_user_id,
        "access_token": Drupal.settings.socialfeed.instagram_access_token,
        "count": Drupal.settings.socialfeed.instagram_picture_count,
      });
    }
  };
})(jQuery);
