/**
 * @file
 * JavaScript ajax helper for Statistics variables retrieving
 */
(function ($) {
  Drupal.behaviors.obiba_agate_profile_controllers = {
    attach: function (context, settings) {

      'use strict';

      mica.agateProfile.controller('ModalPasswordUpdateController',
        ['$scope',
          '$modalInstance',
          'userId',
          'AlertService',
          'AgateUserPassword',
          function ($scope,
                    $modalInstance,
                    userId,
                    AlertService,
                    AgateUserPassword) {
            $scope.ok = function () {
              /*Validation*/
              if ($scope.profile.NewPassword !== $scope.profile.ConfirmPassword) {
                AlertService.alert({
                  id: 'ModalPasswordUpdateController',
                  type: 'danger',
                  msg: Drupal.t('The password and its confirmation do not match!')
                });
              } else {
                AgateUserPassword.save('', {
                    currentPassword: $scope.profile.password,
                    newPassword: $scope.profile.NewPassword
                  }, function (response) {
                    if (response.errorServer) {
                      AlertService.alert({
                        id: 'ModalPasswordUpdateController',
                        type: 'danger',
                        msg: Drupal.t('Server Error :') + response.errorServer
                      });
                    } else {
                      AlertService.alert({
                        id: 'MainController',
                        type: 'success',
                        msg: Drupal.t('The changes have been saved.')
                      });
                      $modalInstance.close();
                    }
                  }
                );
              }
            };

            $scope.cancel = function () {
              $modalInstance.dismiss('cancel');
            };
          }]);

      mica.agateProfile.controller('UserViewProfileController', [
        '$rootScope',
        '$scope',
        '$sce',
        'AgateFormResource',
        'AgateUserProfile',
        '$modal',
        function ($rootScope,
                  $scope,
                  $sce,
                  AgateFormResource,
                  AgateUserProfile,
                  $modal) {

          AgateFormResource.get(function onSuccess(FormResources) {
            $scope.model = {};

            $scope.form = FormResources.form;
            $scope.schema = FormResources.schema;
            $scope.schema.readonly = true;
            AgateUserProfile.get(function onSuccess(userProfile) {
              userProfile.userProfile.username = Drupal.settings.agateParam.userId;
              $scope.model = userProfile.userProfile;
              $scope.DrupalProfile = $sce.trustAsHtml(userProfile.drupalUserDisplay);

              /*********U P D A T E    P A S S W O R D   U S E R ********************/

              $scope.updatePasswordUser = function () {
                var modalInstance = $modal.open({
                  templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_agate/obiba-agate-user-update-password-modal',
                  controller: 'ModalPasswordUpdateController',
                  resolve: {
                    userId: function () {
                      return $scope.model.username;
                    }
                  }
                });
                modalInstance.result.then(function (data) {
                  $scope.profile = data;
                }, function () {
                  console.log('Modal dismissed at: ' + new Date());
                });

              };
            });
          });

        }]);

      mica.agateProfile.controller('UserEditProfileController', ['$scope',
        '$location',
        'AlertService',
        'AgateUserProfile',
        'AgateFormResource',
        function ($scope,
                  $location,
                  AlertService,
                  AgateUserProfile,
                  AgateFormResource) {
          AgateFormResource.get(
            function onSuccess(AgateProfileForm) {
              $scope.model = {};
              $scope.form = AgateProfileForm.form;
              $scope.schema = AgateProfileForm.schema;
              $scope.response = null;
              if ($scope.schema.properties.username) {
                $scope.schema.properties.username.readonly = true;
              }
              AgateUserProfile.get(function onSuccess(userProfile) {
                userProfile.userProfile.username = Drupal.settings.agateParam.userId;
                $scope.model = userProfile.userProfile;

              });

              $scope.onSubmit = function (form) {
                $scope.$broadcast('schemaFormValidate');
                if (form.$valid) {
                  AgateUserProfile.save($scope.model, function (response) {
                    response.locationRedirection = response.locationRedirection ? response.locationRedirection : 'view';
                    if (response && !response.errorServer) {
                      AlertService.alert({
                        id: 'MainController',
                        type: 'success',
                        msg: Drupal.t('The changes have been saved.')
                      });

                    }
                    else {
                      AlertService.alert({
                        id: 'MainController',
                        type: 'warning',
                        msg: response.errorServer
                      });
                    }
                    $location.path(response.locationRedirection).replace();
                  });
                }
              }

            }
          );

        }]);

    }
  }
}(jQuery));
