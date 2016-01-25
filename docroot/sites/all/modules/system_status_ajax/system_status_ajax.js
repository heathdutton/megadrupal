(function ($) {

/**
 * Triggers the AJAX call that checks system status.
 */
Drupal.behaviors.systemStatusAjax = {
  attach: function (context, settings) {
    $('.system-status-ajax-messages:not(.ajax-processed)').addClass('ajax-processed').each(function () {
      Drupal.systemStatusAjax.checkStatus(this, settings);
    });
  }
};

Drupal.systemStatusAjax = {

  /**
   * Executes AJAX commands for the status messages.
   */
  checkStatus: function(messageWrapper, settings) {
    $(messageWrapper).css({visibility: "visible"});
    $(messageWrapper).children('span.system-status-ajax-message').each(function () {

      // Get the "type" from the class, bail if we don't have a match.
      var match = /system-status-ajax-message-([a-z]+)/g.exec($(this).attr('class'));
      if (match == null) {
        return false;
      }

      // Build element settings.
      var elementSettings = {};
      elementSettings.progress = { 'type': 'throbber' };
      elementSettings.url = '/admin/reports/' + match[1] + '/ajax';
      elementSettings.event = 'SystemStatusAjax';

      // Instantiate the AJAX object and trigger the event.
      var base = $(this).attr('id');
      var ajax = new Drupal.ajax(base, this, elementSettings);
      $(ajax.element).trigger('SystemStatusAjax');
    });
  }
}

/**
 * Custom AJAX command that sets the message based on the status.
 */
$(function() {
  Drupal.ajax.prototype.commands.systemStatusAjax = function (ajax, response, status) {

    // Get the message span and div respectively.
    var message = ajax.element;
    var messageWrapper = $(message).parent('.system-status-ajax-messages');

    // Reset the status class and message text.
    if (response.data.status) {
      if (response.data.status != 'warning') {
        $(messageWrapper).removeClass('warning').addClass(response.data.status);
      }
      $(message).html(response.data.message);
    }
  };
});

})(jQuery);
