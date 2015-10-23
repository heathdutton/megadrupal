(function ($) {

  var initialized = false;
  var context = 'entity_context';

  function init() {
    initialized = true;
  }

  Drupal.behaviors.entity_context = {
    attach: function (context, settings) {
      if (!initialized) {
        init();
      }
    }
  };

  /**
   * Visitor Context object.
   */
  Drupal.personalize = Drupal.personalize || {};
  Drupal.personalize.visitor_context = Drupal.personalize.visitor_context || {};
  Drupal.personalize.visitor_context.entity_context = {
    'getContext': function(enabled) {
      if (!Drupal.settings.hasOwnProperty('entity_context')) {
        return [];
      }

      var i, context_values = {};
      for (i in enabled) {
        if (enabled.hasOwnProperty(i) && Drupal.settings.entity_context.hasOwnProperty(i)) {
          context_values[i] = Drupal.settings.entity_context[i];
        }
      }
      
      return context_values;
    }
  };

})(jQuery);
