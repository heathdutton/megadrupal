/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.behaviors.yashare_admin = {};
  Drupal.behaviors.yashare_admin.attach = function(context, settings) {
    $('.dark', context).closest('.form-item').css('background-color', 'gray');
  };

})(jQuery);
