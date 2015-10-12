<?php
/**
 * @file
 * Admin functionality for controlling the Broadstreet Ads module.
 */

/**
 * Primary FormAPI callback for building the form.
 */
function broadstreet_ads_settings_form() {
  $form['zones'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ad zones'),
    // '#description' => t(''),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $zones = variable_get('broadstreet_ads_zones', array());
  ksort($zones);
  $ctr = 0;

  // No ad zones.
  if (empty($zones)) {
    $form['zones']['existing'] = array(
      '#markup' => '<p>' . t('No ad zones have been added yet.') . "</p>\n",
    );
    $form['zones']['#collapsible'] = FALSE;
  }

  // Display the ad zones.
  else {
    // Add some help to the fieldset.
    $form['zones']['#description'] = t('To remove an ad zone simply clear out its zone ID value.');

    // Prep work to display this as a table.
    $form['zones']['existing']['#tree'] = TRUE;
    $form['#theme'] = 'broadstreet_ads_settings_form';

    // Add fields for each zone.
    foreach ($zones as $zone_id => $label) {
      $form['zones']['existing'][$zone_id]['id'] = array(
        '#type' => 'textfield',
        '#default_value' => $zone_id,
      );
      $form['zones']['existing'][$zone_id]['label'] = array(
        '#type' => 'textfield',
        '#default_value' => $label,
        '#field_prefix' => t('Broadstreet Ad:'),
      );
      if (is_numeric($label)) {
        $form['zones']['existing'][$zone_id]['label']['#field_prefix'] .= ' #';
      }
      $ctr++;
    }
  }

  // Provide a way of creating another ad zone.
  $form['new_zone'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add another zone'),
    '#description' => t('By filling in these values a new ad zone can be added to the list.'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($zones),
  );
  $form['new_zone']['new_zone_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Zone ID (numeric, required)'),
    '#default_value' => '',
  );
  $form['new_zone']['new_zone_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Zone Label (optional)'),
    '#default_value' => '',
    '#field_prefix' => t('Broadstreet Ad:'),
    '#description' => t('The zone ID will be used if left blank.'),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['advanced']['blocks'] = array(
    '#title' => t('Enable ad blocks'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('broadstreet_ads_blocks', TRUE),
    '#description' => t('Uncheck to disable ad display using the core <a href="!blocks">Block</a> system. Should only be disabled when using <a href="https://drupal.org/project/panels">Panels</a>, <a href="https://drupal.org/project/panelizer">Panelizer</a> or <a href="https://drupal.org/project/panels_everywhere">Panels Everywhere</a> modules to position the ads.', array('!blocks' => url('admin/structure/block'))),
  );

  // API integration.
  // $form['api'] = array(
  //   '#type' => 'fieldset',
  //   '#title' => t('Broadstreet Ads API integration'),
  //   '#description' => t('By using the API integration the module can automatically load the ad position details, rather than having to manually type them in below.'),
  //   '#collapsible' => TRUE,
  //   '#collapsed' => TRUE,
  // );
  // $form['api']['broadstreet_ads_account_id'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('API token'),
  //   '#default_value' => variable_get('broadstreet_ads_account_id', ''),
  // );

  // Submit button.
  $form['actions'] = array();
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * FormAPI validation callback for broadstreet_ads_settings_form.
 */
function broadstreet_ads_settings_form_validate($form, &$form_state) {
  // Verify the new zone values.
  if (!empty($form_state['values']['new_zone_id'])) {
    if (!is_numeric($form_state['values']['new_zone_id'])) {
      form_set_error('new_zone_id', t('The new ad zone ID must be numerical, i.e. simple numbers.'));
    }
  }

  // Verify the existing ad zones.
  if (!empty($form_state['values']['existing'])) {
    foreach ($form_state['values']['existing'] as $key => $zone) {
      if (!empty($zone['id']) && !is_numeric($zone['id'])) {
        form_set_error($key . '][id', t('Ad zone IDs must be numerical, i.e. simple numbers.'));
      }
    }
  }
}

/**
 * FormAPI submission callback for broadstreet_ads_settings_form.
 */
function broadstreet_ads_settings_form_submit(&$form, &$form_state) {
  $zones = array();

  // Save the account ID.
  variable_set('broadstreet_ads_account_id', $form_state['values']['account_id']);

  // Save the existing ad zone data.
  if (!empty($form_state['values']['existing'])) {
    foreach ($form_state['values']['existing'] as $key => $zone) {
      $zone_id = $zone['id'];
      $zone_label = $zone['label'];

      // Only save zones that have an ID, i.e. allow records to be removed by
      // blanking out the ad zone.
      if (!empty($zone_id)) {
        // Default label.
        if (empty($zone_label)) {
          $zone_label = $zone_id;
        }
        $zones[$zone_id] = $zone_label;
      }
    }
  }

  // Save the new ad.
  if (!empty($form_state['values']['new_zone_id'])) {
    $zone_id = intval($form_state['values']['new_zone_id']);
    $zones[$zone_id] = $zone_id;
    if (!empty($form_state['values']['new_zone_label'])) {
      $zones[$zone_id] = $form_state['values']['new_zone_label'];
    }
  }

  // Save the zones.
  if (!empty($zones)) {
    // Sort the ad zones so they're listed by zone ID.
    ksort($zones);

    variable_set('broadstreet_ads_zones', $zones);

    variable_set('broadstreet_ads_blocks', $form_state['values']['blocks']);
  }
}

/**
 * Theme callback for broadstreet_ads_settings_form.
 */
function theme_broadstreet_ads_settings_form($form = array()) {
  $header = array(t('Zone ID'), t('Zone label'));
  $keys = array('id', 'label');

  // Build the table structure.
  $rows = array();
  if (!empty($form['zones']['existing'])) {
    foreach (element_children($form['zones']['existing']) as $zone_id) {
      $cells = array();
      foreach ($keys as $key) {
        $cells[] = drupal_render($form['zones']['existing'][$zone_id][$key]);
      }
      $rows[] = $cells;
    }
  }
  if (!empty($rows)) {
    $form['zones']['existing']['#value'] = theme('table', $header, $rows);
  }

  return drupal_render($form);
}
