/**
 *  @file
 *  This will pass the settings and initiate the Supersized.
 */
(function($) {

  Drupal.behaviors.viewsJqfxsupersized = {
    attach: function (context) {
      for (id in Drupal.settings.viewsJqfxsupersized) {
        $('#' + id + ':not(.viewsJqfxsupersized-processed)', context).addClass('viewsJqfxsupersized-processed').each(function () {
          var _settings = Drupal.settings.viewsJqfxsupersized[$(this).attr('id')];
          var superS = $(this);
        });
      }
    }
  }

})(jQuery);
