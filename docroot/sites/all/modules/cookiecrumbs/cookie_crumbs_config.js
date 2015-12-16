/**
 * @file
 * Configuration of the Hansels Revenge jQuery library.
 */
(function ($) {

  Drupal.behaviors.cookie_crumbs = {
    attach: function (context, settings) {
      // Attach the breadcrumbs.
      $.fn.hanselsRevenge({
        breadCrumbSelector: '.' + settings.cookie_crumbs.class_name,
        maxDepth: parseInt(settings.cookie_crumbs.items_to_show),
        debug: false,
        cookieOptions: {path: "/"},
        inheritLandingCrumbs: false
      });
    }
  };
})(jQuery);
