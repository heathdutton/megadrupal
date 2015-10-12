/**
 * @file
 * Attaches the behaviors for the Kaltura field.
 */

(function ($) {

/**
 * Open the video player.
 */
Drupal.behaviors.fieldKalturaEmbed = {
  attach: function (context, settings) {
    settings.kaltura = settings.kaltura || {};
    settings.kaltura.embedKWidget = settings.kaltura.embedKWidget || {};

    $.each(settings.kaltura.embedKWidget, function (id, vars) {
      $('#' + id, context).once('kaltura-embed', function () {
        kWidget.embed(vars);
      });
    });
  }
};

})(jQuery);
