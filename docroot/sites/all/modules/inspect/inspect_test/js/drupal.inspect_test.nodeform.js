/**
 * @file
 * Drupal Inspect test Javascript behaviors.
 */

'use strict';

/*jslint browser: true, continue: true, indent: 2, newcap: true, nomen: true, plusplus: true, regexp: true, white: true, ass: true*/
/*global alert: false, confirm: false, console: false*/
/*global jQuery: false, Drupal: false*/

(function ($) {
  Drupal.behaviors.inspectTest = {
    attach: function (context) {
      var $elm;
      if (($elm = $('form.node-form').not('[inspect-test="1"]')).length) {
        // Once.
        $elm.attr('inspect-test', '1');

        // Log inspection of global Drupal to browser console (if exists).
        inspect(Drupal);

        // Log inspection of global Drupal to backend log.
        inspect.log(Drupal, 'Drupal');
        inspect.log(Drupal, {
          message: 'Drupal, displaying function bodies',
          func_body: true
        });

        try {
          throw new Error('Oops');
        }
        catch (er) {
          // Log an error to console (if exists).
          inspect.trace(er);
          // Log an error to backend log.
          inspect.traceLog(er);
        }
      }
    }
  };
}(jQuery));
