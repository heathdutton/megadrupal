(function ($) {

/**
 * Drupal.Nodejs.callback on rules message.
 */
Drupal.Nodejs.callbacks.nodejsAction = {
  callback: function (message) {
    var uniqid            = message.uniqid,
        rulesNodejsAction = Drupal.rulesNodejsAction,
        uniqueIDsArray    = rulesNodejsAction.properties.uniqueIDsArray,
        currentUid        = Drupal.settings.rulesNodejsAction.user,
        authorUid         = message.messageAuthorUid,
        alertsClass       = rulesNodejsAction.properties.alertsClass,
        alertsWrapClass   = rulesNodejsAction.properties.alertsWrapClass,
        $rulesMessage     = $(message.markup),
        $rulesMesBody     = $rulesMessage.find('.body');

    // Add wrapper for rules messages.
    if (!$('body').find('.' + alertsWrapClass).length) {
      $('body').append('<div class="' + alertsWrapClass + '" />')
    }

    // Add alert message if it's unique.
    if (uniqueIDsArray.indexOf(uniqid) < 0 && (currentUid !== authorUid)) {
      // Add message to the alers wrapper.
      $('.' + alertsWrapClass).prepend($rulesMessage);

      if ($rulesMesBody.height() > 120) {
        // Create jQuery object for show more link.
        $showMore = $('<div>', {
          'class': 'rnj-showmore',
          'text': Drupal.t('Show more...')
        }).bind('click', function () {
          rulesNodejsAction.showMore($(message.markup).html());
          rulesNodejsAction.removeAlert($rulesMessage, 1, 200);
          $(this).unbind('click');
        });

        $rulesMesBody.height(100);
        $rulesMesBody.after($showMore)
      }

      // Add unique message id to the array.
      uniqueIDsArray.push(uniqid);

      // Remove old messages if nen have top offset < 0.
      if ($rulesMessage.offset().top < 0) {
        rulesNodejsAction.removeAlert($('.' + alertsWrapClass).find('.' + alertsClass).not(':eq(0)').last(), 1, 100);
      }
    }

    // Remove alert after 15 seconds.
    rulesNodejsAction.removeAlert($rulesMessage, 15000, 500);

    // Remove message close button click.
    $rulesMessage.find('.close-button').bind('click', function (e) {
      rulesNodejsAction.removeAlert($(this).parent(), 1, 500);
      e.stopPropagation();
    });
  }
};

/**
 * Provide functions for rules messages manipulations, etc.
 */
Drupal.rulesNodejsAction = {
  // Properties.
  properties: {
    uniqueIDsArray: [],
    alertsClass: 'rules-message',
    alertsWrapClass: 'rules-messages-wrapper'
  },

  /**
   * Remove alert.
   *
   * @param $alert
   *   jQuery object of alert message.
   * @param timeout
   *   Timeout to message show.
   * @param speed
   *   Animation speed.
   */
  removeAlert: function ($alert, timeout, speed) {
    timeout = timeout || 5000;
    speed = speed || 500;
    setTimeout(function () {
      $alert.animate({
        'opacity': 0.1
      }, speed, function () {
        $(this).remove();
      });
    }, timeout);
  },

  /**
   * Function for show rules message in popup.
   *
   * @param $html
   *   jQuery object of message html.
   */
  showMore: function ($html) {
    var documentH = $(document).height(),
        windowH = $(window).height();

    if (documentH < windowH) {
      documentH = windowH;
    }

    // Create jQuery object for rules message popup overlay.
    $rulesMessagePopupOverlay = $('<div>', {
      'class': 'rules-message-popup-overlay'
    }).css({
      'height': documentH
    });

    // Create jQuery object for rules message popup.
    $rulesMessagePopup = $('<div>', {
      'class': 'rules-message-popup',
      'html': $html
    }).css({
      'top': $('body').scrollTop() + 100
    });

    // Bind popup close.
    $rulesMessagePopup.find('.close-button').bind('click', function () {
      Drupal.rulesNodejsAction.removeAlert($('.rules-message-popup, .rules-message-popup-overlay'), 1, 500);
    });

    // Bind popup close on ESC.
    $(window).keyup(function(e){
      if (e.which == 27) {
        Drupal.rulesNodejsAction.removeAlert($('.rules-message-popup, .rules-message-popup-overlay'), 1, 500);
      }
    });

    // Remove old poup and overlay.
    $('.rules-message-popup, .rules-message-popup-overlay').remove();

    // Add popup and popup overlay to body end.
    $('body').append($rulesMessagePopup, $rulesMessagePopupOverlay);
  }
};

}(jQuery));
