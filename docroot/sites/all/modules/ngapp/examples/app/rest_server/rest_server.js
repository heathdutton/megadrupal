/**
 * @file
 * Code for the ngApp module examples.
 */

(function() {
  'use strict';

  var ControllerRestServer = ['drupalServices', function(drupalServices) {
    // Bind this to vm (view-model).
    var vm = this;

    vm.setActive = function(id) {
      // This forces ngAnimate to re-animate, since there's an expression
      // value change in template's bound c.active variable.
      vm.active = false;

      drupalServices
        .getNodeRetrieve(id)
        .then(function(result) {
          vm.active = result.data;
          vm.active.endpoint = result.path;
        });
    };

    drupalServices
      .getNodeIndex('?page=2&pagesize=3&fields=[nid,title]')
      .then(function(result) {
        vm.data = result.data;
        vm.endpoint = result.path;

        // Set the initial active node.
        vm.setActive(vm.data[0].nid);
      });
  }];

  // Bind component to globally scoped Angular module.
  $drupalApp
    .controller('ControllerRestServer', ControllerRestServer);

}());
