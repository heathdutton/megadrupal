// $Id$

(function ($) {

  Drupal.Webcam = Drupal.Webcam || {};
  Drupal.Webcam.timers = Drupal.Webcam.timers || {};
  Drupal.Webcam.timeouts = Drupal.Webcam.timeouts || {};

  Drupal.Webcam.refresh = function(id) {
    var webcam = Drupal.settings.webcam.webcams[id];

    if (webcam.timeout > 0 && Drupal.Webcam.timeouts[id]++ > webcam.timeout) {
      clearInterval(Drupal.Webcam.timers[id]);
      Drupal.Webcam.error(Drupal.t('Webcam %title has timed out...', {'!title': webcam.title}));
    }
    else {
      var img = document.createElement('img');
      $(img)
        .load(function() {
          // Replace webcam image.
          $('#' + id + ' img.webcam-image')
            .attr('src', img.src)
            .css('width', webcam.width)
            .css('height', webcam.height);

          // Colorbox integration.
          if (webcam.lightbox) {
            // Update link.
            $('#' + id + ' a.colorbox')
              .attr('href', img.src);

            // Update zoomed image.
            if (typeof $.colorbox.element == 'function') {
              var colorbox = $.colorbox.element();
              if ($(colorbox).attr('id') == id + '-zoom' || $(colorbox).attr('id') == id + '-link') {
                $('#cboxPhoto')
                  .attr('src', img.src);
              }
            }
          }
        })
        .error(function() {
          $('#' + id + ' img.webcam-image')
            .attr('src', webcam.url_default)
            .css('width', webcam.width)
            .css('height', webcam.height);

          // @todo Add error messages to the colorbox?
        })
        // @todo Fix the hard-coded question mark. If an image url already
        //   includes it we need to add an ampersand instead.
        .attr('src', webcam.url + "?" + Math.random());
    }
  }

  Drupal.Webcam.error = function(msg) {
    if (Drupal.Webcam.error.errors !== true) {
      $('#' + Drupal.settings.webcam.errorId).prepend('<div class="error"></div>');
      Drupal.Webcam.error.errors = true;
    }

    $('#' + Drupal.settings.webcam.errorId + ' .error').append(msg + '<br />');
  }

  Drupal.behaviors.webcam = {
    attach: function (context, settings) {
      for (id in settings.webcam.webcams) {
        Drupal.Webcam.timers[id] = setInterval("Drupal.Webcam.refresh('" + id + "')", settings.webcam.webcams[id].delay * 1000);
        Drupal.Webcam.timeouts[id] = 0;
        Drupal.Webcam.refresh(id);
      }
    }
  };

})(jQuery);
