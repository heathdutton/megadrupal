/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.paintweb = {
    save: function(ev) {
      ev.preventDefault();
      Drupal.imageeditor.overlay.hide();

      $.ajax({
        type: 'POST',
        url: Drupal.settings.imageeditor.paintweb.options.saveurl,
        async: false, // explicitly need the user to wait while we load...
        data: {
          'data': ev.dataURL,
          'url': Drupal.settings.imageeditor.url
        },
        success: function(data) {
          ev.target.events.dispatch(new pwlib.appEvent.imageSaveResult(true));
          // Destroy the PaintWeb instance.
          ev.target.destroy();
          Drupal.settings.imageeditor.url = 'undefined';
          Drupal.settings.imageeditor.save.image = data;
          Drupal.imageeditor.save();
        },
        error: function(msg) {
          ev.target.events.dispatch(new pwlib.appEvent.imageSaveResult(false));
          // Destroy the PaintWeb instance.
          ev.target.destroy();
          Drupal.settings.imageeditor.url = 'undefined';
          alert(Drupal.t('Failed to save the image') + ': ' + msg);
        }
      });
    }
  };

  Drupal.imageeditor.editors.paintweb = Drupal.imageeditor.editors.paintweb || {};
  Drupal.imageeditor.editors.paintweb.initialize = function($imageeditor_div) {
    $imageeditor_div.find('.paintweb').not('.imageeditor-processed').addClass('imageeditor-processed').click(function(event) {
      event.preventDefault();
      event.stopPropagation();
      var data = $imageeditor_div.data();
      Drupal.imageeditor.save = data.callback;
      var pw = new PaintWeb(), img = document.createElement('img'), canvas;
      Drupal.settings.imageeditor.save = data;
      pw.config.configFile = Drupal.settings.imageeditor.paintweb.options.configFile;
      if (data.url) {
        Drupal.settings.imageeditor.save.create = 0;
        img.src = data.url;
        pw.config.imageLoad = img;
        Drupal.settings.imageeditor.url = data.url;
      }
      else {
        Drupal.settings.imageeditor.save.create = 1;
        canvas = document.createElement('canvas');
        canvas.setAttribute('width', 600);
        canvas.setAttribute('height', 600);
        img.src = canvas.toDataURL('image/png');
        pw.config.imageLoad = img;
        Drupal.settings.imageeditor.url = 'undefined';
      }
      Drupal.imageeditor.overlay.show({}, $(this).attr('title'));
      var $placeholder = $('.imageeditor-modal');
      pw.config.guiPlaceholder = $placeholder.get(0);
      pw.config.viewportWidth = $placeholder.width() + 'px';
      pw.config.viewportHeight = ($placeholder.height() - 140) + 'px';
      pw.init(function (ev) {
        if (ev.state === PaintWeb.INIT_ERROR) {
          alert(Drupal.t('PaintWeb failed to load.'));
        }
      });
      pw.events.add('imageSave', Drupal.imageeditor.paintweb.save);
    });
  };

})(jQuery);
