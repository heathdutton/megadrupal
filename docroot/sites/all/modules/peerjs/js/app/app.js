/**
 * @file
 * Angular app module file.
 * v0.1
 */

(function () {
  'use strict';

  // Compatibility shim for getUserMedia.
  navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia;

  angular.module('PeerJSChat', [
    'ngResource',
    'ngSanitize'
  ])
    .constant('Settings', Drupal.settings.peerjs)
    .config(function ($httpProvider) {

      // Add Drupal RestWS module specific configuration.
      $httpProvider.defaults.headers.common['X-CSRF-Token'] = Drupal.settings.peerjs.restws_csrf_token;
      $httpProvider.defaults.headers.common['Content-Type'] = 'application/json';
    });
}());
