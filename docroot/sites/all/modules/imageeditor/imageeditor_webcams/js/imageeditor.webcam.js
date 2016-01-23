/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.behaviors.imageeditor_webcam = {};
  Drupal.behaviors.imageeditor_webcam.attach = function(context, settings) {
    $('#imageeditor_webcam', context).webcam({
      swffile: Drupal.settings.imageeditor.webcam.swffile,
      mode: 'save',
      onTick: function(remain) {
        if (0 === remain) {
          $('#imageeditor_webcam_status').html('Cheese!');
        }
        else {
          $('#imageeditor_webcam_status').html(remain + ' seconds remaining...');
        }
      },
      onSave: function(data) {
        alert(data);
      },
      onCapture: function () {
        webcam.save('/imageeditor/ajax/save/imageeditor_webcam');
      },
      debug: function (type, string) {
        $('#imageeditor_webcam_status').html(type + ': ' + string);
      },
      onLoad: function() {
        var cams = webcam.getCameraList();
        for (var i in cams) {
          $('#imageeditor_webcam_cams').append('<li>' + cams[i] + '</li>');
        }
        $('#imageeditor_webcam_capture').click(function(event) {
          $('#imageeditor_webcam_status').html('Cheese!');
          webcam.capture();
        });
        $('#imageeditor_webcam_capture5').click(function(event) {
          webcam.capture(5);
        });
      }
    });
  };

})(jQuery);
