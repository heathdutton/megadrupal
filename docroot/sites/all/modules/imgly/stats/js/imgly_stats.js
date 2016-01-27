/* Imgly Statistics javascript events */

(function ($) {
  "use strict";

  Drupal.behaviors.imgly_stats = {
    attach: function(context, settings) {
      var data = 'nid=' + settings.imgly.nid + '&token=' + settings.imgly.token;

      // Log a view event and get total number of views and downloads.
      $.post('/js/imgly/view', data, function(response) {
        if (response && response.ok === true && response.stats) {
          var stats = '';

          for (var i in response.stats) {
            stats += '<div class="imgly-stats-' + i + '">' + response.stats[i].label + ' <span>' + response.stats[i].count + '</span></div>';
          }

          $('.imgly-stats', context).append(stats);
        }
      }, 'json');
    }
  };
}(jQuery));