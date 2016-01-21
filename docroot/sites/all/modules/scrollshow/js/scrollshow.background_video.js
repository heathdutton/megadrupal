
;Drupal.scrollshow = Drupal.scrollshow || {effects: {}};

(function ($) {
  var hiresTimer = null,
      aspectRatio = null;

  function zfill(num, size) {
    var s = num+"";
    while (s.length < size) s = "0" + s;
    return s;
  }

  function getImage(idx, type, settings) {
    var path = settings[type + '_path'] || ""
    return path.replace('[index]', zfill(idx, settings.zfill || 0));
  }

  function getAspectRatio(settings) {
    var src = getImage(settings.first_index, 'lores', settings),
        img = $('img.scrollshow-preload-image[src="' + src + '"]');

    if (img.length && img.height()) {
      return img.width() / img.height();
    }

    return null;
  }

  Drupal.scrollshow.effects.background_video = {
    setup: function (settings) {
      var i;

      // create the div/img to display the background images
      $.each(['lores', 'hires'], function () {
        $('<div></div>')
          .addClass('scrollshow-background-video')
          .addClass('scrollshow-background-video-' + this)
          .css({
            position: 'fixed',
            top: 0,
            left: 0,
            width: '100%',
            overflow: 'hidden',
            zIndex: -99999
          })
          .append($('<img />').css({width: '100%', height: '100%'}))
          .appendTo($('body'));
      });

      // Maintain aspect ratio fails under Android... :-/
      if (Drupal.scrollshow.browser.Android) {
        settings.maintain_aspect_ratio = false;
      }
    },

    getPreloadImages: function (settings) {
      var images = [];
      for (i = settings.first_index; i <= settings.last_index; i++) {
        images.push(getImage(i, 'lores', settings));
      }
      return images;
    },

    resize: function (layout, settings) {
      var windowHeight = layout.windowHeight,
          windowWidth  = layout.windowWidth,
          menuOffset   = Drupal.scrollshow.getMenuOffset(),
          resizeResult = null;

      // make room for the menu at the top
      windowHeight = windowHeight - menuOffset.top;
      $('.scrollshow-background-video').css('margin-top', menuOffset.top);

      // make the wrapper the full size
      $('.scrollshow-background-video').height(windowHeight);

      // resize the image, maintaining the aspect ratio
      if (settings.maintain_aspect_ratio) {
        if (aspectRatio === null) {
          aspectRatio = getAspectRatio(settings);
        }
        if (aspectRatio !== null) {
          // calculate the resize dimensions
          resizeResult = Drupal.scrollshow.calculateResizeDimensions(aspectRatio, windowWidth, windowHeight,
            settings.maintain_aspect_ratio_full);

          $('.scrollshow-background-video img')
            .css({
              position: 'relative',
              width: resizeResult.width,
              height: resizeResult.height,
              left: resizeResult.offsetX,
              top: resizeResult.offsetY
            });
        }
      }
    },

    scroll: function (layout, settings) {
      var scrollTop = layout.scrollTop,
          scrollHeight = layout.scrollHeight,
          idx;

      idx = (settings.last_index - settings.first_index) *
            (scrollTop / scrollHeight);
      idx = Math.round(idx) + settings.first_index;

      // show the lo-res image in both backgrounds (prevents a choppy
      // transition from one to the other)
      $('.scrollshow-background-video img')
        .attr('src', getImage(idx, 'lores', settings));

      // hide the hi-res image
      $('.scrollshow-background-video-hires img')
        .stop(true)
        .css('opacity', 0);

      if (hiresTimer) {
        window.clearTimeout(hiresTimer);
      }
      hiresTimer = window.setTimeout(function () {
        var hires_img = $('.scrollshow-background-video-hires img');
        hires_img
          .attr('src', getImage(idx, 'hires', settings))
          .load(function () {
            if (Drupal.scrollshow.browser.Android) {
              // Android is too slow for this... :-/
              hires_img.css('opacity', 1.0);
            }
            else {
              hires_img.fadeTo(settings.hires_fadein_duration, 1.0, 'linear');
            }
          });
      }, 250);
    }
  };
})(jQuery);

