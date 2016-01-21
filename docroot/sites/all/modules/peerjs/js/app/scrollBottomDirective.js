/**
 * @file
 * scrollIntoView directive.
 */

(function () {
  'use strict';

  angular.module('PeerJSChat')
    .directive('scrollBottom', function () {
      return {
        scope: {
          scrollBottom: "=",
          minHeight: "=",
          maxHeight: "="
        },
        link: function (scope, element, attributes) {
          element.css({
            'min-height': scope.minHeight,
            'max-height': scope.maxHeight,
            'overflow': 'auto'
          });

          scope.$watchCollection('scrollBottom', function (newValue) {
            if (newValue) {
              element[0].scrollTop = element[0].scrollHeight;
            }
          });
        }
      };
    });
}());
