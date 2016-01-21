/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.editors.picozu = Drupal.imageeditor.editors.picozu || {};
  Drupal.imageeditor.editors.picozu.initialize = function($imageeditor_div) {
    $imageeditor_div.find('.picozu').not('.imageeditor-processed').addClass('imageeditor-processed').click(function(event) {
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

  Drupal.behaviors.picozu = {};
  Drupal.behaviors.picozu.attach = function(context, settings) {
    if (!$('body').hasClass('picozu-attached')) {
      $('body').addClass('picozu-attached');
      // See https://www.picozu.com/developers/embedding/
      var eventMethod = window.addEventListener ? 'addEventListener' : 'attachEvent';
      var eventer = window[eventMethod];
      var messageEvent = eventMethod == 'attachEvent' ? 'onmessage' : 'message';
      eventer(messageEvent, function(e) {
        // Check if the origin is the Picozu domain with https.
        if (e.origin === 'https://www.picozu.com' && e.data.url) {
          Drupal.imageeditor.overlay.hide();
          Drupal.settings.imageeditor.save.image = e.data.url;
          Drupal.imageeditor.save();
        }
      }, false);
    }
  };

})(jQuery);
