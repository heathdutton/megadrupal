(function ($) {
  Drupal.behaviors.cpsProcessStatus = {
    attach: function(context, settings) {
      // Hide the action links while the changeset is being published, since
      // they'll do nothing, and also don't reflect the publishing state.
      $('.action-links').hide();
      $('#cps-process-placeholder iframe').hide();
      var status_url = settings.cps_process_status_url;
      Drupal.behaviors.cpsProcessStatus.processJson(status_url);
    },
    processJson: function(status_url) {
      $.getJSON(status_url + '?timestamp=' + Date.now()).done(function(data) {
        if (data != null) {
          $('#cps-process-placeholder span.cps-process').html(data.message);
          if (data.complete !== 1 && data.complete !== -1) {
            setTimeout(function() {
              Drupal.behaviors.cpsProcessStatus.processJson(status_url);
            }, 200);
          }
          if (data.complete === 1) {
            window.location.href = window.location.href.split('?')[0];
          }
        }
        else {
          setTimeout(Drupal.behaviors.cpsProcessStatus.processJson(status_url), 1000);
        }
      });
    }
  };
})(jQuery);
