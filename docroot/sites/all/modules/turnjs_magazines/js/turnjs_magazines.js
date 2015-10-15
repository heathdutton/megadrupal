// ##################### HELPER FUNCTIONS #####################################

/**
 * Responsive zoom helper function. Zoomes to the clicked region.
 *
 * @param {type} event
 * @returns {undefined}
 */
function turnjs_magazines_zoomTo(event) {
  setTimeout(function() {
    if (jQuery('.magazine-viewport').data().turnjs_magazines_regionClicked) {
      jQuery('.magazine-viewport').data().turnjs_magazines_regionClicked = false;
    } else {
      if (jQuery('.magazine-viewport').zoom('value') == 1) {
        jQuery('.magazine-viewport').zoom('zoomIn', event);
      } else {
        jQuery('.magazine-viewport').zoom('zoomOut');
      }
    }
  }, 1);
}

/**
 * http://code.google.com/p/chromium/issues/detail?id=128488
 * @returns {Boolean}
 */
function turnjs_magazines_isChrome() {
  return navigator.userAgent.indexOf('Chrome') != -1;
}

/**
 * Process click on a region.
 *
 * @param {type} event
 * @returns {undefined}
 */
function turnjs_magazines_regionClick(event) {
  var region = jQuery(event.target);
  if (region.hasClass('region')) {
    jQuery('.magazine-viewport').data().turnjs_magazines_regionClicked = true;
    setTimeout(function() {
      jQuery('.magazine-viewport').data().turnjs_magazines_regionClicked = false;
    }, 100);
    var regionType = jQuery.trim(region.attr('class').replace('region', ''));
    return turnjs_magazines_processRegion(region, regionType);
  }
}

/**
 * Process the data of every region.
 *
 * @param {type} region
 * @param {type} regionType
 * @returns {undefined}
 */
function turnjs_magazines_processRegion(region, regionType) {
  data = turnjs_magazines_decodeParams(region.attr('region-data'));
  switch (regionType) {
    case 'link' :
      window.open(data.url);
      break;
    case 'zoom' :
      var regionOffset = region.offset(),
              viewportOffset = jQuery('.magazine-viewport').offset(),
              pos = {
        x: regionOffset.left - viewportOffset.left,
        y: regionOffset.top - viewportOffset.top
      };
      jQuery('.magazine-viewport').zoom('zoomIn', pos);
      break;
    case 'to-page' :
      jQuery('.magazine').turn('page', data.page);
      break;
  }
}

/**
 * Helper to disable the controls (f.e. while an event takes place)
 *
 * @param {type} page
 * @returns {undefined}
 */
function turnjs_magazines_disableControls(page) {
  if (page == 1)
    jQuery('.previous-button').hide();
  else
    jQuery('.previous-button').show();

  if (page == jQuery('.magazine').turn('pages'))
    jQuery('.next-button').hide();
  else
    jQuery('.next-button').show();
}

function turnjs_magazines_get_imageratio($image) {
  return $image.width() / $image.height();
}

/**
 * Resize the viewport on screen size / orientation change.
 * @returns {undefined}
 */
function turnjs_magazines_resizeViewport() {
  var viewport_size = turnjs_magazines_get_viewport_size();
  var width = viewport_size.width - 44,
          height = viewport_size.height,
          options = jQuery('.magazine').turn('options');

  jQuery('.magazine').removeClass('animated');
  jQuery('.magazine-viewport').css({
    width: width + 44,
    height: height
  }).zoom('resize');
  if (jQuery('.magazine').turn('zoom') == 1) {
    var bound = turnjs_magazines_calculateBound({
      width: options.width,
      height: options.height,
      boundWidth: Math.min(options.width, width),
      boundHeight: Math.min(options.height, height)
    });
    if (bound.width % 2 !== 0) {
      bound.width -= 1;
    }
    if (bound.width != jQuery('.magazine').width() || bound.height != jQuery('.magazine').height()) {
      jQuery('.magazine').turn('size', bound.width, bound.height);
    }
    jQuery('.magazine').css({top: -bound.height / 2, left: -bound.width / 2});
  }
  jQuery('.magazine').addClass('animated');
}

/**
 * Width of the flipbook when zoomed in.
 *
 * @returns {@exp;@call;jQuery@call;width}
 */
function turnjs_magazines_largeMagazineWidth() {
  return jQuery(window).width();
}

/**
 * Decode URL Parameters.
 *
 * @param {type} data
 * @returns {unresolved}
 */
function turnjs_magazines_decodeParams(data) {
  var parts = data.split('&'), d, obj = {};
  for (var i = 0; i < parts.length; i++) {
    d = parts[i].split('=');
    obj[decodeURIComponent(d[0])] = decodeURIComponent(d[1]);
  }
  return obj;
}

/**
 * Calculate the width and height of a square within another square.
 *
 * @param {type} d
 * @returns {turnjs_magazines_calculateBound.bound}
 */
