/**
 * @file
 * Breakpoints JavaScript API.
 */

;(function (window, document, $, enquire, settings, undefined) {

  /**
   * Helper function to get all defined breakpointmachine names.
   */
  function getBreakpointMachineNames() {
    var breakpoints = [];

    for (var s in settings.breakpoints) {
      breakpoints.push(s);
    }

    return breakpoints;
  }

  /**
   * Helper function to retrive retrive a breakpoint by name or machine name both.
   */
  function getBreakpointByName(name, machineOnly) {

    if (settings.breakpoints[name]) {
      return settings.breakpoints[name];
    }

    if (machineOnly === false) {
      for (var s in settings.breakpoints) {
        if (settings.breakpoints[s].name.toLowerCase() == name.toLowerCase()) {
          return settings.breakpoints[s];
        }
      }
    }

    return null;
  }

  /**
   * Helper function to parse breakpoint argument into breakpoint objects.
   */
  function getBreakpoints(breakpoint, machineOnly) {

    // Handle all breakpoints option.
    if (breakpoint == 'all') {
      var breakpoints = getBreakpointMachineNames();
    }
    // Handle single breakpoint name.
    else if (typeof breakpoint == 'string') {
      var breakpoints = [breakpoint];
    }
    // Handle array of breakpoint names.
    else {
      var breakpoints = breakpoint;
    }

    if (!$.isArray(breakpoints)) {

      // Handle breakpoint object as argument.
      if (breakpoints && breakpoints.breakpoint) {
        return [breakpoints];
      }

      // Handler errors elegantly.
      throw new Error('Expecting array of breakpoint names, received a variable of type "' + typeof breakpoints + '"');
    }

    return breakpoints.map(function (breakpoint) {
      return typeof breakpoint != 'string' ? breakpoint : getBreakpointByName(breakpoint, machineOnly || false);
    }).filter(function (breakpoint) {
      return breakpoint ? true : false;
    });
  }

  /**
   * Helper function to merge breakpoint media querys.
   */
  function getMediaQuery(breakpoints) {
    return getBreakpoints(breakpoints).map(function (breakpoint) {
      return breakpoint.breakpoint;
    }).join(', ') || (typeof breakpoints == 'string' ? breakpoints : '');
  }

  var Breakpoints = {

    /**
     * Enquire register wrapper.
     * 
     * Use the same arguments provided the enquire.register method, but replace
     * the mediaQuery string by the breakpoint name/machine-name or a array of
     * media query. To register for all breakpoints at once, you can use
     * the tring "all" as the breakpoint value.
     *
     * Please, refer to http://wicky.nillia.ms/enquire.js/#api.
     */
    register: function (breakpoint, handler, shouldDegrade) {
      enquire.register(getMediaQuery(breakpoint), handler, shouldDegrade);
      return Breakpoints;
    },

    /**
     * Enquire unregister wrapper.
     * 
     * Use the same arguments provided the enquire.unregister method, but replace
     * the mediaQuery string by the breakpoint name/machine-name or a array of
     * media query. To unregister for all breakpoints at once, you can use
     * the tring "all" as the breakpoint value.
     *
     * Please, refer to http://wicky.nillia.ms/enquire.js/#api.
     */
    unregister: function (breakpoint, handler) {
      enquire.unregister(getMediaQuery(breakpoint), handler);
      return Breakpoints;
    },

    /**
     * Get the current active breakpoints.
     */
    getCurrent: function () {
      return getBreakpoints('all').filter(function (breakpoint) {
        return window.matchMedia(breakpoint.breakpoint).matches;
      });
    },

    /**
     * Checks if the given breakpoint is current.
     */
    isCurrent: function (breakpoint) {
      var current = this.getCurrent();
      return getBreakpoints(breakpoint).filter(function (breakpoint) {
        return current.indexOf(breakpoint) > -1;
      }).length > 0;
    }
  };

  // Consider any previous existence of Breakpoints object.
  Drupal.Breakpoints = $.extend(true, Breakpoints, Drupal.Breakpoints);

})(this, this.document, jQuery, enquire, Drupal.settings);
