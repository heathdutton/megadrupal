'use strict';

angular.module('lightQuizApp.controllers', []);

angular.module('lightQuizApp.controllers').controller("TestCtrl", function($rootScope, $scope, $location, TestService, SessionService) {
  $rootScope.endTime = 0;

  $scope.isDisabled = function() {
    return !SessionService.isSupported();
  };

  TestService.get().then(function(test) {
    $scope.test = test;

    $scope.start = function() {
      $rootScope.endTime = Date.now() + (test.time_limit * 1000);

      SessionService.setAllAnswers(test.questions.map(function() {
        return undefined;
      }));

      $location.path('test/0');
    };
  });
});

angular.module('lightQuizApp.controllers').controller('QuestionCtrl', function($scope, $routeParams, $location, SessionService, QuestionService, ResultService, TestService) {
  var index = $routeParams.questionId ? parseInt($routeParams.questionId) : 0;

  $scope.time = Date.now();
  $scope.userAnswer = SessionService.getAnswer(index);

  QuestionService.get(index).then(function(question) {
    $scope.question = question;
  });

  TestService.get().then(function(test) {
    $scope.finish = function() {
      var result = ResultService.getResult(test);

      ResultService.sendResult(result).then(function() {
        $location.path('result');
      });
    };
  });

  $scope.getType = function(attachment) {
    return attachment.content_type.split('/')[0];
  };

  $scope.setAnswer = function(answerKey) {
    SessionService.setAnswer(index, answerKey);
    $scope.userAnswer = answerKey;
  };

  $scope.back = function() {
    $location.path('test/' + (index - 1));
  };

  $scope.next = function() {
    $location.path('test/' + (index + 1));
  };
});

angular.module('lightQuizApp.controllers').controller('ResultCtrl', function($scope, $location, SessionService, TestService, ResultService) {
  $scope.result = {};

  $scope.open = function(index) {
    $location.path('result/' + index);
  };

  TestService.get().then(function(test) {
    $scope.test = test;

    $scope.wrongAnswers = ResultService.wrongAnswers(test);
    $scope.incompleteAnswers = ResultService.incompleteAnswers(test);

    $scope.result = ResultService.getResult(test);
  });
});

angular.module('lightQuizApp.controllers').controller('FeedbackCtrl', function($scope, $routeParams, $location, QuestionService, SessionService) {
  var index = $routeParams.questionId ? parseInt($routeParams.questionId) : 0;
  var _userAnswers = SessionService.getAllAnswers();

  $scope.getType = function(attachment) {
    return attachment.content_type.split('/')[0];
  };

  $scope.back = function() {
    $location.path('result');
  };

  QuestionService.get(index).then(function(question) {
    $scope.question = question;
    
    $scope.userAnswer = _userAnswers[index] != undefined
        ? question.answers[_userAnswers[index]].value
        : undefined;
        
      $scope.correctAnswers = question.answers.map(function(answer) {
        if (answer.correct) return answer.value;
      }).filter(Boolean);
  });
});