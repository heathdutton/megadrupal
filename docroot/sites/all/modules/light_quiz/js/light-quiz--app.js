'use strict';

// Bootstrap angular app.
angular.element(document).ready(function() {
  angular.bootstrap(document.getElementById('light-quiz-app'), ['lightQuizApp']);
});

angular.module("lightQuizApp", ['ngRoute', 'ngSanitize', 'timer', 'lightQuizApp.controllers', 'lightQuizApp.services'])

.config(function($routeProvider) {
  $routeProvider
  .when('/', {
    templateUrl: 'templates/test.html',
    controller: 'TestCtrl'
  })
  .when('/test/:questionId', {
    templateUrl: 'templates/question.html',
    controller: 'QuestionCtrl'
  })
  .when('/result', {
    templateUrl: 'templates/result.html',
    controller: 'ResultCtrl'
  })
  .when('/result/:questionId', {
    templateUrl: 'templates/feedback.html',
    controller: 'FeedbackCtrl'
  });
});