function turnjs_magazines_calculateBound(d) {
  var bound = {width: d.width, height: d.height};
  if (bound.width > d.boundWidth || bound.height > d.boundHeight) {
    var rel = bound.width / bound.height;
    if (d.boundWidth / rel > d.boundHeight && d.boundHeight * rel <= d.boundWidth) {
      bound.width = Math.round(d.boundHeight * rel);
      bound.height = d.boundHeight;
    } else {
      bound.width = d.boundWidth;
      bound.height = Math.round(d.boundWidth / rel);
    }
  }
  return bound;
}

/**
 * Returns the viewport size.
 * 
 * @returns {turnjs_magazines_get_viewport_size.Anonym$3}
 */
function turnjs_magazines_get_viewport_size() {
  var viewportWidth = jQuery('#turnjs-magazine-container').width();
  var $firstImage = jQuery('.magazine-viewport div.p1 img');
  var firstImageRatio = turnjs_magazines_get_imageratio($firstImage);
  var viewportHeight = ((viewportWidth / 2) / firstImageRatio);

  return {
    width: viewportWidth,
    height: viewportHeight
  };
}

// ##################### INITIALIZATION #####################################

var turnjs_magazines = turnjs_magazines || {};
turnjs_magazines.loadApp = function() {
  var $firstImage = jQuery('.magazine-viewport div.p1 img');
  // Safely preload the image.
  jQuery("<img />").attr("src", $firstImage.attr('src')).attr('alt', Drupal.t('Loading ...')).prependTo('#turnjs-magazine-loading-firstpage');
  // Animate the loading animation.
  setInterval(function() {
    jQuery('#turnjs-magazine-loading-firstpage').fadeTo(1000, 0.1, function() {
      jQuery(this).fadeTo(1000, 0.4);
    });
  }, 2000);
  $firstImage.one("load", function() {
    setTimeout(turnjs_magazines._loadApp, 10);
  }).each(function() {
    if (this.complete)
      jQuery(this).load();
  });
}

