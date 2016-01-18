<?php

/**
 * @file
 * Admin page callback file for the wow module.
 */

/**
 * Form builder; The API settings form.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function wow_services_authentication_form($form, &$form_state) {
  $private_key = variable_get('wow_key', '');
  $public_key = variable_get('wow_public_key', '');

  $form['authentication'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Authentication'),
    '#description' => t("To send authenticated request you first need to register an application. Because registration isn't automated, application registration is limited to those who meet the following criteria:")
      . '<ul><li>' . t('You plan on making requests from one or more IP addresses (e.g. a production environment and development environment).') . '</li>'
      . '<li>' . t('You can justify making more than 2,000 requests per day from one or more IP addresses.') . '</li></ul>',
    '#collapsible' => !empty($private_key),
    '#collapsed' => !empty($private_key),
  );

  $ssl = extension_loaded('openssl');
  if (!$ssl) {
    $form['authentication']['#description'] .= '<strong class="error">' . t('Your PHP installation does not have SSL support enabled. Authentication has been disabled.') . '</strong>';
  }
  $form['authentication']['wow_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private Key'),
    '#attributes' => array('autocomplete' => 'off'),
    '#default_value' => $private_key,
    '#disabled' => !$ssl
  );
  $form['authentication']['wow_public_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Public Key'),
    '#default_value' => $public_key,
    '#disabled' => !$ssl
  );

  return system_settings_form($form);
}

/**
 * Returns settings form for WoWRemoteEntity class based.
 *
 * @param string $entity_type
 */
function wow_entity_settings($entity_type) {
  $entity_info = entity_get_info($entity_type);
  $cron_safe_threshold = variable_get('cron_safe_threshold', DRUPAL_CRON_DEFAULT_THRESHOLD);

  $t_label = array('!label' => $entity_info['label']);

  $form = array(
    '#type' => 'fieldset',
    '#title' => t('!label entity settings', $t_label),
  );
  $form[$entity_type . '_refresh_method'] = array(
    '#type' => 'radios',
    '#title' => t('!label refresh method', $t_label),
    '#default_value' => variable_get($entity_type . '_refresh_method', WOW_REFRESH_CRON),
    '#options' => array(
      WOW_REFRESH_LOAD => t('At entity load'),
      WOW_REFRESH_CRON => t('Periodically through cron'),
      WOW_REFRESH_EXPLICITLY => t('Explicitly')
    ),
    '#description' => t('The method used to keep updated the entity from the remote service. The first method with ask the service each time an entity is loaded, the second will periodically run in background asking the services for changes, whereas the third one will only call the service on demand (e.g. by code or by a user request). Each of these methods respect If-Modified-Since header.')
  );
  $form[$entity_type . '_refresh_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('!label refresh threshold', $t_label),
    '#size' => 15,
    '#default_value' => variable_get($entity_type . '_refresh_threshold', $cron_safe_threshold),
    '#description' => t('The threshold used to make a remote service call, in seconds.')
  );

  if ($form[$entity_type . '_refresh_threshold']['#default_value'] < $cron_safe_threshold) {
    $form[$entity_type . '_refresh_threshold']['#description'] .= '<br/><span class="warning">' . t('The cron safe threshold value is higher than the !label refresh threshold. You can safely ignore this warning message if you manage externally your cron tasks.', $t_label) . '</span>';
  }

  return $form;
}

/**
 * Return the wow regions supported by the API.
 */
function wow_region_options() {
  $regions = array();

  foreach (wow_api_info() as $region => $info) {
    $regions[$region] = t($info->name);
  }

  return $regions;
}

/**
 * Menu callback; Displays wow admin dashboard.
 */
function wow_admin() {
  $services = wow_service_list();

  foreach ($services as $langcode => $service) {
/*
    if ($service->expires) {
      foreach ($service->expires as $module => $data) {
        foreach ($data as $key => $timestamp) {

        }
      }
    }
*/
  }
  return '';
}

/**
 * Menu callback; Displays wow service addition screen.
 */
