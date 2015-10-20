/**
 * @file
 *
 * @author Dennis Brücke (blackice2999) | TWD - team:Werbedesign UG
 *   @see http://drupal.org/user/413429
 *   @see http://team-wd.de
 */

(function ($) {
  Drupal.behaviors.bootstrap_api_alert = {
    attach: function (context) {
      $('.alert', context).once('bootstrap-api-alert', function () {
        $(this).alert();
      });
    }
  };
})(jQuery);
