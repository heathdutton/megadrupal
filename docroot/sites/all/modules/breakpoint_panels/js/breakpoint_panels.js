(function ($) {
  Drupal.behaviors.breakpoint_panels = {

    attach: function (context) {
      /**
       * Initializes breakpoint panels listeners and handling.
       */

      // If no breakpoints found, then these are not the droids you're looking for, move along.
      if (
        Drupal.settings.breakpoint_panels_breakpoint['breakpoints'] == 'undefined'
        || Drupal.settings.breakpoint_panels_breakpoint['breakpoints']['hasEnquire'] == false
      ) {
        return;
      }
      var settings = Drupal.settings.breakpoint_panels_breakpoint;
      var breakpoints = settings['breakpoints'];

      var that = this;

      // Setup the toggle responsive handlers for use in enquire.js.
      // These are required or an unregister call will blow away handles that are still needed.
      for (var breakpoint in breakpoints) {
        var css = breakpoints[breakpoint]['css'];
        Drupal.settings.breakpoint_panels_breakpoint['breakpoints'][breakpoint]['toggle_handler'] = {
          match: function () {
            if($('.panels-ipe-editing').length < 1) {
              $('.hide-' + css).hide();
            }
          },
          unmatch: function () {
            if($('.panels-ipe-editing').length < 1) {
              $('.hide-' + css).show();
            }
          }
        };
      }

      // Check to see if an admin is using the IPE.
      $('#panels-ipe-customize-page').click(function (context) {
        that.checkForEditing();
      });

      // Update the window dimensions on each resize.
      $(window).resize(function () {
        that.onResize();
      });

      // Do a first manual update to catch the current window dimensions.
      this.onResize();

      // For each AJAX pane check if it should be loaded and register enquire match.
      $('.bp-ajax-pane').each(function () {
        var element = $(this);

        // Kick the hide-* styles up the the .panel-pane to make sure any styles applied to the pane
        // do not still show even if the contents are just a placeholder.
        var parent_classes = element.parent().attr('class').split(/\s+/);
        var pane_ancestor = element.closest('.panel-pane');
        var ipe_ancestor = element.closest('.panels-ipe-portlet-wrapper');
        if (parent_classes.length) {
          for (var style in parent_classes) {
            element.removeClass(parent_classes[style]);
            pane_ancestor.addClass(parent_classes[style]);
            ipe_ancestor.addClass('ipe-' + parent_classes[style]);
          }
        }

        // Setup the enquire.js AJAX loading based on breakpoints.
        var url = element.attr('data-src');
        that.checkForLoad(url, element);
      });

    },

    onResize: function () {
      /**
       * Updates the objects height/width and checks if reloading of the page is required.
       */

      if (this.width && this.height) {
        this.checkForReload();
      }
      var $window = $(window);
      this.width = $window.width();
      this.height = $window.height()

    },

    checkForReload: function () {
      /**
       * If auto loading is enabled in the Breakpoint Panels configuration, then this
       * method will check if the page needs to be reloaded on a resize.
       * This is generally for development purposes.
       */

      var settings = Drupal.settings.breakpoint_panels_breakpoint;
      var breakpoints = settings['breakpoints'];

      if (!(settings['autoload'])) {
        return;
      }

      var $window = $(window);
      for (var breakpoint in breakpoints) {
        for (var key in breakpoints[breakpoint]) {
          // Skip any non-dimensional properties.
          if (key == 'bp' || key == 'css' || key == 'toggle_handler') {
            continue;
          }

          var value = breakpoints[breakpoint][key];

          // If the result changes, the condition has changed, so we need
          // to reload.
          var now = this.checkCondition(key, value, $window.width(), $window.height());
          var before = this.checkCondition(key, value, this.width, this.height);

          if (now !== before) {
            window.location.reload(true);

            // FF prevents reload in onRsize event, so we need to do it
            // in a timeout. See issue #1859058
            if ('mozilla' in $.browser) {
              setTimeout(function () {
                window.location.reload(true);
              }, 10);
            }
            return;
          }
        }
      }

    },

    checkCondition: function (condition, value, width, height) {
      /**
       * Used to check if a media query condition is met.
       */

      var flag = null;

      switch (condition) {
        case 'width':
          flag = width === value;
          break;

        case 'min-width':
          flag = width >= value;
          break;

        case 'max-width':
          flag = width <= value;
          break;

        case 'height':
          flag = height === value;
          break;

        case 'min-height':
          flag = height >= value;
          break;

        case 'max-height':
          flag = height <= value;
          break;

        case 'aspect-ratio':
          flag = width / height === value;
          break;

        case 'min-aspect-ratio':
          flag = width / height >= value;
          break;

        case 'max-aspect-ratio':
          flag = width / height <= value;
          break;

        default:
          break;
      }

      return flag;

    },

    checkForLoad: function (url, element) {
      /**
       * Checks if a pane should be loaded given the current screen size.
       */

      var settings = Drupal.settings.breakpoint_panels_breakpoint;
      var breakpoints = settings['breakpoints'];

      var parent_el = element.parent();
      // var this_shown = false;
      for (var breakpoint in breakpoints) {
       // var cur_bp = settings['breakpoints'][key];
        if (
          !parent_el.hasClass('hide-' + breakpoints[breakpoint]['css'])
          || settings['loadhidden']
          || (settings['adminload'] && settings['isloggedin'])
        ) {
          if (settings['hasEnquire']) {
            var that = this;
            // If at any point the media query is met, make sure the pane contents are loaded via AJAX.
            enquire.register(breakpoints[breakpoint]['bp'], {
              match: function () {
                that.fetch_pane(url, element);
              }
            });
          }
          else {
            // Fallback pseudo-gracefully if enquire.js was not found.
            this.fetch_pane(url, element);
          }
        }
      }

    },

    checkForEditing: function (x) {
      /**
       * Set up the breakpoint panels editing within IPE.
       */

      // Check if the IPE save button is there.
      x = (x) ? x : 0;
      var that = this;
      if ($('#panels-ipe-save').length < 1) {
        // Nope, wait more and try a few more times for good measure.
        x++;
        if (x < 10) {
          setTimeout(function () {
            that.checkForEditing(x);
          }, 500);
        }
        return;
      }

      var settings = Drupal.settings.breakpoint_panels_breakpoint;
      var breakpoints = settings['breakpoints'];

      // Setup the toggle responsive button.
      if ($('.toggleResponsive').length < 1) {
        $('#panels-ipe-edit-control-form div').prepend("<div class='toggleResponsive icon-large icon-eye-open'>Toggle Responsive</div>");
        $('.toggleResponsive').click(function () {
          if (!$(this).hasClass('active')) {

            for (var breakpoint_r in breakpoints) {
              if (settings['hasEnquire'] == true) {
                enquire.register(breakpoints[breakpoint_r]['bp'], breakpoints[breakpoint_r]['toggle_handler']);
              }
            }

            $(this).addClass('active icon-eye-close');
            $(this).removeClass('icon-eye-open');
            $('.panels-ipe-editing').addClass('hide-responsive');
          }
          else {
            for (var breakpoint_u in breakpoints) {
              // $('.hide-' + breakpoints[breakpoint_u]['css']).show();
              if (settings['hasEnquire'] == true) {
                enquire.unregister(breakpoints[breakpoint_u]['bp'], breakpoints[breakpoint_u]['toggle_handler']);
              }
            }

            $(this).removeClass('active icon-eye-close');
            $(this).addClass('icon-eye-open');
            $('.panels-ipe-editing').removeClass('hide-responsive');
          }
        });
      }

    },

    fetch_pane: function (url, element) {
      /**
       * Does an AJAX request for the pane contents if it has not yet been loaded.
       */
      if (!element.hasClass('processed')) {
        var element_settings = {};
        element_settings.progress = {};
        element_settings.url = url + '/' + element.attr('id');
        element_settings.event = 'click';
        var base = element.attr('id');
        var ajax = new Drupal.ajax(base, element, element_settings);
        ajax.eventResponse(element, 'click');
        element.addClass('processed');
      }

    }

  };

})(jQuery);
