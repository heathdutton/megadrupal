/**
 * @file
 */
(function ($) {

  "use strict";

  Drupal.behaviors.experienceSelect = {};

  Drupal.behaviors.experienceSelect.attach = function (context, settings) {
    var $widget = $('.form-type-experience-select').children('div').once('experience');
    var i;
    for (i = 0; i < $widget.length; i++) {
      new Drupal.experience.FresherHandler($widget[i]);
    }
  };

  Drupal.experience = Drupal.experience || {};

  /**
   * Constructor for the FresherHandler object.
   *
   * The FresherHandler is responsible for synchronizing a experience select widget's
   * year with its month. 
   * 
   * @param widget
   *   The fieldset DOM element containing the from and to experience.
   */
  Drupal.experience.FresherHandler = function (widget) {
    this.$widget = $(widget);
    this.$year = this.$widget.find('.year-entry');
    if (this.$year.val() === 'fresher') {
      this.$month = this.$widget.find('div[class$=month]');
      this.$month.hide();
    }
    else {
      this.$month = this.$widget.find('div[class$=month]');
      this.$month.show();
    }
    this.initializeSelects();
    this.bindChangeHandlers();
  };

  /**
   * Store all the select dropdowns in an array on the object, for later use.
   */
  Drupal.experience.FresherHandler.prototype.initializeSelects = function () {
    var $years = this.$year;
    var $year;
    var i;
    var id;
    this.selects = {};
    for (i = 0; i < $years.length; i++) {
      $year = $($years[i]);
      id = $year.attr('id');
      this.selects[id] = {
        'id': id,
        'year': $year
      };
    }
  };

  /**
   * Add a change handler to each of the start experience's year select dropdowns.
   */
  Drupal.experience.FresherHandler.prototype.bindChangeHandlers = function () {
    var id;
    for (id in this.selects) {
      if (this.selects.hasOwnProperty(id)) {
        this.selects[id].year.bind('change.FresherHandler', this.startChangeHandler.bind(this));
      }
    }
  };

  /**
   * Change event handler for each of the start experience's year select dropdowns.
   */
  Drupal.experience.FresherHandler.prototype.startChangeHandler = function (event) {
    this.syncFresherMonth();
  };

  Drupal.experience.FresherHandler.prototype.syncFresherMonth = function () {
    var id;
    for (id in this.selects) {
      if (this.selects.hasOwnProperty(id)) {
        if (this.selects[id].year.val() === 'fresher') {
          this.$month = this.$widget.find('div[class$=month]');
          this.$month.hide();
        }
        else {
          this.$month = this.$widget.find('div[class$=month]');
          this.$month.show();
        }
      }
    }
  };

}(jQuery));
