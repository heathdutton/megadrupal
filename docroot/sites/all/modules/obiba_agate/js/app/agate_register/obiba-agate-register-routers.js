/**
 * @file
 * Obiba Agate Module AngularJs App Routers config.
 */

'use strict';

(function ($) {
  Drupal.behaviors.obiba_agate_register_routes = {
    attach: function (context, settings) {

      mica.agateRegister.config(['$routeProvider', '$locationProvider',
        function ($routeProvider, $locationProvider) {
          $routeProvider
            .when('/join', {
              templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_gate/obiba_agate-user-profile-register-form',
              controller: 'RegisterFormController'
            });
        }]);

    }
  }
}(jQuery));
