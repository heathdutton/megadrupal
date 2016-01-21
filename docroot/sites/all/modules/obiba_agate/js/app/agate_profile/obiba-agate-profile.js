/**
 * @file
 * JavaScript ajax helper for Statistics variables retrieving
 */
(function ($) {
  Drupal.behaviors.obiba_agate_profile = {
    attach: function (context, settings) {
      'use strict';
      /* App Module */
      mica.agateProfile = angular.module('mica.agateProfile', [
        'ui.bootstrap',
        'schemaForm',
        'vcRecaptcha'
      ]);
    }
  }
}(jQuery));
