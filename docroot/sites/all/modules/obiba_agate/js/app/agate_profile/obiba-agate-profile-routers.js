/**
 * @file
 * JavaScript ajax helper for Statistics variables retrieving
 */
'use strict';

(function ($) {
  Drupal.behaviors.obiba_agate_profile_routes = {
    attach: function (context, settings) {

      mica.agateProfile.config(['$routeProvider', '$locationProvider',
        function ($routeProvider, $locationProvider) {
          $routeProvider
           .when('/view', {
              templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_gate/obiba_agate-user-profile-view',
              controller: 'UserViewProfileController'
            }).
            when('/edit', {
              templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_gate/obiba_agate-user-profile-form',
              controller: 'UserEditProfileController'
            })
            .otherwise({
              templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_gate/obiba_agate-user-profile-view',
              controller: 'UserViewProfileController'
            });
        }]);

    }
  }
}(jQuery));
