/**
 * @file
 * Code for the ngApp module examples.
 */

(function() {
  'use strict';

  var ControllerDirective1 = ['$rootScope', function($rootScope) {
    // Bind this to vm (view-model).
    var vm = this;

    $rootScope
      .$on('thingChanged', function(e, data) {
        e.stopPropagation();

        vm.thingChanged = data;
      });

    vm.init = function(e) {
      vm.message = "Here's a registered directive (<code>myDirective1</code>) that uses a controller to output this description. The directive is being output in this example with: <code>&lt;div my-directive-1&gt;&lt;/div&gt;</code>.";
    };
  }];

  // Bind component to globally scoped Angular module.
  $drupalApp
    .directive('myDirective1', ['drupalAppPath', function(drupalAppPath) {
      return {
        controller: ControllerDirective1,
        controllerAs: 'controllerDirective1',
        link: function(scope, element, attrs, controller) {
          controller.init(element);
        },
        // Use "drupalAppPath" constant as a base for a custom directive template:
        templateUrl: drupalAppPath + '/directives/my-test.html'
      };
    }]);

}());
