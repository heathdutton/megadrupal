

(function ($) {


Drupal.behaviors.opignoMozillaOpenbadgesAppBehaviour = {
  attach: function (context, settings) {

    /**
     * Add the possibility to connect an account to the Mozilla Backpack Connect API
     */
    $('input[name=opigno_mozilla_openbadges_app_backpack_connect_button]').click(function() {
      OpenBadges.connect({
        callback: window.location.href,
        scope: ['issue']
      });
    });

  }
};


})(jQuery);