/**
 * @file
 * JavaScript used for settings 'tablet' css class for page <body> for anonymous users.
 */

(function($){

  // Add an extra function to the Drupal ajax object which allows us to trigger
  // an ajax response without an element that triggers it.
  Drupal.ajax.prototype.specifiedResponse = function() {
    var ajax = this;

    // Do not perform another ajax command if one is already in progress.
    if (ajax.ajaxing) {
      return false;
    }

    try {
      $.ajax(ajax.options);
    }
    catch (err) {
      alert('An error occurred while attempting to process ' + ajax.options.url);
      return false;
    }

    return false;
  };

  $(document).ready(function() {
    // Define a custom ajax action not associated with an element.
    var custom_settings = {
      url: Drupal.settings.basePath + 'ai_mobile/ajax',
      event: 'onload',
      keypress: false,
      prevent: false,
      progress: {
        type: 'none',
        message: false
      }
    };

    Drupal.ajax['ai_mobile_ajax_action'] = new Drupal.ajax(null, $(document.body), custom_settings);

    // Trigger custom action.
    Drupal.ajax['ai_mobile_ajax_action'].specifiedResponse();
  });

})(jQuery);
