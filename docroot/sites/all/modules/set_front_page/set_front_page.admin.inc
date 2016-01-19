<?php
/**
 * @file
 * Admin settings.
 */

/**
 * FormAPI callback for the settings form.
 */
function set_front_page_config_form($form, &$form_state) {
  $front_page = variable_get('site_frontpage', '');
  $front_page_alias = url($front_page);
  $front_page_url = url($front_page, array('absolute' => TRUE));
  $form['site_frontpage'] = array(
    '#type' => 'textfield',
    '#title' => t("Site's current front page"),
    '#field_prefix' => t('Internal path:') . ' ',
    '#field_suffix' => '<br />'
      . t('Page alias: @url', array('@url' => $front_page_alias)) . '<br />'
      . t('Full URL: !url', array('!url' => l($front_page_url, $front_page_url))),
    '#default_value' => $front_page,
    '#description' => t('If not set, the built-in Drupal @path page will be used.', array('@path' => url('node'))),
  );

  $default = variable_get('set_front_page_default', '');
  $form['set_front_page_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default front page (optional)'),
    '#description' => t('If provided, each page will have an extra option allowing the front page to be reassigned to this default page instead.'),
    '#field_prefix' => t('Internal path:') . ' ',
    '#default_value' => $default,
  );
  if (!empty($default)) {
    $default_alias = url($default);
    $default_url = url($default, array('absolute' => TRUE));
    $form['set_front_page_default']['#field_suffix'] = '<br />'
      . t('Path alias: @url', array('@url' => $default_alias)) . '<br />'
      . t('Full URL: !url', array('!url' => l($default_url, $default_url)));
  }

  $form['_prefix'] = array(
    '#markup' => t('Select which types of site content should be allowed to be used as the homepage.'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  foreach (set_front_page_supported_entities() as $entity_type => $entity_info) {
    $form[$entity_type] = array(
      '#type' => 'container',
      '#tree' => FALSE,
    );
    $form[$entity_type]['_prefix'] = array(
      '#markup' => t($entity_info['bundle label plural']),
      '#prefix' => '<h4>',
      '#suffix' => '</h4>',
    );
    foreach ($entity_info['bundles'] as $bundle => $bundle_label) {
      $variable_name = 'set_front_page_' . $entity_type . '__' . $bundle;
      $form[$entity_type][$variable_name] = array(
        '#type' => 'checkbox',
        '#title' => t($bundle_label),
        '#default_value' => variable_get($variable_name, FALSE),
      );
    }
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submission callback for set_front_page_config_form().
 */
function set_front_page_config_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);

  // Save each variable.
  foreach ($form_state['values'] as $key => $value) {
    $value = trim($value);

    // If this value is the 'set_frontpage' value, and it's either empty or
    // equal to 'node', then delete the variable so that the default 'node' can
    // be used.
    if ($key == 'site_frontpage' && (empty($value) || $value == 'node')) {
      variable_del($key);
    }

    // Save the variable.
    else {
      variable_set($key, $value);
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}
