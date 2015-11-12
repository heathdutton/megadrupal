/**
 * @file panels_accordion.js
 * Used for getting values from Drupal.settings and initiate the accordion
 * for specific IDs. Options for accordion will come from panel display settings.
 * Each settings could be overridden here :-
 * e.g: $options.header = '.pane-title';
 */
(function ($) {
  Drupal.behaviors.panels_accordion = {
    attach: function (context, settings) {
      $.each(Drupal.settings.panels_accordion, function($index_id, $options){
        $('#' + $index_id).accordion($options);
      });
    }
  };
}(jQuery));