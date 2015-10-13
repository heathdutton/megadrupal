<?php
/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function kands_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL)  {
  // Work-around for a core bug affecting admin themes. See issue #943212.
  if (isset($form_id)) {
    return;
  }

  // Create the form using Forms API: http://api.drupal.org/api/7
  
  // Custom splash image
    global $key;
    $form['splash'] = array(
      '#type' => 'fieldset',
      '#title' => t('Splash image settings'),
      '#description' => t('If toggled on, the following splash image will be displayed.'),
      '#attributes' => array('class' => array('theme-settings-bottom')),
    );
    $form['splash']['default_splash'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the default splash image'),
      '#default_value' => theme_get_setting('default_splash', $key),
      '#tree' => FALSE,
      '#description' => t('Check here if you want the theme to use the splash image supplied with the theme (or alter using css).')
    );
    $form['splash']['settings'] = array(
      '#type' => 'container',
      '#states' => array(
        // Hide the splash settings when using the default splash.
        'invisible' => array(
          'input[name="default_splash"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['splash']['settings']['splash_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to custom splash'),
      '#description' => t('The path to the file you would like to use as your splash image instead of the default.'),
      '#default_value' => theme_get_setting('splash_path', $key),
    );
  
  // Support for external webfont providers javascript
  $form['webfont'] = array(
  	'#type'			 =>	'fieldset',
  	'#title'		 =>	t('Webfont Settings'),
  );
  $form['webfont']['kands_webfont_css'] = array(
    '#type'          => 'textfield',
    '#title'         => t('CSS url'),
    '#default_value' => theme_get_setting('kands_webfont_css'),
    '#description'   => t('Use this field to easily link to an external css, eg. from <a href="http://google.com/webfonts" target="_blank">Google Webfonts</a>.'),
  );
  $form['webfont']['kands_webfont_js'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Javascript url'),
    '#default_value' => theme_get_setting('kands_webfont_js'),
    '#description'   => t('Use this field to easily link to an externally hosted javascript, eg. from <a href="http://google.com/webfonts" target="_blank">Google Webfonts</a> or <a href="http://webfonts.fonts.com" target="_blank">Fonts.com</a>.'),
  );
  
  // $form['#validate'][] = 'kands_theme_settings_validate';
  // $form['#submit'][] = 'kands_theme_settings_submit';
  // */

  // Remove some of the base theme's settings.
  /* -- Delete this line if you want to turn off this setting.
  unset($form['themedev']['zen_wireframes']); // We don't need to toggle wireframes on this site.
  // */

  // We are editing the $form in place, so we don't need to return anything.
}
