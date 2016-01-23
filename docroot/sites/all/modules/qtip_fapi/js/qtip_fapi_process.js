/*jshint strict:true, browser:true, curly:true, eqeqeq:true, expr:true, forin:true, latedef:true, newcap:true, noarg:true, trailing: true, undef:true, unused:true */
/*global Drupal: true, jQuery: true, console :true*/
/**
 * File:        qtip_fapi_process.js
 * Version:     7.x-1.x
 * Description: Process qtip_fapi and add to form
 * Author:      westwesterson
 * Language:    Javascript
 * Project:     UI
 * @module qtip_fapi
 */

(function ($) {
  /**
   * Drupal.behaviors.qtipFAPI.
   *
   * Attach
   * @extends Drupal.behaviors
   */
  "use strict";
  Drupal.behaviors.qtipFAPI = {
    attach: function (context, settings) {
      var form_id = {},
        selector = '',
        overrides = {};

      // Iterate over each settings element by form_id.
      for (form_id in settings.qtipFAPI) {
        if (settings.qtipFAPI.hasOwnProperty(form_id)) {
          // Set default selector to be the form #id
          selector = '#' + form_id;
          //set the initial qTip overrides
          overrides = settings.qtipFAPI[form_id];

          // Allow the selector to be overridden.
          if (overrides.selector) {
            selector = overrides.selector;
            delete overrides.selector;
          }

          // Create the tooltip.
          $(selector, context).qtip(overrides);
        }
      }
    }
  };
})(jQuery);