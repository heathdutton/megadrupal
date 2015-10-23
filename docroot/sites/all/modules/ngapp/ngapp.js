/**
 * @file
 * Code for the ngApp module.
 */

// Global variable for the Angular module.
var $drupalApp;

(function() {
  'use strict';

  // Service to wrap access to the global Drupal.settings object.
  var drupalSettings = [function() {
    var get = function(key) {
      return Drupal.settings[key];
    };

    return get;
  }];

  // Factory to expose the current Drupal user.
  var drupalUser = ['$q', 'drupalSettings', function($q, drupalSettings) {
    var settings = drupalSettings('ngApp').factory;

    return $q(function(resolve, reject) {
      resolve(settings.drupalUser);
    });
  }];

  // Factory to get/set values in browser's localStorage. It also checks for
  // clearing of Drupal caches and flushes connected localStorages accordingly.
  var drupalLocalStorage = ['$window', '$http', 'drupalSettings', function($window, $http, drupalSettings) {
    var settings = drupalSettings('ngApp').factory;

    var factory = {
      flushing: false
    };

    factory.get = function(key) {
      return $window.localStorage.getItem(key);
    };

    factory.set = function(key, data) {
      var result = null;

      if (data) {
        result = $window.localStorage.setItem(key, data);
      }
      else {
        result = $window.localStorage.removeItem(key);
      }

      if (!factory.flushing) {
        if (settings.drupalLocalStorage.save == true) {
          var storage = angular.toJson($window.localStorage);
          $http.post('ngapp/factory', storage);
        }
      }

      return result;
    };

    var init = function() {
      for (var p in settings) {
        var value = factory.get(p);
        if (!value) {
          value = {};

          // Set the client-side UTC flush baseline to compare server-side
          // "cache last flushed" UTC to.
          if (p == 'drupalLocalStorage') {
            value.flush = new Date().getTime();
          }

          factory.set(p, JSON.stringify(value));
        }
      }
    };

    var checkFlush = function() {
      var storage = JSON.parse(factory.get('drupalLocalStorage'));

      if (settings.drupalLocalStorage.flush) {
        var utc_server = parseInt(settings.drupalLocalStorage.flush);
        var utc_client = parseInt(storage.flush);

        if (utc_server >= utc_client) {
          // Clear all client local storage.
          factory.flushing = true;
          for (var p in $window.localStorage) {
            factory.set(p, null);
          }

          // Re-initialize factory vars in local storage.
          init();
          factory.flushing = false;
        }
      }
    };

    init();
    checkFlush();

    return factory;
  }];

  // Interceptor for all HTTP requests from the app.
  var drupalInterceptor = function() {
    this.$get = ['$q', function($q) {
      var provider = {};

      // This will happen on all HTTP requests (see module config below).
      provider.request = function(config) {
        return config;
      };

      provider.response = function(response) {
        // TODO: consider allowing response to mark trusted HTML.
        // for (var key in response.data) {
        //   if (key.endsWith('TrustAsHtml')) {
        //     var trustAsHtmlKey = key.substring(0, key.indexOf('TrustAsHtml'));
        //     var data = response.data[trustAsHtmlKey];
        //   }
        // }

        return response;
      };

      // Forces Angular to stop processing if it received bad HTTP response.
      provider.responseError = function(response) {
        return $q.reject(response);
      };

      return provider;
    }];
  };

  // Factory to expose query "magic methods" to the app.
  var drupalServices = ['$http', '$q', 'drupalSettings', function($http, $q, drupalSettings) {
    var settings = drupalSettings('ngApp').services;

    var factory = {};
    if (!settings) {
      return factory;
    }

    // Set magic methods for each exposed Services resource.
    // @see https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/bind
    var stub = {
      obj: null,
      get: function(arg) {
        var meta = this.obj;

        var path = [
          meta.path,
          arg
        ];

        path = path.join('/');

        var headers = {
          'Content-Type': 'application/json',
          'Accept': 'application/json'
        };

        if (settings.tokens[meta.source]) {
          headers['X-CSRF-Token'] = settings.tokens[meta.source];
        }

        return $http({
            method: 'GET',
            headers: headers,
            url: path
          })
          .then(function(result) {
            // Note: output of path is only for demo purposes; the request for
            // JSON is set via request header, not through the ".json"
            // extension. But for the endpoint path to work as a basic link
            // in the browser we're doing this:
            if (meta.source == 'restws') {
              path = path.replace('/', '.json');
            }

            return {
              path: path,
              data: result.data
            };
          });
      }
    };

    for (var key in settings.resources.GET) {
      factory[key] = stub.get.bind({
        obj: settings.resources.GET[key]
      });
    }

    factory.help = function() {
      return stub.get.bind({
        obj: {
          path: 'ngapp/help',
          source: 'ngapp'
        }
      })()
      .then(function(result) {
        return result.data.services;
      });
    };

    factory.context = function() {
      return stub.get.bind({
        obj: {
          path: 'ngapp/context?path=' + document.location.pathname,
          source: 'ngapp'
        }
      })()
      .then(function(result) {
        return result.data;
      });
    };

    return factory;
  }];

  // Initialize the global Angular module.
  $drupalApp = angular
    .module(Drupal.settings.ngApp.name, [
      'ngAnimate',
      'ngSanitize'
    ])
    .constant('drupalAppPath', Drupal.settings.ngApp.path)
    .provider('drupalInterceptor', drupalInterceptor)
    .config(['$httpProvider', '$compileProvider', function($httpProvider, $compileProvider) {
      $httpProvider
        .interceptors
        .push('drupalInterceptor');

      // Apply some performance boosts to versions gte 1.3.
      // @see https://keyholesoftware.com/2014/11/17/new-features-in-angularjs-1-3
      if (angular.version.major == 1 && angular.version.minor >= 3) {
        $httpProvider
          .useApplyAsync(true);

        $compileProvider
          .debugInfoEnabled(false);
      }
    }])
    .factory('drupalSettings', drupalSettings)
    .factory('drupalUser', drupalUser)
    .factory('drupalLocalStorage', drupalLocalStorage)
    .factory('drupalServices', drupalServices);
}());
