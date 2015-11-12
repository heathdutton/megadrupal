/**
 * @file
 * Tooltip handling for the Profile Pic Changer module
 *
 * This should only get included when jquery_plugin is enabled
 */

(function ($) {
  Drupal.behaviors.ProfilePicChangerTooltip = {
    attach: function() {
      $('.user-picture img').tooltip({position: 'center right'});
    }
  };
})(jQuery);
