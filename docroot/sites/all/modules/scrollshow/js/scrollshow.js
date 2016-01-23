
;Drupal.scrollshow = Drupal.scrollshow || {effects: {}};

(function ($) {
  // Build up the API around the Drupal.scrollshow object.

  Drupal.scrollshow.eachEffect = function (callback) {
    // Always execute the 'default' effect first!
    callback.apply(this.effects['default'], ['default', this.effects['default']]);
    $.each(this.effects, function (name) {
      if (name !== 'default') {
        callback.apply(this, [name, this]);
      }
    });
  };

  Drupal.scrollshow.setup = function (settings) {
    this.eachEffect(function (name) {
      if (this.setup) {
        this.setup(settings[name]);
      }
    });
  };

  Drupal.scrollshow.resize = function (settings) {
    var layout = this.getLayout();
    this.eachEffect(function (name) {
      if (this.resize) {
        this.resize(layout, settings[name]);
      }
    });
  };

  Drupal.scrollshow.scroll = function (settings) {
    var layout = this.getLayout();
    this.eachEffect(function (name) {
      if (this.scroll) {
        this.scroll(layout, settings[name]);
      }
    });
  };

  Drupal.scrollshow.getPreloadImages = function (settings) {
    var images = [];
    this.eachEffect(function (name) {
      if (this.getPreloadImages) {
        images = images.concat(this.getPreloadImages(settings[name]));
      }
    });
    return images;
  };

  Drupal.scrollshow.attach = function (context, settings) {
    this.eachEffect(function (name) {
      if (this.attach) {
        this.attach(context, settings[name]);
      }
    });
  };

  Drupal.scrollshow.detach = function (context, settings) {
    this.eachEffect(function (name) {
      if (this.detach) {
        this.detach(context, settings[name]);
      }
    });
  };

  // A utility function to grab layout information about the current
  // state of the browser window.
  Drupal.scrollshow.getLayout = function () {
    var layout = {
      windowWidth: $(window).width(),
      windowHeight: $(window).height(),
      scrollTop: $(window).scrollTop(),
      scrollHeight: $('body').scrollHeight
    };

    // For older browsers
    if (!layout.scrollHeight) {
      layout.scrollHeight = $(document).height() - layout.windowHeight;
    }

    return layout;
  };

  // A utility function to help resize images to in a way that maintains
  // the aspect ratio.
  Drupal.scrollshow.calculateResizeDimensions = function (aspectRatio, width, height, full) {
    var result = {width: width, height: width / aspectRatio, offsetX: 0, offsetY: 0};

    if (full && result.height < height) {
      // if the full height isn't filled, then we rebase the calculation on
      // the height.
      result.height = height;
      result.width = height * aspectRatio;
      result.offsetX = -(result.width - width) / 2;
    }

    return result;
  };

  // A utility function for getting the menu offset, so we can avoid covering
  // things with the menu.
  Drupal.scrollshow.getMenuOffset = function () {
    var menu          = $('.scrollshow-menu-wrapper'),
        menuTop       = menu.offset().top - $(window).scrollTop(),
        menuHeight    = menu.height(),
        offset        = {top: 0, left: 0, bottom: 0, right: 0};

      // make room for the menu at the top
      if (menuTop == 0) {
        offset.top = menuHeight;
      }

      return offset;
  };

  // Some browser checking...
  Drupal.scrollshow.browser = {
    iOS: /iPad|iPhone|iPod/i.test(navigator.userAgent),
    Android: /Android/i.test(navigator.userAgent),
    Silk: /Silk/i.test(navigator.userAgent)
  };

  // Create a dialog and perform a preload
  function preload(preloadImages, settings, complete) {
    var dialog, progressbar, step, destroy, current = 0, i;

    dialog = $('<div></div>')
      .appendTo($('body'))
      .dialog({
        title: Drupal.t('Loading...'),
        dialogClass: 'scrollshow-preload-dialog',
        autoOpen: false,
        draggable: false,
        resizable: false,
        closeOnEscape: false,
        modal: true
      });

    progressbar = $('<div></div>')
      .appendTo(dialog)
      .css('margin-bottom', '1em')
      .progressbar({
        max: preloadImages.length
      });

    $('#scrollshow-fallback-link')
      .clone()
      .attr('id', '')
      .appendTo(dialog);

    dialog.dialog('open');

    // a recursive function which will progressively preload all the images
    step = function () {
      var img = $('<img />')
        .appendTo($('body'))
        .addClass('scrollshow-preload-image')
        .css('display', 'none');

      img.attr('src', preloadImages[current++]);

      img.load(function() {
          if (current >= preloadImages.length) {
            dialog.dialog('close');

            // clean-up
            window.setTimeout(destroy, 0);
          }
          else {
            progressbar.progressbar('option', 'value', current);

            // load the next image!
            window.setTimeout(step, 0);
          }
        });

        // IE7-8 will fail to trigger the load event if the image is already
        // in the cache, so we trigger it ourselves.
        if (img[0].complete || img[0].naturalWidth > 0) {
          img.load();
        }
    };

    // tear down the jquery ui components
    // TODO: should we remove the images too?
    destroy = function() {
      progressbar
        .progressbar('destroy')
        .remove();
      dialog
        .dialog('destroy')
        .remove();

      // finally, run our complete function!
      complete();
    };

    // load 4 images at once...
    for (i = 0; i < 4; i++) {
      step();
    }
  }

  // Actually call API functions on Drupal.scrollshow for this page.
  Drupal.behaviors.scrollshow = {
    attach: function (context, settings) {
      settings = settings.scrollshow || {};
      $('body').once('scrollshow', function () {
        var resize = function () { Drupal.scrollshow.resize(settings); },
            scroll = function () { Drupal.scrollshow.scroll(settings); },
            complete = function () { resize(); scroll(); },
            preloadImages;

        Drupal.scrollshow.setup(settings);

        // re-layout on resize and scroll
        $(window).resize(resize);
        $(window).scroll(scroll);

        // layout once to get us started!
        complete();

        // preload any images needed by any of the effects
        preloadImages = Drupal.scrollshow.getPreloadImages(settings);
        if (preloadImages.length) {
          preload(preloadImages, settings, complete);
        }
      });

      // If this function is called again, it means that new content has
      // been loaded (ie. via AJAX) and we need to attach to it.
      Drupal.scrollshow.attach(context, settings);
    },
    detach: function (context, settings) {
      settings = settings.scrollshow || {};
      Drupal.scrollshow.detach(context, settings);
    }
  };
})(jQuery);

