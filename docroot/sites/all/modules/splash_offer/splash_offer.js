/**
 * @file
 * The main javascript file for the splash_offer module
 *
 * @ingroup splash_offer
 * @{
 */

(function ($) {

  Drupal.splashOffer = Drupal.splashOffer || {};

  /**
   * Set cookies as needed and defined by settings of this offer
   */
  Drupal.splashOffer.cookieHandler = function(c_value) {
    var c_name = 'splash_offer:' + Drupal.settings.splashOffer.id;
    // Set a cookie if user asks us to
    if (Drupal.settings.splashOffer.hasOwnProperty('cookie') && $('#splash-offer-set-cookie:checked').length) {
      if ( Drupal.settings.splashOffer.cookie.expiry > 0 ) {
        // This cookie will expire after n days
        $.cookie(c_name, c_value, {
          expires: parseInt(Drupal.settings.splashOffer.cookie.expiry, 10),
          path: '/'
        });
      }
      else {
        // This is a session cookie
        $.cookie(c_name, c_value, {
          path: '/'
        });
      }
    }
    // Make sure we remove any cookies if unchecked so user will be prompted
    // again.
    if (!$('#splash-offer-set-cookie:checked').length) {
      $.cookie(c_name, null);
    }
  };

  /**
   * Show the splash offer modal
   */
  Drupal.splashOffer.show = function() {
    var options = {
      dialogClass: 'splash-offer-dialog splash-offer-dialog-' + Drupal.settings.splashOffer.id,
      height: $('.entity-splash-offer').height(),
      width: $('.entity-splash-offer').width(),
      autoOpen: true,
      modal: true,
      draggable: false,
      resizable: false
    };
    if (Drupal.settings.splashOffer.hasOwnProperty('yes')) {
      $.extend(options, {
      buttons: [
        {
          text: Drupal.settings.splashOffer.yes.text,
          click: function() {
            Drupal.splashOffer.cookieHandler('yes');
            Drupal.splashOffer.hide();
            return Drupal.splashOffer.redirect('yes');
          }
        },
        {
          text: Drupal.settings.splashOffer.no.text,
          click: function() {
            Drupal.splashOffer.cookieHandler('no');
            Drupal.splashOffer.hide();
            return false;
          }
        }
      ]
      });
    }
    $('.entity-splash-offer').dialog(options);
  };

  /**
   * Hide the splash offer modal
   */
  Drupal.splashOffer.hide = function() {
    $('.entity-splash-offer').dialog('close');
  };

  /**
   * Redirect based on button click
   */
  Drupal.splashOffer.redirect = function(buttonId) {
    if (typeof Drupal.settings.splashOffer[buttonId].path !== 'undefined') {
      window.open(Drupal.settings.splashOffer[buttonId].path);
      return false;
    }
    return true;
  };

  /**
  * Core behavior for splash_offer.
  */
  Drupal.behaviors.splashOffer = Drupal.behaviors.splashOffer || {};
  Drupal.behaviors.splashOffer.attach = function (context, settings) {

    /**
     * Admin Entity form
     */
    $('.form-item-data-devices-always-trigger .form-checkbox').click(function() {
      // Show the devices when 'always trigger' is disabled
      var expand = !$(this).is(':checked');
      $(this).parents('.fieldset-wrapper').find('fieldset').each(function() {
        if ((expand && $(this).hasClass('collapsed')) || (!expand && !$(this).hasClass('collapsed'))) {
          $(this).find('a.fieldset-title').click();
        }
      });
    });

    /**
     * Displaying a splash offer
     *
     * We are displaying a splash offer if we have an oid
     */
    if (typeof Drupal.settings.splashOffer !== 'undefined' && Drupal.settings.splashOffer.hasOwnProperty('id')) {

      // Don't do anything if cookies are enabled and the cookie is already set.
      var c_name = 'splash_offer:' + Drupal.settings.splashOffer.id;
      if (Drupal.settings.splashOffer.hasOwnProperty('cookie') && $.cookie(c_name)) {
        return;
      }

      Drupal.splashOffer.show();
      $('.splash-offer-no, .ui-dialog-titlebar-close').click(function(){
        Drupal.splashOffer.cookieHandler('no');
        Drupal.splashOffer.hide();
        return false;
      });

      $('.splash-offer-yes').click(function(){
        Drupal.splashOffer.cookieHandler('yes');
        Drupal.splashOffer.hide();
        return Drupal.splashOffer.redirect('yes');
      });
    }

    $(window).resize(function() {
        $('.entity-splash-offer').dialog("option", "position", "center");
    });

  };

  /**
  * @} End of "defgroup splash_offer".
  */

})(jQuery);
