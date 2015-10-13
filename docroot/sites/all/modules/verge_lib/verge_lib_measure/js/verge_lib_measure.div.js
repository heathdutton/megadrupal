/**
 * @file
 * Custom script: Display viewport dimensions in a div.
 *
 * Author: Benjamin Heerschlag
 */

(function($)  {
  function verge_lib_measure_div(context, settings)  {
    /**
     * Prepare variables:
     * - resizeTimeout and timeoutInt are used to delay resize().
     * - Without this some browser call resize() twice. 
     */
    var resizeTimeout;
    // delay in milliseconds.
    var timeoutInt = 10;

    /**
     * Basic stuff, called only once:
     * - append div to body
     * - update div
     */
    $(page).once(function() {
      jQuery("#" + Drupal.settings.verge_lib_measure.append_id).append('<div id="verge-lib-measure"><div id="verge-lib-measure-inner"></div></div>');
      updateViewportDiv();
    });

    /**
     * Custom resize() function; delayed.
     */
    jQuery(window).resize(function() {
      clearTimeout(resizeTimeout);
      resizeTimeout = setTimeout(updateViewportDiv, timeoutInt);
    });
  }

  /**
   * Custom function updateViewportDiv():
   * - Fill div with measurement results as html content.
   */
  function updateViewportDiv() {
    jQuery('#verge-lib-measure-inner').html($.cookie("verge_width") + ' x ' + $.cookie("verge_height"));
  }

  Drupal.behaviors.verge_lib_measure_div = {
    attach: function(context, settings)  {
      verge_lib_measure_div(context, settings);
    }
  }
})(jQuery);
