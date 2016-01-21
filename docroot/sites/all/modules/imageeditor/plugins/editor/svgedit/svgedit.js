/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.editors.svgedit = Drupal.imageeditor.editors.svgedit || {};
  Drupal.imageeditor.editors.svgedit.initialize = function($imageeditor_div) {
    $imageeditor_div.find('.svgedit').not('.imageeditor-processed').addClass('imageeditor-processed').click(function(event) {
      event.preventDefault();
      event.stopPropagation();
      var data = $imageeditor_div.data();
      Drupal.imageeditor.save = data.callback;
      var codename = $(this).data('codename');
      Drupal.settings.imageeditor.save = data;
      var options = Drupal.settings.imageeditor[codename].options;
      if (typeof(data.url) !== 'undefined') {
        options[Drupal.settings.imageeditor[codename].image_url_param] = data.url;
        Drupal.settings.imageeditor.save.create = 0;
        var filename = data.origurl.replace(new RegExp('.*/', 'g'), '');
        $.cookie('imageeditor_filename', filename, {expires: 7, path: Drupal.settings.basePath});
      }
      else {
        delete options[Drupal.settings.imageeditor[codename].image_url_param];
        Drupal.settings.imageeditor.save.create = 1;
      }
      Drupal.imageeditor[Drupal.settings.imageeditor[codename].launch_type].show(options, $(this).attr('title'));
    });
  };

})(jQuery);
