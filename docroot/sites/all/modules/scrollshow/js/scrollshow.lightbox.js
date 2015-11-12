
;Drupal.scrollshow = Drupal.scrollshow || {effects: {}};

(function ($) {
  var external_re = new RegExp('^((f|ht)tps?:)?//(?!' + window.location.host + ')');

  // Returns true if the given link is external
  function isExternalLink(elem) {
    return external_re.test($(elem).attr('href'));
  }

  // Monkeypatch around Lightbox.changeData() so we can resize the container for
  // the iframe contents.
  function LightboxChangeData(imageNum, zoomIn) {
    var res = Lightbox._changeData(imageNum, zoomIn),
        iframe = $('iframe#lightboxFrame');

    iframe.load(function () {
      setTimeout(function () {
        var windowWidth = $(window).width(),
            windowHeight = $(window).height(),
            maxHeight = Math.min(windowHeight * 0.70, windowHeight - 100);
            iframeWidth  = iframe.width(),
            iframeHeight = $('.scrollshow-page-wrapper', iframe.contents()).height() + 20;

        if (iframeHeight <= 0 || iframeHeight > maxHeight) {
          iframeHeight = maxHeight;
        }
        else {
          $(iframe.get(0).contentWindow.document.body).css('overflow', 'hidden');
        }

        iframe.css({'height': iframeHeight + 'px'});
        Lightbox.resizeContainer(iframeWidth, iframeHeight);
      }, 600);

      // TODO: this timeout actually depends on Lightbox.resizeSpeed, but I
      // didn't want to have to convert those, so it's just set to 'slow' = 600
    });

    return res;
  }

  Drupal.scrollshow.effects.lightbox = {
    setup: function (settings) {
      // Monkeypatch Lightbox if it is present
      if (typeof Lightbox !== 'undefined' && Lightbox.changeData !== LightboxChangeData) {
        Lightbox._changeData = Lightbox.changeData;
        Lightbox.changeData  = LightboxChangeData;
      }
    },

    attach: function (context, settings) {
      var added_lightbox_attr = false;

      // if the given context isn't a child of .scrollshow-slide (ie. some
      // AJAX content reloaded inside a node), then we attempt to get the
      // .scrollshow-slides from within the context (ie. when this is called
      // for the full page).
      if ($(context).closest('.scrollshow-slides').length == 0) {
        context = $('.scrollshow-slides', context);
      }

      $('a', context).once('scrollshow', function () {
        var slide_id = $(this).closest('.scrollshow-slide').attr('id'),
            lightbox = 'lightframe' + (slide_id ? '[' + slide_id + ']' : '');

        // Don't open internal anchor links into lightboxes!
        if ($(this).attr('href').indexOf('#') === 0) {
          return;
        }

        // Don't open links intended for a new window into the lightbox!
        if ($(this).attr('target') === '_blank') {
          return;
        }

        // open any links in a new tab, so that users don't leave this page
        $(this).attr('target', '_blank');

        if (!isExternalLink(this) && $(this).is(':visible')) {
          // tell lightbox that it should open it
          $(this).attr('rel', lightbox);

          // append a special GET argument to open without the normal chrome
          this.href = this.href + (this.href.indexOf('?') == -1 ? '?' : '&') + 'scrollshow=1';

          // mark that we need to make sure lightbox runs
          added_lightbox_attr = true;
        }
      });

      if (added_lightbox_attr) {
        // re-run attach behaviors to get lightbox to catch our new links
        Drupal.attachBehaviors(context);
      }
    },

    resize: function (layout, settings) {
      if (typeof Lightbox !== 'undefined') {
        Lightbox.iframe_width = Math.min(layout.windowWidth * 0.95, layout.windowWidth - 40);
      }
    }
  };
})(jQuery);

