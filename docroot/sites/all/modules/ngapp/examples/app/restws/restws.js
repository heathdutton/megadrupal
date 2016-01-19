/**
 * @file
 * Code for the ngApp module examples.
 */

(function() {
  'use strict';

  var ControllerRestWS = ['drupalLocalStorage', 'drupalServices', function(drupalLocalStorage, drupalServices) {
    // Bind this to vm (view-model).
    var vm = this;

    // Get filters from local storage.
    var filters = drupalLocalStorage.get('restws');
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
      drupalServices
        .getTaxonomyTerms('?vocabulary[id]=1')
        .then(function(result) {
          vm.tags = result.data.list;

          for (var i = 0; i < vm.tags.length; i++) {
            vm.tags[i].active = (filters.tags.indexOf(vm.tags[i].tid) >= 0);
          }
        });

      var path = '?type=article&limit=20';
      drupalServices
        .getNodes(path)
        .then(function(result) {
          vm.data = result.data.list;
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
      drupalLocalStorage.set('restws', JSON.stringify(filters));
      setFilters();
    };

    // Set active filter bools and data in vm.
    setFilters();
  }];

  // Bind component to globally scoped Angular module.
  $drupalApp
    .controller('ControllerRestWS', ControllerRestWS);

}());
