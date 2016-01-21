
;Drupal.scrollshow = Drupal.scrollshow || {effects: {}};

(function ($) {
  function createLayer(parent, url, info) {
    var layer = $('<div></div>')
      .appendTo(parent)
      .append($('<img />').attr('src', url))
      .addClass('scrollshow-parallax-layer')
      .css({
        position: 'absolute',
        zIndex: info['z_index'],
        pointerEvents: 'none',
        overflow: 'hidden'
      })
      .data('scrollshow-parallax', info)
      .hide();

    $(parent).css({position: 'relative'});

    return layer;
  }

  function repositionLayers(layout) {
    var windowHeight = layout.windowHeight,
        windowWidth = layout.windowWidth,
        scrollTop = layout.scrollTop;

    $('.scrollshow-parallax-layer').each(function (idx) {
      var layer = $(this),
          info = layer.data('scrollshow-parallax'),
          img = $('img', layer),
          imgWidth = img.width(),
          imgHeight = img.height(),
          x = windowWidth > imgWidth ? (windowWidth - imgWidth) * (info.x / 100.0) : 0,
          y = info.y >= 0
            ? (windowHeight > imgHeight ? (windowHeight - imgHeight) * (info.y / 100.0) : 0)
            : (windowHeight * (info.y / 100.0)),
          layerTop = layer.parent().offset().top + y,
          visible = (scrollTop + windowHeight >= layerTop),
          maxHeight = windowHeight + (windowHeight * Drupal.settings.scrollshow['default'].slidespacing),
          speed, delta;

      // If this slide is currently visible, we need to start animating it.
      if (visible) {
        if (info.direction == 'up') {
          speed = -((info.speed - 100) / 100.0);
        }
        else {
          speed = 1 * (info.speed / 100.0);
        }

        delta = scrollTop - layerTop;

        // add the additional pixels to y
        y = y + (delta * speed);
      }

      // clip horizontally
      if (x + imgWidth > windowWidth) {
        layer.width(windowWidth - x);
      }

      // clip vertically
      if (y + imgHeight > maxHeight) {
        layer.height(maxHeight - y);
      }

      layer
        .css({top: y, left: x})
        .show();
    });
  }

  Drupal.scrollshow.effects.parallax = {
    slides: [],

    setup: function (settings) {
      $('.scrollshow-slide').each(function (index) {
        var slide = settings.slides[index], i;
        if (slide) {
          for (i = 0; i < settings.max_layers; i++) {
            if (slide.layers[i]) {
              // create the layer
              createLayer(this, slide.layers[i], settings.layers[i]);
            }
          }
        }
      });
    },

    resize: function (layout, settings) {
      repositionLayers(layout);
    },

    scroll: function (layout, settings) {
      repositionLayers(layout);
    }
  };
})(jQuery);

