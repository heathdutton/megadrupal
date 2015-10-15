<?php

/**
 * @file
 * Admin file for the silverpop module.
 */

/**
 * Silverpop admin form.
 */
function silverpop_admin() {
  $form = array();

  $form['silverpop_tracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Silverpop Tracking'),
    '#collapsible' => TRUE,
  );

  $tracking_image_example_url = drupal_get_path('module', 'silverpop') . '/images/silverpop-web-tracking-code.png';

  $tracking_image = theme('image', array(
    'attributes' => array('style' => 'border: 1px solid #666; width: 50%;'),
    'alt' => 'Web Tracking Code example.',
    'path' => $tracking_image_example_url
  ));

  $form['silverpop_tracking']['silverpop_help'] = array(
    '#markup' => '<p>You will need to grab two values from the Silverpop web tracking
      code to add tracking to this website.</p><p>' . $tracking_image . '</p>',
  );

  $form['silverpop_tracking']['silverpop_tracked_domains'] = array(
    '#type' => 'textarea',
    '#title' => t('Tracked Domains'),
    '#default_value' => variable_get('silverpop_tracked_domains', ''),
    '#description' => 'Enter a comma-separated list of domains for Silverpop to
      track from <a target="_blank" href="https://pilot.silverpop.com/viewOrganization.do">https://pilot.silverpop.com/viewOrganization.do</a>.',
    '#rows' => 2,
  );

  $form['silverpop_tracking']['silverpop_script_src'] = array(
    '#type' => 'textarea',
    '#title' => t('Silverpop Script Source URL'),
    '#default_value' => variable_get('silverpop_script_src', ''),
    '#description' => 'Copy the source URL from the web tracking code on
      <a target="_blank" href="https://pilot.silverpop.com/viewOrganization.do">https://pilot.silverpop.com/viewOrganization.do</a>.',
    '#rows' => 2,
  );

  $form['silverpop_events'] = array(
    '#type' => 'fieldset',
    '#title' => t('Silverpop Event Tracking'),
    '#collapsible' => TRUE,
  );

  $form['silverpop_events']['table'] = array(
    '#markup' => silverpop_overview()
  );

  // Only require these settings if the webform sub-module is enabled.
  // In the future there could be additional sub-modules that require API
  // settings.
  if (module_exists('silverpop_webform')) {
    $form['silverpop_api'] = array(
      '#type' => 'fieldset',
      '#title' => t('Silverpop API Configuration'),
      '#collapsible' => TRUE,
    );

    $form['silverpop_api']['help'] = array(
      '#markup' => '<p>Add API configuration here. These settings are required
        for Webform integration.</p>',
    );

    $form['silverpop_api']['silverpop_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Silverpop Engage username'),
      '#default_value' => variable_get('silverpop_username', ''),
      '#size' => 60,
      '#maxlength' => 255,
      '#description' => 'The username you use to log into the Silverpop
        administrative portal.',
      '#required' => TRUE,
    );

    $form['silverpop_api']['silverpop_password'] = array(
      '#type' => 'password',
      '#title' => t('Silverpop Engage password'),
      '#size' => 60,
      '#maxlength' => 255,
      '#description' => 'The password you use to log into the Silverpop
        administrative portal. To use existing password, leave this field blank.',
    );

    $form['silverpop_api']['silverpop_apihost'] = array(
      '#type' => 'textfield',
      '#title' => t('Silverpop API host'),
      '#default_value' => variable_get('silverpop_apihost', ''),
      '#size' => 60,
      '#maxlength' => 255,
      '#description' => 'Your API host is typically your engage pod number (e.g.
        Pod 5 would use api5.silverpop.com).',
      '#required' => TRUE,
    );
  }

  $form = system_settings_form($form);

  $form['#submit'] = array('silverpop_encrypt_password', 'system_settings_form_submit');

  return $form;
}

/**
 * Encrypt password (if necessary);
 */
function silverpop_encrypt_password($form, &$form_state) {
  // Encrypt the API password.
  if (module_exists('silverpop_webform')){
    if (!empty($form_state['values']['silverpop_password'])) {
      $password = $form_state['values']['silverpop_password'];
      $form_state['values']['silverpop_password'] = silverpop_set_password($password);
    }
    else {
      $form_state['values']['silverpop_password'] = silverpop_get_password(FALSE);
    }
  }
}

/**
 * Silverpop overview table.
 */
