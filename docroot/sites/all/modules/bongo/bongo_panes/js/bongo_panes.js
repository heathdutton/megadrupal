/**
 * @file
 * Javascript for the Bongo panes module.
 */

 // Namespace jQuery to avoid conflicts.
(function($) {
  // Define the behavior.
  Drupal.bongoPredictions = function() {
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

    // Call updatePredictions every 30 seconds.
    setInterval(Drupal.bongoPredictions.updatePredictions, 30000);
  };

  // Attach bongoPredictions behavior.
  Drupal.behaviors.bongoPredictions = {
    attach: function(context, settings) {
      $('.pane-bongo-predictions', context).once('bongoPredictions', function() {
        Drupal.bongoPredictions();
      });
    }
  };

  // Update all predictions on the page.
  Drupal.bongoPredictions.updatePredictions = function() {
    var predictions = Drupal.settings.bongoPanes;
    for (i = 0; i < predictions.length; i++) {
      prediction = predictions[i];
      stop = prediction.stop;
      agency = prediction.agency;
      show = prediction.predictions_to_show;
      labels = prediction.prediction_labels;
      direction = prediction.direction;

      // Define a custom ajax action not associated with an element.
      var custom_settings = {};
      custom_settings.url = Drupal.settings.basePath + 'bongo_panes_ajax_link_callback/ajax/' + stop + '/' + show + '/' + agency + '/' + labels + '/' + direction + '/';
      custom_settings.event = 'custom';
      custom_settings.keypress = false;
      custom_settings.prevent = false;
      custom_settings.progress = {'type': 'none'};
      Drupal.ajax['bongo_predictions_update'] = new Drupal.ajax(null, $(document.body), custom_settings);

      // Trigger the response.
      Drupal.ajax['bongo_predictions_update'].specifiedResponse();
    }
  };

})(jQuery);
