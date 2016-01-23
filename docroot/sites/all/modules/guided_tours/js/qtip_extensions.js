var qTipExtensions = {};

// Taken from http://craigsworks.com/projects/qtip2/demos/ and modified.
(function($) {
  /**
   * Shows a dialog in the center of the page.
   * @param message Message that you want to show in the dialog.
   * @param title Title of the dialog, default: Dialog
   * @param modal Wether the dialog shall be modal or not, default: true
   */
  qTipExtensions.dialog = function(message, title, modal) {
    if (message === null || message === false || message === '')
      return;

    if (title === null || title === false || title === '')
      title = Drupal.t('Dialog');

    if (modal === null || modal === '')
      modal = true;

    if (!$(message).is('button') && $(message).find('button').length === 0) {
      if (typeof message === 'object')
        message = $(message).html();

      message += '<br /><br /><button>' + Drupal.t('Okay') + '</button>';
    }

    $(document.body).qtip({
      content: {
        text: message,
        title: title
      },
      position: {
        my: 'center',
        at: 'center',
        target: $(window)
      },
      show: {
        ready: true,
        modal: {
          on: modal,
          blur: !modal
        }
      },
      hide: false,
      style: {
        classes: Drupal.settings.qTipExtensions['dialog additional classes'] + ' qtip-extensions-dialog'
      },
      events: {
        render: function(event, api) {
          if($.fn.qtip.defaults.events.render !== null && $.isFunction($.fn.qtip.defaults.events.render))
            $.fn.qtip.defaults.events.render(event, api);
          
          $('button', api.elements.content).click(api.hide);
        },
        hide: function(event, api) {
          api.destroy();
        }
      }
    });
  };
})(jQuery);
