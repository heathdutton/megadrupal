/**
 * @file Behaviors for the front-end display of Salesforce-driven picklists.
 */

(function ($) {
  'use strict';

  /**
   * Attaches the show/hide logic for dependent piclists in webforms.
   *
   * @property {function} attach
   *   Adds handler to the forms to update all dependent picklists whenever
   *   a controlling picklist changes values.
   */
  Drupal.behaviors.salesforce_webforms = Drupal.behaviors.salesforce_webforms || {};

  Drupal.behaviors.salesforce_webforms.weight = -10;
  Drupal.behaviors.salesforce_webforms.attach = function(context, settings) {
    // Create state handler for detecting that no options selected.
    if (Drupal.states) {
      // Custom state for select list.
      Drupal.states.Trigger.states.select_not_empty = {
        'change': function () {
          return $('option:selected', this).length ? true : false;
        }
      }
    }
  };
})(jQuery);
