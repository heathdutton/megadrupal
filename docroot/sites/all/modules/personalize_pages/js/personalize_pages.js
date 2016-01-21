(function ($) {

  Drupal.behaviors.personalizePages = {
    attach: function(context, settings) {
      Drupal.personalize = Drupal.personalize || {};
      Drupal.personalize.executors = Drupal.personalize.executors || {};
      Drupal.personalize.executors.personalizePages = Drupal.personalize.executors.show;
    }
  }

})(jQuery);
