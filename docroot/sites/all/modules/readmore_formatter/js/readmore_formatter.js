/**
 * readmore formatter
 * @file
 * this file contains js to initialize readmore js
 */
(function ($) {
Drupal.behaviors.readmoreSettings = {
  attach: function (context) {
    var readmoreSettings = Drupal.settings.readmoreSettings;
    // initialize readmore
    for (var key in readmoreSettings) {
      $('.field-readmore-' + key).readmore(readmoreSettings[key]);
    }
  }
};
})(jQuery);
