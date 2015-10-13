<?php
/**
 * @file
 * Standard installation functions.
 */

/**
 * Menu callback to renders all of the fields for the node entity type.
 */
function domain_fields_settings_form($form, &$form_state, $domain, $entity_type = 'node') {
  $settings = domain_fields_settings($domain);
  $instances = field_info_instances();
  $field_types = field_info_field_types();
  $bundles = field_info_bundles();

  $modules = system_rebuild_module_data();

  $form['#headers'] = array(t('Bundle instance'), t('Settings'));
  $form['#instances'] = $instances;
  $form['#bundles'] = $bundles;

  $form['#tree'] = TRUE;
  $form['#entity'] = entity_get_info($entity_type);
  $form['#entity_type'] = $form_state['entity_type'] = $entity_type;
  $form['#domain'] = $form_state['domain'] = $domain;
  foreach ($instances as $_entity_type => $type_bundles) {
    if ($entity_type != $_entity_type) {
      continue;
    }
    foreach ($type_bundles as $bundle => $bundle_instances) {
      foreach ($bundle_instances as $field_name => $instance) {
        $field = field_info_field($field_name);
        $form[$entity_type][$bundle][$field_name] = array(
          '#type' => 'fieldset',
          '#title' => t('@field_label (@field_name)', array(
            '@field_name' => $field['locked'] ? t('@field_name (Locked)', array('@field_name' => $field_name)) : $field_name,
            '@field_label' => $field_types[$field['type']]['label'],
          )),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#description' => t('<small>(!module module)</small>', array(
            '!module' => $field_types[$field['type']]['module'],
          )),
        );
        $form[$entity_type][$bundle][$field_name]['required'] = array(
          '#type' => 'checkbox',
          '#title' => t('Force requried'),
          '#default_value' => _domain_fields_instance_settings($entity_type, $bundle, $field_name, 'required', 0, $domain),
        );
        $form[$entity_type][$bundle][$field_name]['edit'] = array(
          '#type' => 'checkbox',
          '#title' => t('Remove from forms'),
          '#default_value' => _domain_fields_instance_settings($entity_type, $bundle, $field_name, 'edit', 0, $domain),
        );
        $form[$entity_type][$bundle][$field_name]['view'] = array(
          '#type' => 'checkbox',
          '#title' => t('Remove from all displays'),
          '#default_value' => _domain_fields_instance_settings($entity_type, $bundle, $field_name, 'view', 0, $domain),
        );
        $form[$entity_type][$bundle][$field_name]['label'] = array(
          '#type' => 'textfield',
          '#title' => t('Label override'),
          '#default_value' => _domain_fields_instance_settings($entity_type, $bundle, $field_name, 'label', '', $domain),
        );
        $form[$entity_type][$bundle][$field_name]['help'] = array(
          '#type' => 'textarea',
          '#title' => t('Help override'),
          '#default_value' => _domain_fields_instance_settings($entity_type, $bundle, $field_name, 'help', '', $domain),
        );
      }
    }
  }

  if (!empty($form)) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('domain_fields_settings_form_save_submit'),
    );
    $form['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('domain_fields_settings_form_reset_submit'),
    );
  }

  return $form;
}

/**
 * Submit handler to reset all of the domains field settings.
 */
function domain_fields_settings_form_reset_submit($form, &$form_state) {
  $domain_id = $form_state['domain']['domain_id'];
  $entity_type = $form_state['entity_type'];

  $settings = variable_get('domain_fields_' . $domain_id, array());
  $settings[$entity_type] = array();
  variable_set('domain_fields_' . $domain_id, $settings);
}

/**
 * Submit handler to save all of the domains field settings.
 */
function domain_fields_settings_form_save_submit($form, &$form_state) {
  $domain_id = $form_state['domain']['domain_id'];
  $entity_type = $form_state['entity_type'];

  $settings = variable_get('domain_fields_' . $domain_id, array());
  $settings[$entity_type] = $form_state['values'][$entity_type];
  variable_set('domain_fields_' . $domain_id, $settings);
}

/**
 * Theme the domain field settings form.
 */
function theme_domain_fields_settings_form($variables) {
  $form = $variables['form'];

  $bundles = $form['#bundles'];
  $entity = $form['#entity'];

  $output = '';

  $output .= t('<p>The following form allows you to alter the %entity field settings for the domain %domain.</p>',
      array('%entity' => domain_fields_entity_title($form['#entity_type']), '%domain' => domain_title($form['#domain'])));
  $output .= t('<p>Form and display access settings are incremental; they add restrictions to the field but never grant access if restricted. A required field will stay required, even if the domain field settings suggest otherwise.</p>');
  $output .= t('<p>Required, labels and help text should alter most fields. However, if you discover a field that is not altered, please lodge a feature request at !link to add support for the field. Text overrides are not translated.</p>',
    array('!link' => l(t('Domain fields issue queue'), 'http://drupal.org/project/issues/domain_fields', array('external' => TRUE))));

  foreach ($form['#instances'] as $entity_type => $type_bundles) {
    if ($entity_type != $form['#entity_type']) {
      continue;
    }
    $rows = array();
    foreach ($type_bundles as $bundle => $bundle_instances) {
      $bn = t('@bundle (@type)', array('@bundle' => $bundles[$entity_type][$bundle]['label'], '@type' => $entity_type));
      $rows[$bundle][0] = array('data' => $bn, 'style' => 'vertical-align: top;');
      $rows[$bundle][1] = array('data' => drupal_render($form[$entity_type][$bundle]));
    }

    if (empty($rows)) {
      $output .= t('No fields have been defined yet.');
    }
    else {
      // Sort rows by field name.
      ksort($rows);
      $headers = array(
        0 => array('data' => $form['#headers'][0], 'style' => 'width: 9%;'),
        1 => array('data' => $form['#headers'][1], 'style' => 'width: 90%;'),
      );
      $output .= theme('table', array('header' => $headers, 'rows' => $rows));
    }
  }

  $output .= drupal_render_children($form);

  return $output;
}
