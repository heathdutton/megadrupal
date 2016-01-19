/**
 * @file
 * Attaches the behaviors for the Shared Content server.
 *
 * This js is targeted to be added in an overlay response.
 */

(function ($) {

  /**
   * Trigger an overlay request for all local sites.
   *
   * Take care that the user stays in overlay mode as long as a link local link
   * is uses. As soon as a external link is triggered, it will be opened in the
   * top window.
   */
  Drupal.behaviors.SharedContentUrlAlter = {
    attach: function(context) {
      var origin = new RegExp(document.location.origin, 'i');
      $('a[href]').each(function (index, link) {
        if (origin.test(link.href)) {
          $(link).click(function () {
            var glue = /\?/i.test(link.href) ? '&' : '?';
            window.location = link.href + glue + 'sc[overlay]=true';
            return false;
          });
        }
        else {
          $(link).click(function () {
            window.top.location = link.href;
            return false;
          });
        }
      });
    }
  };

})(jQuery);