function wow_services_add_screen() {
  $build['predefined'] = drupal_get_form('wow_services_predefined_form');
  $build['custom'] = drupal_get_form('wow_services_custom_form');
  return $build;
}

/**
 * Predefined services setup form.
 */
function wow_services_predefined_form($form) {
  include_once 'wow.inc';
  $predefined = _wow_prepare_predefined_list();
  $form['service list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Predefined service'),
    '#collapsible' => TRUE,
  );
  $form['service list']['langcode'] = array(
    '#type' => 'select',
    '#title' => t('Service name'),
    '#default_value' => key($predefined),
    '#options' => $predefined,
    '#description' => t('Use the <em>Custom service</em> section below if your desired service does not appear in this list.'),
  );
  $form['service list']['actions'] = array('#type' => 'actions');
  $form['service list']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add service')
  );
  return $form;
}

/**
 * Custom service addition form.
 */
function wow_services_custom_form($form) {
  $form['custom service'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom service'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  _wow_services_common_form($form['custom service']);
  $form['custom service']['actions'] = array('#type' => 'actions');
  $form['custom service']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add custom service')
  );
  // Reuse the validation and submit functions of the predefined service setup form.
  $form['#submit'][] = 'wow_services_predefined_form_submit';
  $form['#validate'][] = 'wow_services_predefined_form_validate';
  return $form;
}

/**
 * Editing screen for a particular service.
 *
 * @param $langcode
 *   Language code of the service to edit.
 */
function wow_services_edit_form($form, &$form_state, $langcode) {
  if ($service = db_query("SELECT * FROM {wow_services} WHERE language = :language", array(':language' => $langcode))->fetchObject()) {
    _wow_services_common_form($form, $service);
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save service')
    );
    $form['#submit'][] = 'wow_services_edit_form_submit';
    $form['#validate'][] = 'wow_services_edit_form_validate';
    return $form;
  }
  else {
    drupal_not_found();
    drupal_exit();
  }
}

/**
 * Common elements of the service addition and editing form.
 *
 * @param $form
 *   A parent form item (or empty array) to add items below.
 * @param $service
 *   Service object to edit.
 */
function _wow_services_common_form(&$form, $service = NULL) {
  if (!is_object($service)) {
    $service = new stdClass();
  }
  if (isset($service->language)) {
    $form['langcode_view'] = array(
      '#type' => 'item',
      '#title' => t('Language code'),
      '#markup' => $service->language
    );
    $form['langcode'] = array(
      '#type' => 'value',
      '#value' => $service->language
    );
  }
  else {
    $form['langcode'] = array(
      '#type' => 'textfield',
      '#title' => t('Language code'),
      '#maxlength' => 12,
      '#required' => TRUE,
      '#default_value' => @$service->language,
      '#disabled' => isset($service->language),
      '#description' => t('<a href="@rfc4646">RFC 4646</a> compliant language identifier. Language codes typically use a country code, and optionally, a script or regional variant name. <em>Examples: "en", "en-US" and "zh-Hant".</em>', array('@rfc4646' => 'http://www.ietf.org/rfc/rfc4646.txt')),
    );
  }
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Service name in English'),
    '#maxlength' => 64,
    '#default_value' => @$service->name,
    '#required' => TRUE,
    '#description' => t('Name of the service in English. Will be available for translation in all languages.'),
  );
  $form['native'] = array(
    '#type' => 'textfield',
    '#title' => t('Native service name'),
    '#maxlength' => 64,
    '#default_value' => @$service->native,
    '#required' => TRUE,
    '#description' => t('Name of the service in the language being added.'),
  );
  $form['region'] = array(
    '#type' => 'select',
    '#title' => t('Service region'),
    '#options' => wow_region_options(),
    '#default_value' => @$service->region,
    '#required' => TRUE,
    '#description' => t('Region of the service. Will be used to determine the host API to fetch information from.'),
  );
  $form['locale'] = array(
    '#type' => 'textfield',
    '#title' => t('Service locale'),
    '#maxlength' => 12,
    '#required' => TRUE,
    '#default_value' => @$service->locale,
    '#description' => t('Locale of the service.  The locales supported vary from region to region and align with those supported on the community sites.'),
  );
}

