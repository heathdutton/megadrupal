/**
 * @file
 * Chat controller.
 */

(function () {
  'use strict';

  angular.module('PeerJSChat')
    .controller('PeerJSChatCtrl', function ($scope, Settings, EntityService, PeerService) {
      $scope.peerService = PeerService;
      $scope.peerService.formData = {};
      $scope.peerService.formData.status = localStorage.getItem('peerStatus') || 0;
      $scope.peerService.formData.videoChatStatus = 0;
      $scope.user = Settings.user;

      $scope.init = function () {
        if ($scope.peerService.formData.status == 1) {
          $scope.peerService.changeStatus(1);
        }
      };
    });
}());