function silverpop_overview() {
  $result = db_query("SELECT * FROM {silverpop_settings} ORDER BY id DESC");

  $output = '';
  $addnew = theme('item_list', array('items' => array(l(t("Add New"), "admin/config/services/silverpop/add"))));

  if ($result->rowCount() == 0) {
    $output = '<p>No Silverpop event tracking set up.</p>';
    $output .= $addnew;
  }
  else {
    $header = array(
      array('data' => t('Custom Event Type')),
      array('data' => t('Custom Event Name')),
      array('data' => t('CSS Selector')),
      array('data' => t(''), 'colspan' => 2),
    );

    foreach ($result as $row) {
      $rows[] = array(
        array('data' => $row->event_type),
        array('data' => $row->event_name),
        array('data' => $row->css_selector),
        array('data' => l(t("Edit"), "admin/config/services/silverpop/" . $row->id . "/edit")),
        array('data' => l(t("Delete"), "admin/config/services/silverpop/" . $row->id . "/delete")),
      );
    }

    $output .= theme('table', array('header' => $header, 'rows' => $rows));
    $output .= $addnew;
  }

  return $output;
}

/**
 * Silverpop settings form.
 *
 * @param array $form
 * @param array $form_state
 * @param integer $id
 *   Webform id.
 *
 * @return array
 */
function silverpop_settings_form($form, &$form_state, $id=NULL) {
  $event_id = "";
  $css_selector = "";
  $event_type = "";
  $event_name = "";

  $event_image_example_url = drupal_get_path('module', 'silverpop') . '/images/silverpop-web-tracking-events.png';

  $event_image = theme('image', array(
    'attributes' => array('style' => 'border: 1px solid #666; width: 50%;'),
    'alt' => 'Custom Web Tracking Events example.',
    'path' => $event_image_example_url
  ));

  $form['help'] = array(
    '#markup' => '<p>You will need to set up custom events in Silverpop and
      associate them here.<br />@see <a target="_blank" href="https://pilot.silverpop.com/viewOrganization.do">https://pilot.silverpop.com/viewOrganization.do</a></p>
      <p>' . $event_image . '</p>'
  );

  if (isset($id)) {
    $sql = "SELECT * FROM {silverpop_settings} WHERE id = :id";
    $settings = db_query_range($sql, 0, 1, array(':id' => $id))->fetchObject();

    $form['id'] = array(
      '#type' => 'hidden',
      '#value' => $id,
    );

    $event_id = $settings->id;
    $css_selector = $settings->css_selector;
    $event_type = $settings->event_type;
    $event_name = $settings->event_name;
  }

  $form['event_type'] = array(
    '#type' => 'textfield',
    '#title' => 'Custom Event Name (no spaces allowed)',
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => $event_type,
    '#required' => TRUE,
    '#description' => 'Add the name of the custom event you set up in Silverpop.'
  );

  $form['event_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Custom Event Friendly Name',
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => $event_name,
    '#required' => TRUE,
    '#description' => 'Add the friendly name of the custom event you set up in Silverpop.'
  );

  $form['css_selector'] = array(
    '#type' => 'textfield',
    '#title' => 'CSS Selector',
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => $css_selector,
    '#required' => TRUE,
    '#description' => 'This is the CSS selector that will add Silverpop
      tracking. Examples are CSS id\'s (e.g. "#foobar") or class names (e.g. ".foobar").'
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Silverpop settings form submit.
 *
 * @param array $form
 * @param array $form_state
 */
function silverpop_settings_form_submit($form, &$form_state) {
  $event_name = $form_state['values']['event_name'];
  $event_type = $form_state['values']['event_type'];

  $css_selector = $form_state['values']['css_selector'];

  // Update settings table.
  if (isset($form_state['values']['id'])) {
    $id = $form_state['values']['id'];

    db_update('silverpop_settings')
      ->fields(array(
        'event_name' => $event_name,
        'event_type' => $event_type,
        'css_selector' => $css_selector
      ))
      ->condition('id', $id)
      ->execute();

    drupal_set_message(t("Settings updated successfully."));
  }
  // Insert into settings table.
  else {
    db_insert('silverpop_settings')
      ->fields(array(
        'event_name' => $event_name,
        'event_type' => $event_type,
        'css_selector' => $css_selector,
      ))
      ->execute();

    drupal_set_message(t("New settings saved successfully."));
  }

  drupal_goto("admin/config/services/silverpop");
}

/**
 * Silverpop settings delete confirmation.
 *
 * @param array $form
 * @param array $form_state
 * @param integer $id
 *   Webform id.
 */
function silverpop_settings_delete_confirm($form, &$form_state, $id) {
  $form['settings'] = array(
    '#type' => 'value',
    '#value' => $id
  );

  return confirm_form($form, t('Are you sure you want to delete this entry?'),
    'admin/config/services/silverpop', t('This action cannot be undone.'), t('Delete'),
    t('Cancel')
  );
}

/**
 * Silverpop settings delete confirm submit.
 *
 * @param array $form
 * @param array $form_state
 */
function silverpop_settings_delete_confirm_submit($form, &$form_state) {
  $form_values = $form_state['values'];

  if ($form_values['confirm']) {
    $id = $form_values['settings']['#value'];

    $query = "DELETE  FROM {silverpop_settings} where id=:id limit 1";
    db_query($query, array(':id' => $id));

    drupal_set_message(t('Entry has been deleted successfully.'));
  }

  drupal_goto("admin/config/services/silverpop");
}
