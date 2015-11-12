 'use strict';

/* Services */
angular.module('restwsSearch', [], function($provide) {
  $provide.factory('restwsSearch', ['$http', '$filter', '$q', function($http, $filter, $q) {
    var restwsSearch = {
      transformData: function(data) {
        var facets = {};
        
        angular.forEach(data.facets, function(facetValue, facetKey) {
          angular.forEach(facetValue.items, function(itemValue, itemKey) {
            itemValue.path = itemValue.path.replace(restwsSearch.url + '?', '');
            itemValue.active = (itemValue.active == 1);
          });
        });

        return data;
      },
      async : function(url, params) {
        restwsSearch.url = url;
        
        var promise = $http({
          method : 'GET',
          url : url,
          params: params,
          cache : true
        }).then(function(response) {
          return restwsSearch.transformData(response.data);
        });
        // Return the promise to the controller
        return promise;
      }
    };

    return restwsSearch;
  }]);
});