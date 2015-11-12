/**
 * @file
 * Product picker functionality.
 */

/*global document,Drupal,window,jQuery,Backbone,_*/
(function ($) {
  "use strict";

  Drupal.consultation = Drupal.consultation || {};

  /**
   * Attach consult behaviour to page.
   */
  Drupal.behaviors.consult = {
    attach: function (context, settings) {
      // Find all consult placeholders in the context. If any exist then
      // look for corresponding settings in order to init a consultation.
      $('.js-consult-interview', context).each(function () {
        var interviewName = $(this).attr('data-interview-id');
        if (settings.consult.hasOwnProperty(interviewName)) {
          $(this).consult(settings.consult[interviewName]);
        }
      });
    }
  };

  /**
   * Theme function for consultation choices.
   *
   * @param choices
   *   An object with a key-value pair for each choice.
   * @param activeChoice
   *   The key for the currently selected answer (if there is any).
   * @return
   *   HTML for the buttons.
   */
  Drupal.theme.prototype.consultationChoices = function (choices, activeChoice) {
    var $button, $el = $('<div>');
    $.each(choices, function (delta, choice) {
      $button = $('<a>').addClass('button').addClass('choice').attr('data-choice', choice.key).attr('href', '#!').text(choice.label);
      if (choice.key === activeChoice) {
        $button.addClass('active');
      }
      $el.append($button);
    });
    return $el.html();
  };

  /**
   * Events used by the consult app.
   */
  Drupal.consultation.events = {
    // View events. Used to control when and how elements are rendered.
    REFLOW: 'consultReflow',
    REDRAW: 'consultRedraw',
    // App level events.
    PREFILL: 'consultPrefill',
    RESET: 'consultReset',
    QUESTION_ANSWERED: 'consultQuestionAnswered',
    // Added for completeness, 'change' is a built in to Backbone.Model.
    CHANGE: 'change'
  };

  /**
   * Methods for testing criteria against a set of results.
   * Determines how to assess missing criterion.
   */
  Drupal.consultation.CRITERIA_TEST_TYPE = {
    ASSUME_PASS: 'assumePass',
    ASSUME_FAIL: 'assumeFail'
  };

  /**
   * Criteria Class.
   *
   * A set of criterion as name-value pairs.
   */
  Drupal.consultation.Criteria = Backbone.Model.extend({
    /**
     * Test results for criteria.
     */
    test: function (results, testType) {
      var self = this,
        keys = this.keys(),
        value,
        result = true;

      // If there are no criterion, then the test passes automatically.
      if (_.size(keys) === 0) {
        return true;
      }

      // Default test type.
      if (_.isUndefined(testType)) {
        testType = Drupal.consultation.CRITERIA_TEST_TYPE.ASSUME_FAIL;
      }

      _.each(keys, function (key) {
        value = self.get(key);
        if (testType === Drupal.consultation.CRITERIA_TEST_TYPE.ASSUME_FAIL) {
          if (value !== results[key]) {
            result = false;
          }
        }
        else {
          if (results.hasOwnProperty(key) && value !== results[key]) {
            result = false;
          }
        }
      });

      return result;
    }
  });

  /**
   * Rule Class.A set of Criteria.
   */
  Drupal.consultation.Rule = Backbone.Collection.extend({
    model: Drupal.consultation.Criteria,
    /**
     * Test results for rule.
     *
     * If any criteria pass, then the rule will pass.
     * I.E criterion are AND'ed together, Criteria are OR'ed together.
     * This enables the creation of complex logic rules.
     */
    test: function (results, testType) {
      var result = false;

      // Test passes automatically if there are no criteria.
      if (this.length === 0) {
        return true;
      }

      // Test passes if any criteria pass (they are OR'd together).
      this.each(function (criteria) {
        result = result || criteria.test(results, testType);
      });

      return result;
    }
  });

  /**
   * Result types.
   */
  Drupal.consultation.RESULT_TYPE = {
    QUESTION: 'questions',
    RESULT: 'result'
  };

  /**
   * Result Class.
   *
   * Information that is presented to the user based on their answers.
   */
  Drupal.consultation.Result = Backbone.Model.extend({
    defaults: function () {
      return {
        type: Drupal.consultation.RESULT_TYPE.RESULT,
        markup: "Empty recommendation",
        dependencies: []
      };
    },

    /**
     * Initialization.
     */
    initialize: function (obj) {
      // Convert static 'dependencies' property into model of type Rule.
      this.set('dependencies', new Drupal.consultation.Rule(this.get('dependencies')));
    },

    /**
     * Tests if dependencies are met for this entity.
     */
    testDependencies: function (results) {
      var result;
      if (this.get('shown') === '1') {
        result = this.get('dependencies').test(results, Drupal.consultation.CRITERIA_TEST_TYPE.ASSUME_PASS);
      }
      else {
        result = this.get('dependencies').test(results, Drupal.consultation.CRITERIA_TEST_TYPE.ASSUME_FAIL);
      }
      return result;
    }
  });

  /**
   * Question Class.
   *
   * A single question in the consultation. Also records user's response
   * to the question.
   *
   * Extends Result, with additional properties for answers/response.
   */
  Drupal.consultation.Question = Drupal.consultation.Result.extend({
    defaults: function () {
      return {
        type: Drupal.consultation.RESULT_TYPE.QUESTION,
        name: 'Q0',
        markup: '<div>Empty question<br><br><div class="js-answers"></div></div>',
        answers: {'yes': 'Yes', 'no': 'No'},
        dependencies: [],
        userResponse: null
      };
    }
  });

  /**
   * QuestionList Class.
   *
   * Collection of questions.
   */
  Drupal.consultation.QuestionList = Backbone.Collection.extend({
    model: Drupal.consultation.Question
  });

  /**
   * ResultsList Class.
   *
   * Collection of results.
   */
  Drupal.consultation.ResultsList = Backbone.Collection.extend({
    model: Drupal.consultation.Result
  });

  /**
   * Consultation Class.
   *
   * Class representing the entire consultation process.
   *
   * Can be considered to be the 'app' class.
   */
  Drupal.consultation.Consultation = Backbone.Model.extend({
    initialize: function () {
      // Convert static properties into Backbone collections.
      // These static properties are fed in from Drupal.settings when the
      // object is constructed.
      this.set('questions', new Drupal.consultation.QuestionList(this.get('questions')));
      this.set('results', new Drupal.consultation.ResultsList(this.get('results')));

      this.listenTo(this.get('questions'), Drupal.consultation.events.CHANGE, this.onQuestionChanged);
    },

    /**
     * Gets all answers to questions that have their dependencies met.
     *
     * Questions that have unsatisfied dependencies are ignored.
     */
    getAnswers: function () {
      var i;
      var results = {};
      var lastRound = {};
      var getAnswer = function (question) {
        if (question.testDependencies(lastRound)) {
          var answer = question.get('userResponse');
          if (!_.isNull(answer) && !_.isUndefined(answer)) {
            results[question.get('name')] = answer;
          }
        }
      };

      for (i = 0; i < 100; i++) {
        results = {};
        this.get('questions').each(getAnswer);
        if (_.isEqual(results, lastRound)) {
          break;
        }
        lastRound = results;
      }
      return results;
    },

    /**
     * Set multiple answers.
     *
     * Triggers the 'prefill' event.
     * @param answers
     */
    setAnswers: function (answers) {
      var questions = this.get('questions');
      _.each(answers, function (answer, questionName) {
        // Use the 'silent' option to prevent all views from updating on every
        // iteration of the loop.
        questions.findWhere({name: questionName}).set({userResponse: answer}, {silent: true});
      });
      // When the loop is completed, firing the 'prefill' event updates
      // all views at once.
      this.trigger(Drupal.consultation.events.PREFILL, answers);
    },

    /**
     * Get all results that have been satisfied by the user's answers thus far.
     */
    getResults: function () {
      var answers = this.getAnswers();
      return this.get('results').filter(function (result) {
        return result.testDependencies(answers);
      });
    },

    /**
     * Reset all questions. Triggers the 'reset' event.
     */
    reset: function () {
      this.get('questions').each(function (question) {
        question.set({userResponse: null}, {silent: true});
      });
      this.trigger(Drupal.consultation.events.RESET, {});
    },

    /**
     * Question change handler.
     */
    onQuestionChanged: function (question) {
      // Did the user change their answer to the question?
      if (question.changed.hasOwnProperty('userResponse')) {
        this.trigger(Drupal.consultation.events.QUESTION_ANSWERED, question);
      }
    }
  });

  /**
   * ResultView Class.
   *
   * View for a single Result.
   */
  Drupal.consultation.ResultView = Backbone.View.extend({
    /**
     * Results view init.
     */
    initialize: function () {
      // Element is constructed from markup provided by Drupal.settings.
      this.$el = $(this.model.get('markup'));
      this.el = this.$el.get(0);
      this.listenTo(this.model, Drupal.consultation.events.CHANGE, this.render);
    },

    /**
     * Empty function.
     *
     * Used to maintain Backbone conventions.
     */
    render: function () {
      // Enables chaining convenience.
      return this;
    },

    /**
     * Hide/show element by testing dependencies.
     *
     * Called when answers change to other questions.
     */
    reflow: function (results) {
      var result = this.model.testDependencies(results);
      if (result) {
        this.$el.removeClass('entity-inactive');
      }
      else {
        this.$el.addClass('entity-inactive');
      }
    }
  });

  /**
   * QuestionView Class.
   *
   * View for Questions. Inherits from ResultView, with additional functionality
   * for button interactivity.
   */
  Drupal.consultation.QuestionView = Drupal.consultation.ResultView.extend({
    events: {
      'click .choice': 'clickChoice'
    },

    /**
     * Results render.
     *
     * Called when model is changed.
     */
    render: function (event) {
      // Rendering of buttons will change depending on how user answered.
      var choicesMarkup = Drupal.theme('consultationChoices', this.model.get('answers'), this.model.get('userResponse'));
      this.$el.find('.js-answers').html(choicesMarkup);
      // Add class that indicates if the question has been answered.
      if (this.model.get('userResponse')) {
        this.$el.addClass('consult-question-answered');
      }
      else {
        this.$el.removeClass('consult-question-answered');
      }
      return this;
    },

    /**
     * Choice click handler.
     *
     * Sets 'userResponse' on model.
     */
    clickChoice: function (event) {
      // Set 'userResponse' property based on which button was clicked.
      var userResponse = $(event.currentTarget).attr('data-choice');
      this.model.set('userResponse', userResponse);
      // Trigger jQuery event on dom.
      this.$el.trigger(Drupal.consultation.events.QUESTION_ANSWERED, [{
        name: this.model.get('name'),
        answer: this.model.get('userResponse'),
        el: this.el
      }]);
    }
  });

  /**
   * ConsultationView Class.
   *
   * View for Consultation (top-level view).
   */
  Drupal.consultation.ConsultationView = Backbone.View.extend({
    /**
     * Init ofr main view.
     */
    initialize: function () {
      var self = this;

      // Listen for app events; this.model is the top-level Consultation model.
      this.listenTo(this.model, Drupal.consultation.events.QUESTION_ANSWERED, this.onQuestionAnswered);
      this.listenTo(this.model, Drupal.consultation.events.PREFILL, this.onPrefill);
      this.listenTo(this.model, Drupal.consultation.events.RESET, this.onReset);

      // Iterate through question groups and populate group containers.
      _.each(this.model.get('questions').groupBy('type'), function (groupQuestions, groupName) {
        // Container for group; fallback to 'js-questions' if not found.
        var $groupEl = self.$el.find("[data-group-name='" + groupName + "']");
        if ($groupEl.length === 0) {
          $groupEl = self.$el.find('.js-questions');
        }
        // Iterate through each question in this group, and populate view.
        _.each(groupQuestions, function (question) {
          var questionView = new Drupal.consultation.QuestionView({model: question});
          // Question views need to respond to global redraw and reflow events.
          questionView.listenTo(self, Drupal.consultation.events.REFLOW, questionView.reflow);
          questionView.listenTo(self, Drupal.consultation.events.REDRAW, questionView.render);
          $groupEl.append(questionView.render().$el);
        });
      });

      // Iterate through result groups and populate group containers.
      _.each(this.model.get('results').groupBy('type'), function (groupResults, groupName) {
        // Container for group; fallback to 'js-results' if not found.
        var $groupEl = self.$el.find("[data-group-name='" + groupName + "']");
        if ($groupEl.length === 0) {
          $groupEl = self.$el.find('.js-results');
        }
        // Iterate through each result in this group, and populate view.
        _.each(groupResults, function (result) {
          var resultView = new Drupal.consultation.ResultView({model: result});
          // Result views only need to respond to reflow events (not redraw).
          resultView.listenTo(self, Drupal.consultation.events.REFLOW, resultView.reflow);
          $groupEl.append(resultView.render().$el);
        });
      });

      // Trigger dependencies to be resolved.
      this.trigger(Drupal.consultation.events.REFLOW, this.model.getAnswers());
    },

    /**
     * Question changed event handler.
     *
     * Triggers views to reflow only (i.e. hide or show).
     */
    onQuestionAnswered: function (question) {
      var results = this.model.getAnswers();
      this.trigger(Drupal.consultation.events.REFLOW, results);
    },

    /**
     * Answers reset event handler.
     */
    onReset: function () {
      this.reflowAndRedraw({});
      this.$el.trigger(Drupal.consultation.events.RESET);
    },

    /**
     * Answers pre-fill event handler.
     */
    onPrefill: function (answers) {
      this.reflowAndRedraw(answers);
      this.$el.trigger(Drupal.consultation.events.PREFILL, [answers]);
    },

    /**
     * All views reflow and redraw event handler.
     */
    reflowAndRedraw: function (answers) {
      this.trigger(Drupal.consultation.events.REDRAW);
      this.trigger(Drupal.consultation.events.REFLOW, answers);
    }
  });

  // Define private vars for jQuery plugin.
  var interviews = [];
  var publicMethods = ['getAnswers', 'setAnswers', 'getResults', 'reset'];

  /**
   * Define jQuery plugin function.
   */
  $.fn.consult = function (method) {
    var self = this,
      params = _.toArray(arguments).slice(1),
      results = [];

    if (self.length === 0) {
      return;
    }

    this.each(function () {
      var el = this;
      // If method is specified (and its a public method), then call it.
      if (_.isString(method) && _.contains(publicMethods, method)) {
        // Find the interview for this element.
        var interview = _.find(interviews, function (interview) {
          return interview.view.el === el;
        });
        // If an interview exists for the given element, call the method on it.
        if (interview) {
          var func = interview[method];
          var result = func.apply(interview, params);
          if (!_.isUndefined(result)) {
            results.push(result);
          }
        }
      }
      // If method is an object, then init an interview using the object as
      // the constructor's settings.
      else if (_.isObject(method)) {
        var interviewName = $(this).attr('data-interview-id');
        var consultation = new Drupal.consultation.Consultation(method);
        consultation.view = new Drupal.consultation.ConsultationView({
          model: consultation,
          name: interviewName,
          el: this
        });
        interviews.push(consultation);
      }
    });

    // If no results, enable chaining by returning original jQuery obj.
    if (results.length === 0) {
      return self;
    }

    // If only one result, return it.
    if (results.length === 1) {
      return results[0];
    }

    // Return multiple results (occurs if jQuery obj has multiple elements).
    return results;
  };
}(jQuery));
