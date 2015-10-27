/**
 * @file
 * Code for the ngApp module examples.
 */

(function() {
  'use strict';

  var ControllerViewsJson = ['drupalLocalStorage', 'drupalServices', function(drupalLocalStorage, drupalServices) {
    // Bind this to vm (view-model).
    var vm = this;

    // Get filters from local storage.
    var filters = drupalLocalStorage.get('views_json');
    if (filters) {
      filters = JSON.parse(filters);
    }
    else {
      filters = {
        tags: []
      };
    }

    // Function for updating vm active filters bools and fetching new data.
    var setFilters = function() {
      vm.filter_1 = (filters.tags.indexOf('Term 1') >= 0);
      vm.filter_2 = (filters.tags.indexOf('Term 2') >= 0);
      vm.filter_3 = (filters.tags.indexOf('Term 3') >= 0);

      drupalServices
        .getArticles('?tags=' + filters.tags.join(','))
        .then(function(result) {
          vm.data = result.data;
          vm.endpoint = result.path;
        });
    };

    // Filter button click action.
    vm.doFilter = function(value) {
      var ix = filters.tags.indexOf(value);
      if (ix >= 0) {
        filters.tags.splice(ix, 1);
      }
      else {
        filters.tags.push(value);
      }

      // Add filter to local storage so its persisted between page refreshes.
      drupalLocalStorage.set('views_json', JSON.stringify(filters));
      setFilters();
    };

    // Set active filter bools and data in vm.
    setFilters();
  }];

  // Bind component to globally scoped Angular module.
  $drupalApp
    .controller('ControllerViewsJson', ControllerViewsJson);

}());
