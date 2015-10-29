(function ($) {

/**
 * Custom summary for samara theme setings vertical tabs.
 */
Drupal.behaviors.vertical_tabs_samara = {
  attach: function (context) {

    // Default settings.
    $('fieldset#edit-theme-settings', context).drupalSetSummary(function (context) {
      var default_settings = {
        toggle_logo: Drupal.t('Logo'),
        toggle_name: Drupal.t('Site name'),
        toggle_slogan: Drupal.t('Site slogan'),
        toggle_node_user_picture: Drupal.t('User pictures in posts'),
        toggle_comment_user_picture: Drupal.t('User pictures in comments'),
        toggle_comment_user_verification: Drupal.t('User verification status in comments'),
        toggle_favicon: Drupal.t('Shortcut icon'),
        toggle_main_menu: Drupal.t('Main menu'),
        toggle_secondary_menu: Drupal.t('Secondary menu'),
      };
      var output = [];
      var i = 0;
      $('fieldset#edit-theme-settings input[type=checkbox]')
        .each(function(key, value) {
          if ($(value).attr('checked')) {
            output[i++] = default_settings[$(value).attr('name')];
          }
        })
      return output.join(', ');
    });

    // Common settings.
    $('fieldset#edit-common', context).drupalSetSummary(function (context) {

      var position_options = {
        '-2':Drupal.t('Far left'),
        '-1':Drupal.t('Left'),
         '1':Drupal.t('Right'),
         '2':Drupal.t('Far right'),
      };

      var color_scheme_options = {
        'default':Drupal.t('default'),
        'dark':Drupal.t('Dark'),
      };

      return [
        Drupal.t('Color scheme') + ': ' + color_scheme_options[$('#edit-color-scheme').val()],
        Drupal.t('Base font size') + ': ' + $('#edit-base-font-size').val(),
        Drupal.t('First sidebar position') + ': ' + position_options[$('#edit-sidebar-first-weight').val()],
        Drupal.t('Second sidebar position') + ': ' + position_options[$('#edit-sidebar-second-weight').val()],
      ].join('<br/>');

    });

    // Layout settings.
    for (var i = 1; i <= 3; i++) {
      $('fieldset#edit-layout-' + i, context).drupalSetSummary(function (context) {
        var prefix = '#edit-layout-' + $(context).attr('id').split('-')[2];
        return [
          $(prefix + '-min-width').val(),
          $(prefix + '-width').val(),
          $(prefix + '-max-width').val(),
        ].join(' < ');
      });
    }

  }
};

})(jQuery);
