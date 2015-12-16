/**
 * @file
 * Display new log entries via JS.
 */

(function ($) {

  /**
   * Notify users about new log entries.
   */
  Drupal.behaviors.notifyLog = {
    attach: function (context, settings) {
      if (typeof(Drupal.settings.notifyLog) != 'undefined') {
        // Add "Close all" entry.
        $.growl({
          title: Drupal.t('Close all'),
          message: '<span class="notify-log-close-all"></span>',
          static: 1
        });

        $.each(Drupal.settings.notifyLog, function(key, value) {
          $.growl(value);
        });
        Drupal.settings.notifyLog = undefined;
      }
    }
  };

  /**
   * Close all buttom handler.
   */
  Drupal.behaviors.notifyLogCloseAll = {
    attach: function (context, settings) {
      $('div.growl-default').once().click(function () {
        if ($(this).find('div.growl-message span.notify-log-close-all')) {
          $('div.growl-close').click();
        }
      });
    }
  };

})(jQuery);
