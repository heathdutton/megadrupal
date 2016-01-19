/**
 * @file
 * universalMuted directive.
 *
 * Fixes issue where firefox doesn't respect muted attribute as per:
 * https://github.com/angular/angular.js/issues/7474
 */

(function () {
  'use strict';

  angular.module('PeerJSChat')
    .directive('universalMuted', function () {
      return {
        restrict: 'A',
        scope: {},
        link: function (scope, element, attributes) {
          element[0].muted = true;
        }
      };
    });
}());
