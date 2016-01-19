/**
 * @file
 * Obiba Agate Module AngularJs App.
 */

(function ($) {
  Drupal.behaviors.obiba_agate_register = {
    attach: function (context, settings) {

      'use strict';
      /* App Module */
      mica.agateRegister = angular.module('mica.agateRegister', [
        'ui.bootstrap',
        'schemaForm',
        'vcRecaptcha'
      ]);

    }
  }
}(jQuery));