/**
 * Validate the service addition form.
 */
function wow_services_predefined_form_validate($form, &$form_state) {
  $langcode = $form_state['values']['langcode'];
  $predefined = $form['service list']['langcode']['#options'];

  if (($duplicate = db_query("SELECT COUNT(*) FROM {wow_services} WHERE language = :language", array(':language' => $langcode))->fetchField()) != 0) {
    form_set_error('langcode', t('The service %language (%code) already exists.', array('%language' => $predefined[$langcode], '%code' => $langcode)));
  }

  if (!array_key_exists($langcode, language_list('language'))) {
    // TODO: propose to install a language.
    form_set_error('langcode', t('The language %language (%code) is not installed.', array('%language' => $predefined[$langcode], '%code' => $langcode)));
  }

  if (!isset($form_state['values']['name'])) {
    // Predefined language selection.
    require_once 'wow.inc';
    $predefined = _wow_get_predefined_list();
    if (!isset($predefined[$langcode])) {
      form_set_error('langcode', t('Invalid language code.'));
    }
  }
  else {
    // Reuse the editing form validation routine if we add a custom service.
    wow_services_edit_form_validate($form, $form_state);
  }
}

/**
 * Process the service addition form submission.
 */
function wow_services_predefined_form_submit($form, &$form_state) {
  require_once 'wow.inc';
  $langcode = $form_state['values']['langcode'];

  if (isset($form_state['values']['name'])) {
    // Custom service form.
    wow_add_service($langcode, $form_state['values']['region'], $form_state['values']['locale'], $form_state['values']['name']);
    drupal_set_message(t('The service %name has been created and can now be used. More information is available on the <a href="@wow-help">help screen</a>.', array('%name' => t($form_state['values']['name']), '@wow-help' => url('admin/help/wow'))));
  }
  else {
    // Predefined service selection.
    $predefined = _wow_get_predefined_list();
    wow_add_service($langcode);
    drupal_set_message(t('The service %name has been created and can now be used. More information is available on the <a href="@wow-help">help screen</a>.', array('%name' => t($predefined[$langcode][1]), '@wow-help' => url('admin/help/wow'))));
  }

  // See if we have service data to import for the newly added service,
  // collect and import them.
  if ($batch = wow_batch_by_service($langcode, '_wow_batch_service_finished')) {
    batch_set($batch);
  }

  $form_state['redirect'] = 'admin/config/wow/service';
}

/**
 * Validate the service editing form. Reused for custom service addition too.
 */
function wow_services_edit_form_validate($form, &$form_state) {
  // Ensure sane field values for langcode and name.
  if (!isset($form['langcode_view']) && preg_match('@[^a-zA-Z_-]@', $form_state['values']['langcode'])) {
    form_set_error('langcode', t('%field may only contain characters a-z, underscores, or hyphens.', array('%field' => $form['langcode']['#title'])));
  }
  if ($form_state['values']['name'] != check_plain($form_state['values']['name'])) {
    form_set_error('name', t('%field cannot contain any markup.', array('%field' => $form['name']['#title'])));
  }
  if ($form_state['values']['native'] != check_plain($form_state['values']['native'])) {
    form_set_error('native', t('%field cannot contain any markup.', array('%field' => $form['native']['#title'])));
  }
}

/**
 * Process the language editing form submission.
 */
function wow_services_edit_form_submit($form, &$form_state) {
  db_update('wow_services')
    ->fields(array(
      'name' => $form_state['values']['name'],
      'native' => $form_state['values']['native'],
      'region' => $form_state['values']['region'],
      'locale' => $form_state['values']['locale'],
    ))
    ->condition('language', $form_state['values']['langcode'])
    ->execute();
  $default = wow_service_default();
  if ($default->language == $form_state['values']['langcode']) {
    $properties = array('name', 'native', 'region', 'locale', 'enabled', 'weight');
    foreach ($properties as $keyname) {
      if (isset($form_state['values'][$keyname])) {
        $default->$keyname = $form_state['values'][$keyname];
      }
    }
    variable_set('wow_service_default', $default);
  }
  $form_state['redirect'] = 'admin/config/wow/service';
  return;
}