turnjs_magazines._loadApp = function() {
  var flipbook = jQuery('.magazine');
  // Only load if the flipbook exists on the page.
  if (flipbook.length > 0) {
    // The height of the viewport has to be calculated!
    // It's the width of the turnjs-magazine-container / 2 (because 2-page layout)
    // divided by the image ratio of the first image. (width / height).
    // The result is the true height of the images after resizing to
    // turnjs-magazine-container.

    var viewport_size = turnjs_magazines_get_viewport_size();
    // Create the flipbook
    flipbook.turn({
      // Display style
      display: 'double',
      // Magazine width
      width: viewport_size.width,
      // Magazine height
      height: viewport_size.height,
      // Duration in millisecond
      duration: 1000,
      // Hardware acceleration
      acceleration: !turnjs_magazines_isChrome(),
      // Enables gradients
      gradients: true,
      // Auto center this flipbook
      autoCenter: true,
      // Elevation from the edge of the flipbook when turning a page
      elevation: 50,
      // The inclination of the page during the transition.
      inclination: 50,
      // Events
      when: {
        turning: function(event, page, view) {
          var book = jQuery(this),
                  currentPage = book.turn('page'),
                  pages = book.turn('pages');

          // Show and hide navigation buttons
          turnjs_magazines_disableControls(page);
          jQuery('.magazine-thumbnails .page-' + currentPage).
                  parent().
                  removeClass('current');
          jQuery('.magazine-thumbnails .page-' + page).
                  parent().
                  addClass('current');
        },
        turned: function(event, page, view) {
          turnjs_magazines_disableControls(page);
          jQuery(this).turn('center');
          // Peel on load.
          jQuery(this).turn('peel', 'br');
        }
      }
    }).turn('peel', 'br');

    // Zoom.js
    jQuery('.magazine-viewport').zoom({
      flipbook: jQuery('.magazine'),
      max: function() {
        return turnjs_magazines_largeMagazineWidth() / jQuery('.magazine').width();
      },
      when: {
        swipeLeft: function() {
          jQuery(this).zoom('flipbook').turn('next');
        },
        swipeRight: function() {
          jQuery(this).zoom('flipbook').turn('previous');
        },
        zoomIn: function() {
          jQuery('.magazine').removeClass('animated').addClass('zoom-in');
          jQuery('.zoom-icon').removeClass('zoom-icon-in').addClass('zoom-icon-out');
          if (!jQuery.isTouch) {
            jQuery('<div />', {'class': 'exit-message'}).
                    html('<div>' + Drupal.t('Press ESC to exit') + '</div>').
                    appendTo(jQuery('body')).
                    delay(2000).
                    animate({opacity: 0}, 2000, function() {
              jQuery(this).remove();
            });
          }
        },
        zoomOut: function() {
          jQuery('.exit-message').hide();
          jQuery('.zoom-icon').removeClass('zoom-icon-out').addClass('zoom-icon-in');
          setTimeout(function() {
            jQuery('.magazine').addClass('animated').removeClass('zoom-in');
            turnjs_magazines_resizeViewport();
          }, 0);
        }
      }
    });
    // Zoom event
    if (jQuery.isTouch)
      jQuery('.magazine-viewport').bind('zoom.doubleTap', turnjs_magazines_zoomTo);
    else
      jQuery('.magazine-viewport').bind('zoom.tap', turnjs_magazines_zoomTo);

    // Using arrow keys to turn the page
    jQuery(document).keydown(function(e) {
      var previous = 37, next = 39, esc = 27;
      switch (e.keyCode) {
        case previous:
          // left arrow
          jQuery('.magazine').turn('previous');
          e.preventDefault();
          break;
        case next:
          //right arrow
          jQuery('.magazine').turn('next');
          e.preventDefault();
          break;
        case esc:
          jQuery('.magazine-viewport').zoom('zoomOut');
          e.preventDefault();
          break;
      }
    });
    // URIs - Format #/page/1
    Hash.on('^page\/([0-9]*)jQuery', {
      yep: function(path, parts) {
        var page = parts[1];
        if (page !== undefined) {
          if (jQuery('.magazine').turn('is'))
            jQuery('.magazine').turn('page', page);
        }
      },
      nop: function(path) {
        if (jQuery('.magazine').turn('is'))
          jQuery('.magazine').turn('page', 1);
      }
    });
    jQuery(window).resize(function() {
      turnjs_magazines_resizeViewport();
    }).bind('orientationchange', function() {
      turnjs_magazines_resizeViewport();
    });
    // Events for thumbnails
    jQuery('.magazine-thumbnails').click(function(event) {
      var page;
      if (event.target && (page = /page-([0-9]+)/.exec(jQuery(event.target).attr('class')))) {
        jQuery('.magazine').turn('page', page[1]);
      }
    });
    jQuery('.magazine-thumbnails li').
            bind(jQuery.mouseEvents.over, function() {
      jQuery(this).addClass('thumb-hover');
    }).bind(jQuery.mouseEvents.out, function() {
      jQuery(this).removeClass('thumb-hover');
    });
    if (jQuery.isTouch) {
      jQuery('.magazine-thumbnails').
              addClass('thumbanils-touch').
              bind(jQuery.mouseEvents.move, function(event) {
        event.preventDefault();
      });
    } else {
      jQuery('.magazine-thumbnails ul').mouseover(function() {
        jQuery('.magazine-thumbnails').addClass('magazine-thumbnails-hover');
      }).mousedown(function() {
        return false;
      }).mouseout(function() {
        jQuery('.magazine-thumbnails').removeClass('magazine-thumbnails-hover');
      });
    }

    // Regions
    if (jQuery.isTouch) {
      jQuery('.magazine').bind('touchstart', turnjs_magazines_regionClick);
    } else {
      jQuery('.magazine').click(turnjs_magazines_regionClick);
    }

    // Events for the next button
    jQuery('.next-button').bind(jQuery.mouseEvents.over, function() {
      jQuery(this).addClass('next-button-hover');
    }).bind(jQuery.mouseEvents.out, function() {
      jQuery(this).removeClass('next-button-hover');
    }).bind(jQuery.mouseEvents.down, function() {
      jQuery(this).addClass('next-button-down');
    }).bind(jQuery.mouseEvents.up, function() {
      jQuery(this).removeClass('next-button-down');
    }).click(function() {
      jQuery('.magazine').turn('next');
    });

    // Events for the previous button
    jQuery('.previous-button').bind(jQuery.mouseEvents.over, function() {
      jQuery(this).addClass('previous-button-hover');
    }).bind(jQuery.mouseEvents.out, function() {
      jQuery(this).removeClass('previous-button-hover');
    }).bind(jQuery.mouseEvents.down, function() {
      jQuery(this).addClass('previous-button-down');
    }).bind(jQuery.mouseEvents.up, function() {
      jQuery(this).removeClass('previous-button-down');
    }).click(function() {
      jQuery('.magazine').turn('previous');
    });


    // Zoom icon
    jQuery('.zoom-icon').bind('mouseover', function() {
      if (jQuery(this).hasClass('zoom-icon-in'))
        jQuery(this).addClass('zoom-icon-in-hover');
      if (jQuery(this).hasClass('zoom-icon-out'))
        jQuery(this).addClass('zoom-icon-out-hover');
    }).bind('mouseout', function() {
      if (jQuery(this).hasClass('zoom-icon-in'))
        jQuery(this).removeClass('zoom-icon-in-hover');
      if (jQuery(this).hasClass('zoom-icon-out'))
        jQuery(this).removeClass('zoom-icon-out-hover');
    }).bind('click', function() {
      if (jQuery(this).hasClass('zoom-icon-in'))
        jQuery('.magazine-viewport').zoom('zoomIn');
      else if (jQuery(this).hasClass('zoom-icon-out'))
        jQuery('.magazine-viewport').zoom('zoomOut');
    });

    turnjs_magazines_resizeViewport();
    jQuery('.magazine').addClass('animated');
    // Show!
    jQuery('#turnjs-magazine-container').css('opacity', 0);
    jQuery('#turnjs-magazine-container').fadeTo(2000, 1);
    jQuery('#turnjs-magazine-loading-firstpage').remove();
  }
};
