'use strict';

angular.module('lightQuizApp.services', []);

angular.module('lightQuizApp.services').factory('SessionService', function() {
  var STORAGE_ID = 'light-quiz';

  var getAllAnswers = function() {
    return JSON.parse(localStorage.getItem(STORAGE_ID)) || [];
  };

  var setAllAnswers = function(answers) {
    answers = answers || [];

    localStorage.setItem(STORAGE_ID, angular.toJson(answers));
  };

  var getAnswer = function(questionKey) {
    return getAllAnswers()[questionKey];
  };

  var setAnswer = function(questionKey, answerKey) {
    var answers = getAllAnswers();
    answers[questionKey] = answerKey;

    setAllAnswers(answers);
  };

  var isLocalStorageSupported = function () {
    try {
      window.sessionStorage.setItem('test', '1');
      window.sessionStorage.removeItem('test');
      return true;
    }
    catch (error) {
      return false;
    }
  };

  return {
    getAllAnswers: getAllAnswers,
    setAllAnswers: setAllAnswers,
    getAnswer: getAnswer,
    setAnswer: setAnswer,
    isSupported: isLocalStorageSupported
  };
});

angular.module('lightQuizApp.services').factory('QuestionService', function($q, TestService) {
  var getAllQuestions = function() {
    var deferred = $q.defer();

    TestService.get().then(function(test) {
      deferred.resolve(test.questions);
    }, function(message) {
      deferred.reject(message);
    });

    return deferred.promise;
  };

  var getQuestion = function(key) {
    var deferred = $q.defer();

    TestService.get().then(function(test) {
      var question = test.questions[key];

      question.index = key;
      question.total = test.questions.length;

      if (question.attachment) {
        question.attachment_data = test['_attachments'][question.attachment];
      }

      deferred.resolve(question);
    }, function(message) {
      deferred.reject(message);
    });

    return deferred.promise;
  };

  return {
    all: getAllQuestions,
    get: getQuestion
  };
});

angular.module('lightQuizApp.services').factory('ResultService', function($q, $http, SessionService) {
  var sendResult = function(result) {
    return $http.post('/api/light-quiz/result-incoming', result);
  };

  var getResult = function(test) {
    var score = correctAnswers(test).length;
    var percentage = Math.round(score / (test.questions.length / 100));

    return {
      id: test._id,
      score: score,
      percentage: percentage
    };
  };

  var correctAnswers = function(test) {
    return SessionService.getAllAnswers().map(function(value, key) {
      if (value != undefined && test.questions[key].answers[value].correct) return key;
    }).filter(function(value) {
      return value != undefined;
    });
  };

  var wrongAnswers = function(test) {
    return SessionService.getAllAnswers().map(function(value, key) {
      if (value != undefined && !test.questions[key].answers[value].correct) return key;
    }).filter(function(value) {
      return value != undefined;
    });
  };

  var incompleteAnswers = function(test) {
    return SessionService.getAllAnswers().map(function(value, key) {
      if (value == undefined) return key;
    }).filter(function(value) {
      return value != undefined;
    });
  };

  return {
    sendResult: sendResult,
    getResult: getResult,
    correctAnswers: correctAnswers,
    wrongAnswers: wrongAnswers,
    incompleteAnswers: incompleteAnswers
  };
});

Drupal.behaviors.lightQuizTestService = {
  attach: function(context, settings) {
    angular.module('lightQuizApp.services').factory('TestService', function($q) {
      return {
        get: function() {
          var deferred = $q.defer();
          deferred.resolve(settings.light_quiz.test);
          return deferred.promise;
        }
      };
    })
  }
};