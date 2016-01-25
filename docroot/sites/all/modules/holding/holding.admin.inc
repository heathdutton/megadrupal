<?php

/**
 * @file holding.admin.inc
 *
 * Contains admin page callbacks.
 */

/**
 * Admin settings form.
 */
function holding_admin($form_state) {
  $form['holding_sites'] = array(
    '#type' => 'textarea',
    '#title' => t('Holding sites'),
    '#description' => t("List of site URLs, one per line. On a single site installation, use 'default'."),
    '#default_value' => implode("\n", unserialize(variable_get('holding_sites', serialize(array())))),
  );

  $form['holding_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to holding page'),
    '#description' => t("Drupal-relative path to your holding page. This must be the exact file name; no directory indexing. The path will not be visible to visitors. Examples: 'holding/index.html', 'sites/all/themes/YOURTHEME/holding/index.html'"),
    '#default_value' => variable_get('holding_page', ''),
  );

  $form['holding_parse_tokens'] = array(
    '#type' => 'checkbox',
    '#title' => t('Parse the holding page for tokens'),
    '#description' => t("If enabled, the holding page file will be parsed for the tokens."),
    '#default_value' => variable_get('holding_parse_tokens', FALSE),
  );

  if (module_exists('token')) {
    $form['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array(),
      // There are no fields to insert tokens into, so clickability just hampers
      // ability to copy and paste.
      '#click_insert' => FALSE,
    );
  }

  $form['#submit'][] = 'holding_admin_submit';

  // automatic_defaults FALSE because we have to unserialize the sites.
  return system_settings_form($form, FALSE);
}

/**
 * Admin settings form validation.
 *
 * Check the holding page file and site folders exist.
 */
function holding_admin_validate($form_id, &$form_state) {
  // Check the holding page exists as a file.
  if (!file_exists($form_state['values']['holding_page'])) {
    form_set_error('holding_page', t('The file @file does not exist.', array('@file' => check_plain($form_state['values']['holding_page']))));
  }

  // Check each of the site folders exists in /sites.
  $sites = array_map('trim', explode("\n", $form_state['values']['holding_sites']));
  $bad_sites = array();
  foreach ($sites as $conf_dir) {
    if (!file_exists('sites/' . $conf_dir)) {
      // Can't set form_set_error on each one here: @see <http://drupal.org/node/549020>
      $bad_sites[] = check_plain($conf_dir);
    }
  }
  if (count($bad_sites)) {
    drupal_set_message('The following directories do not exist; be sure these are the ones you mean: ' . implode(', ', $bad_sites), 'warning');
  }
}

/**
 * Admin settings form submit.
 *
 * Act before system_settings_form_submit() and turn the submitted sites list into a serialized array.
 */
function holding_admin_submit($form_id, &$form_state) {
  # cookie if you can tell me why explode leaves a damn \n at the end of the lines:
  # print "<pre> >>sites[0]<< </pre>";
  $sites = array_map('trim', explode("\n", $form_state['values']['holding_sites']));
  $form_state['values']['holding_sites'] = serialize($sites);
}
