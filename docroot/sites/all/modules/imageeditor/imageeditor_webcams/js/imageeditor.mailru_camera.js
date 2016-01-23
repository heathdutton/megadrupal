/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.behaviors.imageeditor_mailru_camera = {};
  Drupal.behaviors.imageeditor_mailru_camera.attach = function(context, settings) {
    var el = $('.imageeditor-webcams .camera', context);
    FileAPI.Camera.publish(el, {width: 640, height: 480}, function(err, cam) {
      if (!err) {
        $('.imageeditor-webcams .controls-shot').click(function() {
          var shot = cam.shot();
          shot.get(function(err, img) {
            var $gallery = $('.imageeditor-webcams .gallery', context);
            $gallery.append('<div class="gallery-thumb"><img src="' + img.toDataURL() + '" width="128"></div>');
            var $thumbs = $gallery.find('.gallery-thumb');
            if ($thumbs.length > 5) {
              $thumbs.first().remove();
            }
            $thumbs.last().click(function() {
              cam.stop();
              $.ajax({
                type: 'POST',
                url: parent.Drupal.settings.imageeditor.imageeditor_mailru_camera.parameters.callback_url,
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
          });
        });
      }
      else {
        alert('Unfortunately, your browser does not support webcam.');
      }
    });
  };

})(jQuery);
