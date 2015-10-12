/**
 * @file
 * Viewport dimensions measurement script.
 * Depends on verge.js library by Ryan Van Etten (http://verge.airve.com/).
 *
 * Author: Benjamin Heerschlag
 */

(function($)  {
  function verge_lib_measure_cookie(context, settings)  {
    /**
     * Prepare variables:
     * - resizeTimeout and timeoutInt are used to delay resize().
     * - Without this some browser call resize() twice. 
     */
    var resizeTimeout, verge_height, verge_width;
    // delay in milliseconds.
    var timeoutInt = 10;

    /**
     * Basic stuff, called only once:
     * - extend jquery
     * - measure viewport dimensions
     */
    $(page).once(function() {
      jQuery.extend(verge);
      refreshViewportDimensions();
    });

    /**
     * Custom resize() function; delayed.
     */
    jQuery(window).resize(function() {
      clearTimeout(resizeTimeout);
      resizeTimeout = setTimeout(refreshViewportDimensions, timeoutInt);
    });
  }

  /**
   * Custom function refreshViewportDimensions():
   * - Get viewport measurement results with verge.
   * - Save results in cookie, so other scripts can make use of it.
   */
  function refreshViewportDimensions() {
    var verge_height = $.viewportH();
    var verge_width = $.viewportW();
    $.cookie("verge_height", verge_height);
    $.cookie("verge_width", verge_width);
  }

  Drupal.behaviors.verge_lib_measure_cookie = {
    attach: function(context, settings)  {
      verge_lib_measure_cookie(context, settings);
    }
  }
})(jQuery);
