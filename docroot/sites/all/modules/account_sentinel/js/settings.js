/**
 * @file
 * Provides an AJAX handler for the cron URL reset functionality.
 */

(function ($) {
  "use strict";
  Drupal.behaviors.AccountSentinelSettings = {
    attach: function (context, settings) {
      $("#cron-reset-link").click(function () {
        $.getJSON(this.getAttribute("href") + "&js=1", function (data) {
          $("#cron-link").html(data.url_cron).attr("href", data.url_cron);
          $("#cron-reset-link").attr("href", data.url_reset);
        });
        return false;
      });
    }
  };
})(jQuery);
