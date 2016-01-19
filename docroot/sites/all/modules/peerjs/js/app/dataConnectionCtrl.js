/**
 * @file
 * Chat controller.
 */

(function () {
  'use strict';

  angular.module('PeerJSChat')
    .controller('DataConnectionCtrl', function ($scope, $filter, EntityService, Settings) {
      $scope.user = {};
      $scope.formData = {};
      $scope.messages = [];

      $scope.init = function () {
        $scope.user = $scope.dataConnection.metadata.user;

        // Handle dataConnection data event.
        $scope.dataConnection.on('data', function (data) {
          if (data.type === 'private') {
            $scope.messages.push(data);
            $scope.formData.messageToggle = true;
          }
        });
      };

      /**
       * Send data to the dataConnection.
       */
      $scope.message = function () {
        var message = {
          type: 'private',
          peer: $scope.dataConnection.peer,
          user: Settings.user,
          text: $scope.formData.messageText
        };

        $scope.dataConnection.send(message);
        $scope.messages.push(message);
        $scope.formData.messageText = '';
      };
    });
}());
