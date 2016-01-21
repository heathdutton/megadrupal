(function ($) {
  Drupal.behaviors.DgisAmoCRMPreloader = {
    attach: function (context, settings) {
      if (Drupal.settings.DgisAmoCRM && Drupal.settings.DgisAmoCRM != undefined) {
        $('a.drupal-2gis-link').on('click', function() {
          $('body').html(Drupal.settings.DgisAmoCRM.preloader_page);
        })
      }
    }
  };
})(jQuery);
