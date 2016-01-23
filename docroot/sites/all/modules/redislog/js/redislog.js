Drupal.behaviors.redisLog = {
  attach: function(context, settings) {
    angular.module("redisLog", []).controller("redisLogCtrl", function($scope) {
      $scope.logs = settings.redislog.logs;
    });
  }
};
