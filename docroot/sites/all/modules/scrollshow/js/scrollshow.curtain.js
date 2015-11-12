
;Drupal.scrollshow = Drupal.scrollshow || {effects: {}};

(function ($) {
  var curtain = null, aspectRatio = 0;

  Drupal.scrollshow.effects.curtain = {
    setup: function (settings) {
      if (settings.image_url) {
        curtain = $('<img />');
        curtain
          .prependTo('.scrollshow-slides')
          .attr('src', settings.image_url)
          .attr('id', 'scrollshow-curtain')
          .addClass('scrollshow-curtain')
          .css('top', 0)
          .css('zIndex', settings.above_slides ? 10 : -1)
          .load(function () {
            aspectRatio = $(this).width() / $(this).height();
            setTimeout(function () {
              Drupal.scrollshow.resize(Drupal.settings.scrollshow);
            }, 0);
          });

      }
    },

    resize: function (layout, settings) {
      var slidespacing  = Drupal.settings.scrollshow['default'].slidespacing,
          curtainHeight = layout.windowHeight,
          menuOffset    = Drupal.scrollshow.getMenuOffset(),
          resizeResult  = null;

      // make room for the menu at the top
      curtainHeight = curtainHeight - menuOffset.top;
      curtain.css('margin-top', menuOffset.top);

      if (aspectRatio) {
          // calculate the resize dimensions
          resizeResult = Drupal.scrollshow.calculateResizeDimensions(aspectRatio,
            layout.windowWidth, curtainHeight, false);

          curtain
            .css({
              position: (settings.take_up_space && settings.effect == 'slide') ? 'relative' : 'fixed',
              width: resizeResult.width,
              height: resizeResult.height,
              left: resizeResult.offsetX,
              //top: resizeResult.offsetY
            });

          if (settings.take_up_space && slidespacing) {
            curtain.css('margin-bottom', layout.windowHeight * slidespacing);
          }

          if (settings.take_up_space && settings.effect == 'fade') {
            $('.scrollshow-slides .scrollshow-slide:first').css('margin-top', layout.windowHeight +
              (layout.windowHeight * slidespacing));
          }
      }
    },

    scroll: function (layout, settings) {
      var amount;
      if (curtain) {
        if (settings.effect == 'fade') {
          amount = layout.windowHeight - (layout.scrollTop * 2);
          if (amount < 0) {
            curtain.hide();
          }
          else {
            curtain
              .show()
              .css('opacity', amount / layout.windowHeight);
          }
        }
        else if (settings.effect == 'slide') {
          curtain.css('top', -layout.scrollTop);
        }
      }
    }
  };
})(jQuery);

