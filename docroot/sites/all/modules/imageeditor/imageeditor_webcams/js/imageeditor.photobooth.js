/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.behaviors.imageeditor_photobooth = {};
  Drupal.behaviors.imageeditor_photobooth.attach = function(context, settings) {
    $('.imageeditor-webcams .controls-wrapper').hide();
    var $photobooth = $('.imageeditor-webcams .camera', context);
    var myPhotobooth = new Photobooth($photobooth);
    myPhotobooth.resize(640, 480);
    myPhotobooth.onImage = function(dataUrl) {
      var $gallery = $('.imageeditor-webcams .gallery', context);
      $gallery.append('<div class="gallery-thumb"><img src="' + dataUrl + '" width="128"></div>');
      var $thumbs = $gallery.find('.gallery-thumb');
      if ($thumbs.length > 5) {
        $thumbs.first().remove();
      }
      $thumbs.last().click(function() {
        myPhotobooth.destroy();
        $.ajax({
          type: 'POST',
          url: parent.Drupal.settings.imageeditor.imageeditor_photobooth.parameters.callback_url,
          async: false,
          data: {'data': $(this).find('img').attr('src'), 'url': parent.Drupal.settings.imageeditor.save.url},
          success: function(data) {
            parent.Drupal.settings.imageeditor.save.image = data;
            parent.Drupal.imageeditor.save();
            parent.Drupal.imageeditor.overlay.hide();
          },
          error: function(msg) {
            alert(Drupal.t('Failed to save the image') + ': ' + msg);
            parent.Drupal.imageeditor.overlay.hide();
          }
        });
      });
    };
  };

})(jQuery);
