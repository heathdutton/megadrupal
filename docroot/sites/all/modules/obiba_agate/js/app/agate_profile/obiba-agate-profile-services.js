/**
 * @file
 * JavaScript ajax helper for Statistics variables retrieving
 */
'use strict';

(function ($) {
  Drupal.behaviors.obiba_agate_profile_services = {
    attach: function (context, settings) {

      mica.agateProfile
        .factory('AgateFormResource', ['$resource',
          function ($resource) {
            return $resource(Drupal.settings.basePath + 'agate/agate-form/ws', {}, {
              'get': {
                method: 'GET', errorHandler: true
              }
            });
          }])
        .factory('AgateUserProfile', ['$resource', function ($resource) {
          return $resource(Drupal.settings.basePath + 'agate/agate-user-profile/ws', {}, {
            'save': {method: 'PUT', errorHandler: true},
            'get': {method: 'GET', errorHandler: true}
          });
        }])
        .factory('AgateUserPassword', ['$resource', function ($resource) {
          return $resource(Drupal.settings.basePath + 'agate/agate-user-update-password/ws', {}, {
            'save': {method: 'PUT', errorHandler: true}
          });
        }]);
    }
  }
}(jQuery));