/**
 * User interface for the service deletion confirmation screen.
 */
function wow_services_delete_form($form, &$form_state, $langcode) {

  // Do not allow deletion of English service.
  if ($langcode == 'en') {
    drupal_set_message(t('The English service cannot be deleted.'));
    drupal_goto('admin/config/wow/service');
  }

  if (wow_service_default('language') == $langcode) {
    drupal_set_message(t('The default service cannot be deleted.'));
    drupal_goto('admin/config/wow/service');
  }

  // For other services, warn user that data loss is ahead.
  $services = wow_service_list();

  if (!isset($services[$langcode])) {
    drupal_not_found();
    drupal_exit();
  }
  else {
    $form['langcode'] = array('#type' => 'value', '#value' => $langcode);
    return confirm_form($form, t('Are you sure you want to delete the service %name?', array('%name' => t($services[$langcode]->name))), 'admin/config/wow/service', t('Deleting a service will remove all interface translations associated with it. This action cannot be undone.'), t('Delete'), t('Cancel'));
  }
}

/**
 * Process service deletion submissions.
 */
function wow_services_delete_form_submit($form, &$form_state) {
  $services = wow_service_list();
  if (isset($services[$form_state['values']['langcode']])) {
    // Remove the service.
    db_delete('wow_services')
      ->condition('language', $form_state['values']['langcode'])
      ->execute();

    $variables = array('%locale' => $services[$form_state['values']['langcode']]->name);
    drupal_set_message(t('The service %locale has been removed.', $variables));
    watchdog('wow', 'The service %locale has been removed.', $variables);
  }

  // Changing the language settings impacts the interface:
  cache_clear_all('*', 'cache_page', TRUE);

  $form_state['redirect'] = 'admin/config/wow/service';
  return;
}

/**
 * @defgroup wow-api-add Service addition API
 * @{
 * Add a service.
 *
 * The service addition API is used to create services and store them.
 */

/**
 * API function to add a service.
 *
 * @param $langcode
 *   Language code.
 * @param $region
 *   Region of the service.
 * @param $locale
 *   Locale of the service.
 * @param $name
 *   Name of the service.
 */
function wow_add_service($langcode, $region = NULL, $locale = NULL, $name = NULL) {
  // Default locale on language code.
  if (empty($locale)) {
    $locale = $langcode;
  }

  // If name was not set, we add a predefined service.
  if (!isset($name)) {
    require_once 'wow.inc';
    $predefined = _wow_get_predefined_list();

    $region = $predefined[$langcode][0];
    $locale = $predefined[$langcode][1];
    $name = $predefined[$langcode][2];
    $native = isset($predefined[$langcode][3]) ? $predefined[$langcode][3] : $name;
  }

  $languages = language_list();
  $enabled = $languages[$langcode]->enabled;
  db_insert('wow_services')
    ->fields(array(
      'language' => $langcode,
      'name' => $name,
      'native' => $native,
      'region' => $region,
      'locale' => $locale,
      'enabled' => $enabled,
    ))
    ->execute();

  // Only set it as default if enabled.
  if ($enabled && language_default('language') == $langcode) {
    variable_set('wow_service_default', (object) array('language' => $langcode, 'region' => $region, 'locale' => $locale, 'name' => $name, 'enabled' => (int) $enabled));
  }

  // Kill the static cache in wow_service_list().
  drupal_static_reset('wow_service_list');

  watchdog('wow', 'The service %name has been created.', array('%name' => $name));
}
/**
 * @} End of "wow-api-add"
 */

/**
 * Menu callback; Displays wow admin services overview screen.
 */
