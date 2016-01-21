/**
 * @file
 * This will pass the settings and start the Galleria.
 */
(function($) {
Drupal.behaviors.viewsJqfxGalleria = {
  attach: function (context) {
    for (id in Drupal.settings.viewsJqfxGalleria) {
      $('#' + id + ':not(.viewsJqfxGalleria-processed)', context).addClass('viewsJqfxGalleria-processed').each(function () {
        var _settings = Drupal.settings.viewsJqfxGalleria[$(this).attr('id')];

        // Eval setting that are functions
        if (_settings['dataConfig']) {
          var galDataSource = _settings['dataConfig'];
          eval("_settings['dataConfig'] = " + galDataConfig);
        }
        if (_settings['extend']) {
          var galExtend = _settings['extend'];
          eval("_settings['extend'] = " + galExtend);
        }

        // Fire up the Galleria.
        $(this).galleria(_settings);
      });
    }
  }
}

})(jQuery);
