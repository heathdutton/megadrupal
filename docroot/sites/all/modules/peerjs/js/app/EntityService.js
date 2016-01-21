/**
 * @file
 * Entity service.
 */

(function () {
  'use strict';

  angular.module('PeerJSChat')
    .factory('EntityService', function ($resource, Settings) {
      var url = Settings.basePath + ':entity_type/:id';
      var paramDefaults = {};
      var actions = {
        update: {
          method: 'PUT',
          isArray: true
        },
        delete: {
          method: 'DELETE',
          isArray: true
        },
        save: {
          method: 'POST',
          responseType: 'json'
        }
      };

      return $resource(url, paramDefaults, actions);
    });
}());
