/**
 * @file
 * Code for the ngApp module examples.
 */

(function() {
  'use strict';

  var ControllerServicesViews = ['drupalLocalStorage', 'drupalServices', function(drupalLocalStorage, drupalServices) {
    // Bind this to vm (view-model).
    var vm = this;

    // Get offset from local storage.
    var offset = drupalLocalStorage.get('offset');
    if (!offset) {
      offset = 0;
    }
    offset = parseInt(offset);

    vm.doPage = function(increment) {
      offset = (offset + increment);
      drupalLocalStorage.set('offset', offset);
      vm.offset = offset;

      // Get the results of the default display for a View named
      // "ngapp_example_services_views" from our REST server.
      drupalServices
        .getViewsRetrieve('ngapp_example_services_views?display_id=default&limit=3&offset=' + vm.offset)
        .then(function(result) {
          vm.data = result.data;
          vm.endpoint = result.path;
        });
    };

    // Initial data binding to vm; use 0 here so offset stays the same between
    // page refreshes.
    vm.doPage(0);
  }];

  // Bind component to globally scoped Angular module.
  $drupalApp
    .controller('ControllerServicesViews', ControllerServicesViews);

}());
