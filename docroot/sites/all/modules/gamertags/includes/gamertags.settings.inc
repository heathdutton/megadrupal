<?php
/**
 * @file
 * Settings file for the Gamertags project.
 */

/**
 * Administrative setting callback.
 */
function gamertags_settings() {
  $form['#attached']['js'][] = drupal_get_path('module', 'gamertags') . '/misc/gamertags.settings.js';
  // Collects the enabled state for each platform.
  $form['collect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Usage'),
    '#description' => t('Collect and Display:'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#theme' => 'gamertags_settings_collect',
  );

  // Various settings for each platform.
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['settings']['gamertags_on_registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ask for Gamertags on registration screen'),
    '#description' => t("Check this if you want users to be able to input their Gamertags on the user registration page as well as from their user account pages after they've registered."),
    '#default_value' => variable_get('gamertags_on_registration', TRUE),
  );
  $form['settings']['gamertags_validate_tags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Validate Gamertags'),
    '#description' => t("Check if you want to validate user profile Gamertags.<dl><dt>Currently validates:</dt><dd>Playstation (although cannot recognise when a profile isn't activated)</dd></dl>"),
    '#default_value' => variable_get('gamertags_validate_tags', TRUE),
  );

  // Collects the URLs for the enabled platforms.
  $form['urls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gamertag URLs'),
    '#description' => t("You should never need to change these settings but if
      you do, note that the URL needs to be fully formed and it
      <strong>must</strong> contain <strong>ONE</strong> replacement
      symbol '<strong>@gamertag</strong>' which will be where the console username is
      swapped in.  See the defaults for examples of usage."
    ) . '<div id="gamertags_message"' .
    (_gamertags_test_any() ? '' : ' class="gamertags-error"') . '>' .
    (_gamertags_test_any() ? '' : t('No Gamertags are currently set to be displayed.')) . '</div>',
    '#collapsible' => TRUE,
    '#collapsed' => !_gamertags_test_any() ? FALSE : TRUE,
  );

  // Populate each section with platform data.
  foreach (gamertags_return_all() as $class => $platform_info) {
    if (method_exists($class, 'adminCollectionSettings')) {
      $form['collect'][$platform_info['name']] = call_user_func(array($class, 'adminCollectionSettings'));
    }
    if (method_exists($class, 'adminExtraSettings')) {
      $form['settings'] += call_user_func(array($class, 'adminExtraSettings'));
    }
    if (method_exists($class, 'adminUrlSettings')) {
      $form['urls'][$platform_info['name']] = call_user_func(array($class, 'adminUrlSettings'));
      $form['urls'][$platform_info['name']] += array(
        '#prefix' => '<div class="gamertags-' . $platform_info['name'] . '-wrapper"' . (variable_get("{$class}_collect", TRUE) ? '>' : ' style="display: none;">'),
        '#suffix' => '</div>',
      );
    }
  }

  // Add custom validation for the URLs.
  $form['#validate'][] = 'gamertags_settings_validate';
  return system_settings_form($form);
}

/**
 * Form validation for checking URLs contain at least one string replacement symbol.
 */
function gamertags_settings_validate(&$form, &$form_state) {
  foreach (element_children($form['urls']) as $platform) {
    foreach (element_children($form['urls'][$platform]) as $key) {
      if (!preg_match("/@gamertag/", $form_state['values'][$key])) {
        form_set_error($key, t("Error, URL must contain a string replacement symbol i.e. '@gamertag'"));
      }
      elseif (preg_match_all("/(@gamertag+)/", $form_state['values'][$key], $matches) && count($matches[0]) != 1) {
        form_set_error($key, t("Error, URL must contain only ONE replacement symbol i.e. '@gamertag'"));
      }
    }
  }
}

/**
 * Themes output for the collect and display settings.
 */
function theme_gamertags_settings_collect($variables) {
  $form = $variables['form'];
  $rows = array();
  foreach (element_children($form) as $key) {
    $row = array();
    $row[] = drupal_render($form[$key]['checkbox']);
    $row[] = drupal_render($form[$key]['weight']);
    $rows[] = array('data' => $row);
  }
  $header = array(t('Gamertag'), t('Weight'));
  return '<div id="gamertags-usage-wrapper">' . theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No Gamertags available.'), 'attributes' => array('id' => 'gamertags-usage-table'))) . '</div>' . drupal_render_children($form);
}
