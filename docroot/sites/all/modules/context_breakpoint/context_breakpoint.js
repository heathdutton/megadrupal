(function ($) {
  Drupal.behaviors.contextBreakpoint = {
    width: null,
    height: null,

    breakpointsCookie: 'context_breakpoints',
    activeBreakpoints: null,

    saveResolutionCookie: false,
    resolutionCookie: 'context_breakpoint_resolution',

    reloadActive: false,

    // settings
    settings: null,
    contexts: null,

    // Whether user is on an admin page.
    isAdminPage: null,

    arrayDiff: function(a1, a2) {
      return $(a1).not(a2).get().concat($(a2).not(a1).get());
    },

    preInit: function() {
      if (!('context_breakpoint' in Drupal.settings)) {
        return;
      }

      var config = Drupal.settings.context_breakpoint;

      // Set settings that we need later on.
      this.settings = config.settings;
      this.contexts = config.contexts;
      this.isAdminPage = config.is_admin;

      this.breakpointsInUrl = this.settings.breakpoints_in_url;
      this.autoReload = this.isReloadEnabled();

      var cookieFlag = this.checkForCookie();
      var flagUrl  = this.checkForUrlDiscrepancy();

      if (flagUrl) {
        window.location.href = flagUrl;
      }
      else if (cookieFlag) {
        this.doReload();
      }
    },

    checkForCookie: function() {
      if ($.cookie(this.breakpointsCookie) === null) {
        this.saveCookie(this.checkBreakpoints());
        return true;
      }

      return false;
    },

    checkForUrlDiscrepancy: function() {
      if (this.breakpointsInUrl && this.autoReload) {
        var href = window.location.href;

        var currentCookie = $.cookie(this.breakpointsCookie);

        var pattern = new RegExp('context\-breakpoints\=([^\&\?]+)');
        var result = pattern.exec(href);

        var newUrl = null;

        if (result === null) {
          if (currentCookie === '') {
            // do nothing, since no context is set anyway
          }
          else {
            var newUrl = href + (href.search(/\?/) === -1 ? '?' : '&') + 'context-breakpoints=' + currentCookie;
          }
        }
        else {
          if (result[1] !== currentCookie) {
            if (currentCookie === '') {
              // Strip out the whole parameter, since we do not need it
              newUrl = href.replace('&context-breakpoints=' + result[1], '');
              newUrl = newUrl.replace('?context-breakpoints=' + result[1], '');
            }
            else {
              newUrl = href.replace(result[1], currentCookie);
            }
          }
        }

        return newUrl;
      }

      return false;
    },

    attach: function (context, settings) {
      if (!this.contexts) {
        // Nothing to do if no contexts available.
        return;
      }

      // Do not do anything if reload is already triggered
      // to prevent messing with the cookie again.
      if (this.checkForCookieReload) {
        return;
      }

      var that = this;

      // Update cookies on each resize.
      $(window).resize(function() {
        that.onResize();
      });

      // Check if cookie with resolution should also be saved.
      this.saveResolutionCookie = this.settings.save_resolution;
      if (!this.saveResolutionCookie) {
        // Otherwise, delete if it exists.
        $.cookie(this.resolutionCookie, '', {'expires': new Date(0)});
      }

      // Retrieve active breakpoints from cookie.
      this.activeBreakpoints = this.getCookieBreakpoints();

      // Do a first manual cookie update to catch the current state.
      this.onResize();
    },

    // Set the cookie with screen and browser width + height.
    // Then check if we need to reload.
    onResize: function() {
      // Compute currently active breakpoints.
      var newActiveBreakpoints = this.checkBreakpoints();

      // If required, update resolution cookie.
      if (this.saveResolutionCookie) {
        $window = $(window);
        var value = $window.width() + 'x' + $window.height()
           + '|' + screen.width + 'x' + screen.height;

        $.cookie(this.resolutionCookie, value);
      }

      // Check if any breakpoint has become active or inactive
      var diff = this.arrayDiff(this.activeBreakpoints, newActiveBreakpoints);

      if (diff.length) {
        // Update the cookie.
        this.saveCookie(newActiveBreakpoints);
        this.activeBreakpoints = newActiveBreakpoints;

        // If url auto-change is enabled, we have to do it now.
        var newUrl = this.checkForUrlDiscrepancy();
        if (newUrl) {
          window.location.href = newUrl;
        }
        else {
          // Check if we have to reload.
          for (var key in diff) {
            if (this.isReloadEnabled(diff[key])) {
              this.doReload();
              break;
            }
          }
        }
      }
    },

    isReloadEnabled: function(context) {
      // On admin pages, do not reload.
      if (this.isAdminPage && this.settings.admin_disable_reload) {
        return false;
      }
      // Check for specific context.
      else if (context) {
        return this.contexts[context].autoreload;
      }
      // Check if any context has reload enabled.
      else {
        var contexts = this.contexts;

        for (var key in contexts) {
          if (contexts[key].autoreload) {
            return true;
          }
        }

        return false;
      }
    },

    saveCookie: function(activeBreakpoints) {
      var points = activeBreakpoints.length ? activeBreakpoints.join(',') : 'none';
      $.cookie(this.breakpointsCookie, points);
    },

    getCookieBreakpoints: function() {
      var value = $.cookie(this.breakpointsCookie);
      if (value === 'none') {
        value = null;
      }

      var breakpoints = value ? value.split(',') : [];

      return breakpoints;
    },

    checkBreakpoints: function(curWidth, curHeight) {
      var contexts = this.contexts;
      var $window = $(window);

      var activeBreakpoints = [];

      for (var contextName in contexts) {
        var context = contexts[contextName];

        isActive = true;

        for (var key in context.breakpoints) {

          for (var cmd in context.breakpoints[key]) {
            var value = context.breakpoints[key][cmd];

            // If the result changes, the condition has changed, so we need
            // to reload.
            var deviceCheck = cmd.search('device') !== -1;

            var width = height = null;
            if (deviceCheck) {
              width = screen.width;
              height = screen.height;
            }
            else {
              width = $window.width();
              height = $window.height();
            }

            var flag = this.checkCondition(cmd, width, height, value);

            if (!flag) {
              isActive = false;
              break;
            }
          }

        }

        if (isActive) {
          activeBreakpoints.push(contextName);
        }
      }

      return activeBreakpoints;
    },

    doReload: function() {
      window.location.reload(true);

      // FF prevents reload in onRsize event, so we need to do it
      // in a timeout. See issue #1859058
      if ('mozilla' in $.browser)  {
        setTimeout(function() {
          window.location.reload(true);
        }, 10);
      }
      return;
    },

    checkCondition: function(condition, width, height, value) {
      var flag = null;

      switch (condition) {
        case 'width':
        case 'device-width':
          flag = width === value;
          break;

        case 'min-width':
        case 'min-device-width':
          flag = width >= value;
          break;

        case 'max-width':
        case 'max-device-width':
          flag = width <= value;
          break;

        case 'height':
        case 'device-height':
          flag = height === value;
          break;

        case 'min-height':
        case 'min-device-height':
          flag = height >= value;
          break;

        case 'max-height':
        case 'max-device-height':
          flag = height <= value;
          break;

        case 'aspect-ratio':
        case 'device-aspect-ratio':
          flag = width / height === value;
          break;

        case 'min-aspect-ratio':
        case 'min-device-aspect-ratio':
          flag = width / height >= value;
          break;

        case 'max-aspect-ratio':
        case 'max-device-aspect-ratio':
          flag = width / height <= value;
          break;

        default:
          break;
      }

      return flag;
    }
  };
})(jQuery);
