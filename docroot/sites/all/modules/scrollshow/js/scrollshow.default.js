
;Drupal.scrollshow = Drupal.scrollshow || {effects: {}};

(function ($) {
  // A shim to turn touch events into scroll events for iOS, because it
  // only fires scroll events after the scroll is completed.
  function setupTouchScrollShim() {
    var startY;

    document.addEventListener('touchstart', function (e) {
      startY = e.touches[0].pageY;
    });

    document.addEventListener('touchmove', function (e) {
      var deltaY = startY - e.touches[0].pageY;
      $('html,body').scrollTop($(document).scrollTop() + deltaY);
      e.preventDefault();
    });
  };

  // Sets up the menu and internal smooth scroll links
  function setupSmoothScroll(context, settings) {
    var path_re = /.+?\:\/\/.+?(\/.+?)(?:#|\?|$)/;

    // onClick event to smooth scroll to an internal fragment
    function onclick(evt) {
      var current    = window.location.protocol + '//' + window.location.hostname + window.location.pathname,
          menuOffset = Drupal.scrollshow.getMenuOffset(),
          index, fragment, destination;

      if (this.href.indexOf(current) === 0) {
        index = this.href.indexOf('#');
        if (index !== -1) {
          fragment = (this.href+"").substring(index);
          destination = $(fragment).offset().top;
          //destination = $(fragment).offset().top + menuOffset.top;
          // To fix scrolling to the top on the Kindle Fire
          if (destination == 0) {
            destination = 1;
          }
          $('html,body').animate(
            {scrollTop: destination},
            settings.smoothscroll_duration,
            settings.smoothscroll_easing
          );
          evt.preventDefault();
        }
      }
    }

    // The menu items for sure!
    $('.menu a', context).once('scrollshow').click(onclick);

    // Look for links to pages that are being used in the scrollshow and link
    // internally...
    $('a', context).not('.scrollshow-fallback-link a').each(function () {
      var link = this,
          matches = path_re.exec(link.href),
          path = matches.length > 1 ? matches[1] : null,
          fragment;

      if (path !== null) {
        // remove the leading slash
        if (path.indexOf('/') === 0) {
          path = path.substring(1);
        }

        // if the path matches, we convert it to an internal link and add the
        // smoothscroll onclick handler.
        fragment = settings.path2fragment[path];
        if (fragment) {
          $(link)
            .once('scrollshow')
            .attr('href', window.location + '#' + fragment)
            .click(onclick);
        }
      }
    });
  }

  // Update the active menu item based on the current scroll position
  function updateActiveMenu(layout) {
    var scrollTop    = layout.scrollTop,
        slidespacing = Drupal.settings.scrollshow['default'].slidespacing,
        currentSlide = null;

    $('.scrollshow-slide').each(function () {
      // Gives leeway of either: (1) half the slidespacing or (2) 10px.
      var slideTop = $(this).offset().top -
        (slidespacing ? (layout.windowHeight * slidespacing) / 2 : 10);

      if (scrollTop > slideTop) {
        currentSlide = this;
      }
      else {
        return false;
      }
    });

    $('.scrollshow-menu-wrapper .menu li a').removeClass('active');
    if (currentSlide) {
      $('.scrollshow-menu-wrapper .menu li a[data-slide-id="' + currentSlide.id + '"]').addClass('active');
    }
  }

  // See if we should show a modal dialog asking the user to rotate
  // their phone. For now, we're targetting iPhones.
  function checkOrientation(layout) {
    var askUserToRotate =
          Drupal.scrollshow.browser.iOS &&
          typeof window.orientation !== 'undefined' &&
          (window.orientation == 0 || window.orientation == 180) &&
          layout.windowWidth < 400 &&
          layout.windowHeight > 400,
        dialog = $('#scrollshow-orientation-dialog');

    if (askUserToRotate) {
      if (dialog.length == 0) {
        dialog = $('<div id="scrollshow-orientation-dialog"></div>')
          .appendTo('body')
          .dialog({
            title: Drupal.t('Please rotate your phone!'),
            dialogClass: 'scrollshow-orientation-dialog',
            autoOpen: false,
            draggable: false,
            resizable: false,
            closeOnEscape: false,
            modal: true
          })
          .text(Drupal.t('Please rotate your phone so that you\'re holding it horizontally.'));
      }

      dialog.dialog('open');
    }
    else if (dialog.length != 0) {
      dialog.dialog('close');
    }
  }

  // We implement our functionality as the 'default' effect.
  Drupal.scrollshow.effects['default'] = {
    setup: function (settings) {
      // for iPad/iPhone
      if (Drupal.scrollshow.browser.iOS) {
        setupTouchScrollShim();
      }

    },

    attach: function (context, settings) {
      // We're currently having problems with smooth scroll actually going to the right
      // place on the Kindle Fire -- disabling for now!
      if (!Drupal.scrollshow.browser.Silk) {
        setupSmoothScroll(context, settings);
      }
    },

    resize: function (layout, settings) {
      var windowHeight = layout.windowHeight;

      $('.scrollshow-slide').each(function () {
        var h = $(this).height(), padding, paddingTop, paddingBottom;
        if (h < windowHeight) {
          padding = Math.max(0, Math.round((windowHeight - h)));
          paddingTop = padding * (settings.slideposition / 100.0);
          paddingBottom = padding - paddingTop;
          $(this).css({
            'padding-top': paddingTop + 'px',
            'padding-bottom': paddingBottom + 'px',
          });

        }

        if (settings.slidespacing) {
          $(this).css('margin-bottom', windowHeight * settings.slidespacing);
        }
      });

      // Remove slide spacing from the last one
      $('.scrollshow-slide:last').css('margin-bottom', '');

      updateActiveMenu(layout);
      checkOrientation(layout);
    },

    scroll: function (layout, settings) {
      updateActiveMenu(layout);
    }
  };
})(jQuery);

