/**
 * @file
 * Ajax Command Invoke Chain js.
 *
 * Processing code for the new command, used like 'invoke' but with the chance
 * to chain calls to several methods.
 */

(function ($) {
  'use strict';

  /**
   * Provides processing for the new ajax command.
   *
   * @param {Object} ajax - Drupal Ajax object.
   * @param {Object} response - Response from server with data retrieved.
   * @param {string} status - Value returned by response processed.
   */
  Drupal.ajax.prototype.commands.invoke_chain = function (ajax, response, status) {
    // Get selector information from the response. If it's not there, default to presets.
    var wrapper = response.selector ? $(response.selector) : $(ajax.wrapper);

    // Get the element.
    var $element = $(wrapper);

    // Loop through all chain to work with data.
    for (var i = 0; i < response.chain.length; i++) {
      // Get the method and arguments.
      var method = response.chain[i].shift();

      // Call the function, and store the obtained object to perform the
      // chainning.
      $element = $element[method].apply($element, response.chain[i]);
    }
  };

})(jQuery);
