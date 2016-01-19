/**
 * @file
 * UI handling for the Profile Pic Changer module
 */

(function ($) {
  Drupal.ProfilePicChanger = Drupal.ProfilePicChanger || {};

  Drupal.ProfilePicChanger.pic_updated = function(context, params) {
    var new_img = params['argument'];
    $('.user-picture img').attr('src', new_img);
  };

  Drupal.behaviors.ProfilePicChanger = {
    attach: function() {
      // change the title so the tooltip text indicates what to do
      $('.user-picture img').attr('title', Drupal.t("Click on the image to change your profile picture"));
    }
  };

  Drupal.ajax.prototype.commands.pic_updated = Drupal.ProfilePicChanger.pic_updated;

})(jQuery);
