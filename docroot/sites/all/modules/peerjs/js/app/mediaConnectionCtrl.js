/**
 * @file
 * Chat controller.
 */

(function () {
  'use strict';

  angular.module('PeerJSChat')
    .controller('MediaConnectionCtrl', function ($scope, $sce, EntityService) {
      $scope.user = {};
      $scope.remoteStreamUrl = '';

      $scope.init = function () {
        $scope.user = $scope.mediaConnection.metadata.user;

//        // Get user information.
//        EntityService.get({
//          type: 'peerjs_peer',
//          pid: $scope.mediaConnection.peer
//        }).$promise.then(function (data) {
//            if (data.list.length === 1) {
//              EntityService.get({ type: 'user', id: data.list[0].uid }).$promise.then(function (data) {
//                $scope.user = data;
//              });
//            }
//          });
//
        // Set remoteStream url.
        if (angular.isObject($scope.mediaConnection.remoteStream) && !$scope.mediaConnection.remoteStream.ended) {
          $scope.remoteStreamUrl = $sce.trustAsResourceUrl(URL.createObjectURL($scope.mediaConnection.remoteStream));
        }
        else {
          $scope.mediaConnection.on('stream', function (stream) {
            $scope.remoteStreamUrl = $sce.trustAsResourceUrl(URL.createObjectURL(stream));
          });
        }
      };

      /**
       * Close the mediaConnection.
       */
      $scope.hangup = function () {
        $scope.mediaConnection.close();
      };

    });
}());
