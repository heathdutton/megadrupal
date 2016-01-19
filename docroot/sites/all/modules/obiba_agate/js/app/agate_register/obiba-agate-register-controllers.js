/**
 * @file
 * Obiba Agate Module AngularJs App Controller.
 */

(function ($) {
  Drupal.behaviors.obiba_agate_register_controllers = {
    attach: function (context, settings) {

      'use strict';
      mica.agateRegister.controller('RegisterFormController',
        ['$scope',
          '$log',
          'UserResourceJoin',
          'vcRecaptchaService',
          'AgateJoinFormResource',
          function ($scope,
                    $log,
                    UserResourceJoin,
                    vcRecaptchaService,
                    AgateJoinFormResource) {
            AgateJoinFormResource.get(
              function onSuccess(AgateProfileForm) {
                $scope.form = AgateProfileForm.form;
                $scope.schema = AgateProfileForm.schema;
                $scope.config = {
                  key: AgateProfileForm.captchaConfig
                };
                $scope.response = null;
                $scope.widgetId = null;
                $scope.model = {};

                $scope.setWidgetId = function (widgetId) {
                  $scope.widgetId = widgetId;
                };
                $scope.setResponse = function (response) {
                  $scope.response = response;
                };

                $scope.setWidgetId = function (widgetId) {
                  $scope.widgetId = widgetId;
                };
                $scope.onSubmit = function (form) {
                  // First we broadcast an event so all fields validate themselves
                  $scope.$broadcast('schemaFormValidate');
                  // Then we check if the form is valid
                  if (form.$valid) {
                    UserResourceJoin.post(angular.extend({}, $scope.model, {reCaptchaResponse: $scope.response})).
                      success(function (data, status, headers, config) {
                        $scope.alert = {
                          message: Drupal.t('You will receive an email to confirm your registration with the instructions to set your password.'),
                          type: 'success'
                        };
                        var elem = document.getElementById("obiba-user-register");
                        angular.element(elem).remove();

                      })
                      .error(function (data, status, headers, config) {
                        var errorParse = angular.fromJson(data);
                        console.log('Error Server Code:' + status);
                        $scope.alert = {
                          message: Drupal.t(angular.fromJson(errorParse.errorMessage).message),
                          type: 'danger'
                        };
                        //re-load ReCaptcha field
                        vcRecaptchaService.reload($scope.widgetId);

                      });
                  }
                  $scope.closeAlert = function () {
                    $scope.alert = [];
                  };
                };
                $scope.onCancel = function (form) {
                  window.location = Drupal.settings.basePath;
                }

              }
            );
          }]);

    }
  }
}(jQuery));
