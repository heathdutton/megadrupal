/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.editors.deviantartmuro = Drupal.imageeditor.editors.deviantartmuro || {};
  Drupal.imageeditor.editors.deviantartmuro.initialize = function($imageeditor_div) {
    $imageeditor_div.find('.deviantartmuro').not('.imageeditor-processed').addClass('imageeditor-processed').click(function(event) {
      event.preventDefault();
      event.stopPropagation();
      var data = $imageeditor_div.data();
      Drupal.imageeditor.save = data.callback;
      Drupal.imageeditor.overlay.show({}, $(this).attr('title'));
      $('.imageeditor-modal').damuro({
        sandbox: Drupal.settings.imageeditor.deviantartmuro.options.sandbox
      });
    });
  };

})(jQuery);
