/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function($) {
  "use strict";

  Drupal.yashare_standard = {};
  Drupal.yashare_standard.initWidgets = function (context, settings) {
    $.each(settings.yashare, function (index, value) {
      $('#' + index + ':not(.yashare-processed)', context).addClass('yashare-processed').each(function () {
        new Ya.share(value);
      });
    });
  };

  Drupal.behaviors.yashare_standard = {};
  Drupal.behaviors.yashare_standard.attach = function (context, settings) {
    Drupal.yashare.initialize(Drupal.yashare_standard.initWidgets, context, settings);
  };

})(jQuery);
