
/**
 * @file
 * Javascript functions for getdirections module in colorbox
 *
 * @author Bob Hutchinson http://drupal.org/user/52366
 * jquery stuff
*/

(function ($) {
  Drupal.behaviors.getdirections_box = {
    attach: function() {
      // hide the returnlinks in a box
      $(".getdirections_returnlink").hide();
    }
  }
})(jQuery);