function wow_services_overview_form() {
  drupal_static_reset('wow_service_list');
  $services = wow_service_list('language');

  $options = array();
  $form['name'] = array();
  $form['weight'] = array('#tree' => TRUE);
  foreach ($services as $langcode => $service) {

    $options[$langcode] = '';
    if ($service->enabled) {
      $enabled[] = $langcode;
    }
    $form['weight'][$langcode] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $service->name)),
      '#title_display' => 'invisible',
      '#default_value' => $service->weight,
      '#attributes' => array('class' => array('service-order-weight')),
    );
    $form['name'][$langcode] = array('#markup' => check_plain($service->name));
    $form['native'][$langcode] = array('#markup' => check_plain($service->native));
    $form['region'][$langcode] = array('#markup' => wow_api_info($service->region, 'name'));
    $form['locale'][$langcode] = array('#markup' => check_plain($service->locale));
  }
  $form['enabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled services'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => isset($enabled) ? $enabled : array(),
  );
  $form['site_default'] = array(
    '#type' => 'radios',
    '#title' => t('Default service'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => wow_service_default('language'),
    '#disabled' => TRUE,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  $form['#theme'] = 'wow_services_overview_form';

  return $form;
}

/**
 * Returns HTML for the WoW Services content form.
 */
function theme_wow_services_content_form($variables) {

}

/**
 * Returns HTML for the WoW Services overview form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_wow_services_overview_form($variables) {
  $rows = array();
  $form = $variables['form'];
  $default = wow_service_default();
  foreach ($form['name'] as $key => $element) {
    // Do not take form control structures.
    if (is_array($element) && element_child($key)) {
      // Disable checkbox for the default language, because it cannot be disabled.
      if ($key == $default->language) {
        $form['enabled'][$key]['#attributes']['disabled'] = 'disabled';
      }

      // Add invisible labels for the checkboxes and radio buttons in the table
      // for accessibility. These changes are only required and valid when the
      // form is themed as a table, so it would be wrong to perform them in the
      // form constructor.
      $title = drupal_render($form['name'][$key]);
      $form['enabled'][$key]['#title'] = t('Enable !title', array('!title' => $title));
      $form['enabled'][$key]['#title_display'] = 'invisible';
      $form['site_default'][$key]['#title'] = t('Set !title as default', array('!title' => $title));
      $form['site_default'][$key]['#title_display'] = 'invisible';
      $rows[] = array(
        'data' => array(
          '<strong>' . $title . '</strong>',
          drupal_render($form['native'][$key]),
          drupal_render($form['locale'][$key]),
          drupal_render($form['region'][$key]),
          array('data' => drupal_render($form['enabled'][$key]), 'align' => 'center'),
          drupal_render($form['site_default'][$key]),
          drupal_render($form['weight'][$key]),
          l(t('edit'), 'admin/config/wow/service/edit/' . $key) . (($key != 'en' && $key != $default->language) ? ' ' . l(t('delete'), 'admin/config/wow/service/delete/' . $key) : '')
        ),
        'class' => array('draggable'),
      );
    }
  }
  $header = array(array('data' => t('English name')), array('data' => t('Native name')), array('data' => t('Code')), array('data' => t('Region')), array('data' => t('Enabled')), array('data' => t('Default')), array('data' => t('Weight')), array('data' => t('Operations')));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'service-order'), 'empty' => t('No services available.')));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('service-order', 'order', 'sibling', 'service-order-weight');

  return $output;
}

/**
 * Process service overview form submissions, updating existing services.
 */
function wow_services_overview_form_submit($form, &$form_state) {
  $services = wow_service_list();
  $default = wow_service_default();

  foreach ($services as $langcode => $service) {
    if ($form_state['values']['site_default'] == $langcode || $default->language == $langcode) {
      // Automatically enable the default language and the language
      // which was default previously (because we will not get the
      // value from that disabled checkbox).
      $form_state['values']['enabled'][$langcode] = 1;
    }

    $service->weight = $form_state['values']['weight'][$langcode];

    if ($form_state['values']['enabled'][$langcode]) {
      $service->enabled = 1;
    }
    else {
      $service->enabled = 0;
    }

    db_update('wow_services')
      ->fields(array(
        'weight' => $service->weight,
        'enabled' => $service->enabled,
      ))
      ->condition('language', $langcode)
      ->execute();
  }

  drupal_set_message(t('Configuration saved.'));

  $form_state['redirect'] = 'admin/config/wow/service';
  return;
}
