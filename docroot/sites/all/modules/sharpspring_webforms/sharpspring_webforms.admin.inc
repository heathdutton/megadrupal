<?php
/**
 * @file
 * Provides admin configuration for the SharpSpring Webforms module.
 */

/**
 * Defines the SharpSpring Webform configuration form.
 */
function sharpspring_webforms_config_form($form, &$form_state) {
  $form['tracking']['webforms'] = array(
    '#title' => t('Webform Lead Generation'),
    '#description' => '<p>' . t("Add unique tracking IDs to each webform on the site. For each webform you want to track, enter the form's endpoint ID as it appears in the unique tracking code. It should be formatted like @endpoint.", array('@endpoint' => 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx (8-4-4-4-12)')) . '</p>',
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );

  // Get list of all webforms.
  $wf_query = db_select('webform', 'w')
    ->fields('w')
    ->orderBy('nid')
    ->addTag('node_access')
    ->execute()
    ->fetchAll();

  // Get list of all SharpSpring form endpoints.
  $webform_codes = _sharpspring_webforms_return_endpoint_values();

  $forms_count = 0;
  foreach ($wf_query as $row) {
    if (!empty($row->nid)) {
      $forms_count++;
      $webform = node_load($row->nid);

      $unpublished = (isset($webform->status)) ? !$webform->status : TRUE;

      $enabled = (!empty($webform_codes[$row->nid]->status)) ? $webform_codes[$row->nid]->status : 0;
      $endpoint = (!empty($webform_codes[$row->nid]->endpoint)) ? $webform_codes[$row->nid]->endpoint : '';
      $base_uri = (!empty($webform_codes[$row->nid]->base_uri)) ? $webform_codes[$row->nid]->base_uri : '';
      $title = (!empty($webform->title)) ? $webform->title : '';

      $form['tracking']['webforms']['enable'][$row->nid] = array(
        '#title' => l($title, 'node/' . $row->nid),
        '#default_value' => $enabled,
        // Disable checkbox if form is unpublished.
        '#disabled' => $unpublished,
        '#type' => 'checkbox',
        '#size' => 50,
        '#maxlength' => 40,
      );

      $form['tracking']['webforms'][$row->nid] = array(
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#states' => array(
          'visible' => array(
            ':input[name="' . $row->nid . '"]' => array('checked' => TRUE),
          ),
        ),
      );

      $form['tracking']['webforms'][$row->nid]['header'] = array(
        '#markup' => '<h4 style="margin-top: -10px;">"' . check_plain($title) . '" Form</h4>',
      );

      $form['tracking']['webforms'][$row->nid]['base_uri_' . $row->nid] = array(
        '#title' => 'Base URI',
        '#description' => t("The form's baseURI will appear in the tracking code as:") . " <pre>__ss_noform.push(['baseURI', '<strong>https://app-XXXXXX.sharpspring.com/webforms/receivePostback/XXXXXXXX/</strong>']);</pre>",
        '#default_value' => $base_uri,
        '#type' => 'textfield',
        '#size' => 50,
        '#maxlength' => 255,
      );

      $form['tracking']['webforms'][$row->nid]['endpoint_' . $row->nid] = array(
        '#title' => 'Endpoint',
        '#description' => t("The form's endpoint will appear in the tracking code as:") . " <pre>__ss_noform.push(['endpoint', '<strong>xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx</strong>']);</pre>",
        '#default_value' => $endpoint,
        '#type' => 'textfield',
        '#size' => 70,
        '#maxlength' => 40,
      );
    }

  }

  $form['tracking']['webforms']['count'] = array(
    '#type' => 'hidden',
    '#default_value' => $forms_count,
  );

  $form['#submit'][] = 'sharpspring_webforms_config_form_submit';
  return system_settings_form($form);
}

/**
 * SharpSpring Webform admin form validation.
 */
function sharpspring_webforms_config_form_validate(&$form, &$form_state) {
  // Only get numeric array keys within webforms group (i.e. webform nids).
  $keys = array_filter(array_keys($form['tracking']['webforms']), 'is_int');

  foreach ($keys as $wid) {

    $status = $form['tracking']['webforms']['enable'][$wid]['#value'];
    $base_uri = $form['tracking']['webforms'][$wid]['base_uri_' . $wid]['#value'];
    $endpoint = $form['tracking']['webforms'][$wid]['endpoint_' . $wid]['#value'];

    // If the form tracking isn't active, it doesn't matter if these are wrong.
    if ($status == 1) {
      // Check whether the base URI and endpoint will actually work.
      if (!_sharpspring_validate_endpoint($endpoint)) {
        form_set_error('endpoint_' . $wid, t('A valid SharpSpring Endpoint is formatted like @endpoint. Please check for any whitespace in your entry.', array('@endpoint' => 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx')));
        continue;
      }
      if (!_sharpspring_validate_uri($base_uri)) {
        form_set_error('base_uri_' . $wid, t('A valid SharpSpring Base URI is formatted like @baseuri.', array('@baseuri' => 'https://app-XXXXXX.sharpspring.com/webforms/receivePostback/XXXXXXXX')));
        continue;
      }
    }
  }
}

/**
 * SharpSpring Webform admin form submit.
 */
function sharpspring_webforms_config_form_submit(&$form, &$form_state) {
  // Only get numeric array keys within webforms group (i.e. webform nids).
  $keys = array_filter(array_keys($form['tracking']['webforms']), 'is_int');

  foreach ($keys as $wid) {

    $status = $form['tracking']['webforms']['enable'][$wid]['#value'];
    $base_uri = $form['tracking']['webforms'][$wid]['base_uri_' . $wid]['#value'];
    $endpoint = $form['tracking']['webforms'][$wid]['endpoint_' . $wid]['#value'];

    // Check for existing webform settings.
    $result_check = db_select('sharpspring_webforms', 'nid')
      ->fields('nid')
      ->condition('nid', $wid, '=')
      ->execute();

    $existing = FALSE;
    foreach ($result_check as $result) {
      if (!empty($result)) {
        $existing = TRUE;
      }
    }

    // Now save new info.
    $table_name = 'sharpspring_webforms';
    $data = array(
      'nid' => $wid,
      'base_uri' => $base_uri,
      'endpoint' => $endpoint,
      'status' => $status,
    );

    if ($existing) {
      drupal_write_record($table_name, $data, 'nid');
    }
    else {
      drupal_write_record($table_name, $data);
    }

  }
}
