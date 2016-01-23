<?php
/**
 * @file
 *
 */

/**
 * Form of the mapping, map CCK fields to ax schema attributes
 */
function openid_profile_map($form, $form_state) {
  $mapping = $attributes = $mapped_attributes = $unmapped_attributes = array();
  $source = openid_profile_get_mapping($mapping);
  $already_mapped = array_keys($mapping);

  ctools_include('plugins');
  $plugins = ctools_get_plugins('openid_profile', 'field_ax');

  // Get supported attributes and fields from plugins
  foreach ($plugins as $name => $plugin) {
    $plugin = openid_profile_load_plugin($plugin['name']);
    if (method_exists($plugin, 'get_attributes')) {
      $attributes += (array) $plugin->get_attributes();
    }
  }

  // Allow other modules to hook into the profile field definition
  drupal_alter('openid_profile', $attributes);

  // Determin mapped attributes
  foreach ($mapping as $mapped_field) {
    $mapped_attributes[$mapped_field['value']] = $attributes[$mapped_field['value']];
  }
  // Determin unmapped attributes
  $unmapped_attributes = array_diff($attributes, $mapped_attributes);

  $options_openid = array();
  $openid_attrs = openid_ax_api_schema_definitions();

  if (module_exists('openid_provider_ax') || module_exists('openid_client_ax')) {
    foreach ($openid_attrs as $id => $attr) {
      if (!in_array($id, $already_mapped)) {
        $options_openid += array($id => 'AX: ' . $attr['label']);
      }
    }
  }

  $openid_sreg_fields = array();
  if (module_exists('openid_provider_sreg') || module_exists('openid_client_sreg')) {
    foreach ($openid_attrs as $id => $attr) {
      if (isset($attr['sreg']) && !in_array($attr['sreg'], $already_mapped)) {
        $openid_sreg_fields += array($attr['sreg'] => 'SReg: ' . $attr['label']);
      }
    }
  }

  if (!module_exists('openid_client_ax') && !module_exists('openid_provider')) {
    $form['warning']['#value'] = t('Enable OpenID Client AX module to use this module on an OpenID client site.');
    return $form;
  }
  elseif (!(module_exists('openid_provider_sreg') || module_exists('openid_provider_ax')) && !module_exists('openid_client_ax')) {
    $form['warning']['#value'] = t('Enable OpenID Provider SReg, OpenID Provider AX or both to use the module on an OpenID provider site.');
    return $form;
  }

  // Collect sreg labels
  $sreg_fields = array();
  foreach ($openid_attrs as $id => $attr) {
    if (isset($attr['sreg'])) {
      $sreg_fields[$attr['sreg']] = $attr['label'];
    }
  }

  // Generate table for showing the current mapping.
  // Show standard fields, not managable.
  $rows = array(
    array(
      'node' => 'Account: Username',
      'openid' => 'SReg: Nickname',
      'handler' => '<i>Standard field</i>',
      'link' => '',
    ),
    array(
      'node' => 'Account: Email',
      'openid' => 'SReg: Email',
      'handler' => '<i>Standard field</i>',
      'link' => '',
    ),
  );

  foreach ($mapping as $openid => $field) {
    if (isset($field['handler']['name'])) {
      $handler = '<i>' . $field['handler']['name']. '</i>';
    }
    else {
      $handler = '<i><strong>' . t('missing') . '</i></strong>';
    }

    if (!strstr($openid, '.sreg.')) {
      $rows[] = array(
        'node' => $attributes[$field['value']],
        'openid' => 'AX: ' . $openid_attrs[$openid]['label'],
        'handler' => '<i>' . $handler . '</i>',
        'link' => l(t('Delete'), "openid_profile/delete/" . base64_encode($openid)),
      );
    }
    else {
      $rows[] = array(
        'node' => $attributes[$field_name],
        'openid' => 'SReg: ' . $sreg_fields[$openid],
        'handler' => '<i>' . $handler . '</i>',
        'link' => l(t('Delete'), "openid_profile/delete/" . base64_encode($openid) . '/sreg'),
      );
    }
  }

  $current_map = theme('table', array(
    'header' => array(
      'node' => t('Field'),
      'openid' => t('OpenID field'),
      'handler' => t('Handler'),
      'link' => t('Operation')),
    'rows' => $rows,
    'empty' => t('No mapping defined.')));

  if ($source == OPENID_PROFILE_DEFAULT) {
    $status = t('(default)');
  }
  elseif ($source == OPENID_PROFILE_OVERRIDDEN) {
    $status = t('(default, <em>overridden</em>)');
  }
  else { // OPENID_PROFILE_NORMAL
    $status = '';
  }
  $form['current_map'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mappings !status', array('!status' => $status)),
    '#collapsible' => FALSE,
  );
  $form['current_map']['map'] = array(
    '#markup' => isset($current_map) ? $current_map : '',
  );
  if ($source == OPENID_PROFILE_OVERRIDDEN) {
    $form['current_map']['revert'] = array(
      '#type' => 'submit',
      '#value' => t('Revert'),
      '#submit' => array('openid_profile_map_revert_submit'),
    );
  }
  $form['map'] = array(
    '#type' => 'fieldset',
    '#title' => t('Map user fields to OpenID fields'),
  );
  if (count($unmapped_attributes) == 0 || (count($openid_attrs) == 0 && count($openid_sreg_fields) == 0)) {
    $form['map']['info'] = array('#markup' => t('No more fields to map.'));
  }
  else {
    $form['#attached']['css'] = array(drupal_get_path('module', 'openid_profile') . "/openid_profile.css");
    $form['map']['map_a'] = array(
      '#type' => 'select',
      '#title' => t('User fields'),
      '#options' => $unmapped_attributes,
    );
    $form['map']['map_b'] = array(
      '#type' => 'select',
      '#title' => t('OpenID field'),
      '#options' => $options_openid + $openid_sreg_fields,
    );
    $form['map']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
      '#submit' => array('openid_profile_map_add_submit'),
    );
    $form['map']['#description'] = t('Fields defined by the <a href="http://openid.net/specs/openid-attribute-exchange-1_0.html">Attribute Exchange</a> specification are prefixed AX, fields defined by the <a href="http://openid.net/specs/openid-simple-registration-extension-1_0.html">Simple Registration</a> specification are prefixed SReg.');
  }

  // Define read-only fields on the client
  if (module_exists('openid_client_ax')) {
    $form['readonly'] = array(
      '#type' => 'fieldset',
      '#title' => t('Define read-only fields.'),
      '#description' => t('Select the fields that can only be changed on the provider side.'),
    );
    $form['readonly']['openid_profile_read_only_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Readonly fields.'),
      '#options' => _openid_profile_read_only_prepare($attributes),
      '#default_value' => variable_get('openid_profile_read_only_fields', array()),
    );
    $form['readonly']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('openid_profile_read_only_submit'),
    );
  }

  return $form;
}

function openid_profile_read_only_submit($form, &$form_state) {
  variable_set('openid_profile_read_only_fields', $form_state['values']['openid_profile_read_only_fields']);
}

/**
 * Saves the user-defined mapping.
 */
function openid_profile_map_add_submit($form, &$form_state) {
  $mapping = array();
  openid_profile_get_mapping($mapping);

  if (!empty($form_state['values']['map_b']) && !empty($form_state['values']['map_a'])) {

    // Getting field type or name of field in case of immediate user's fields
    $field_elements = explode('::', $form_state['values']['map_a']);
    $field_name = array_shift($field_elements);
    if (!$field_info = field_info_field($field_name)) {
      $field_info = array('type' => $field_name);
    }

    // Selecting adeqaute handler for field type to AX conversion
    ctools_include('plugins');
    $plugins = ctools_get_plugins('openid_profile', 'field_ax');

    $valid_plugins = array();
    foreach ($plugins as $plugin) {
      if (in_array($field_info['type'], $plugin['field_types'])) {
        $valid_plugins[] = $plugin;
      }
    }

    // @TODO: Move this check to validation hook
    if (!empty($valid_plugins)) {
      $mapping += array(
        $form_state['values']['map_b'] => array(
          'value'=> $form_state['values']['map_a'],
          // @TODO: Make handlers selectable through the UI
          'handler' => $valid_plugins[0]['handler'],
          'field_type' => $field_info['type'],
        ),
      );
      variable_set('openid_profile_map', $mapping);
    }
    else {
      drupal_set_message(t('No valid AX convertion handler found for the selected field'), 'error');
    }
  }
}

function openid_profile_map_revert_submit($form, &$form_state) {
  $mapping_imported = module_invoke_all('openid_profile_mapping');
  variable_set('openid_profile_map', $mapping_imported[$type]);
}

/**
 * Shows the current mapping code to the user.
 */
function openid_profile_export($form, $form_state, $type = '') {
  $mapping = array();
  openid_profile_get_mapping($mapping, $type);
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $array = '$mapping = array();';
  $head = "\$mapping[\"$type\"] = ";
  $tail = ';';
  $return = '
return $mapping;';
  $form['show'] = array(
    '#title' => t('PHP code'),
    '#type' => 'textarea',
    '#default_value' => $array . $head . var_export($mapping, TRUE) . $tail . $return,
    '#description' => t('This code can be pasted into an implementation of hook_openid_profile_mapping().'),
  );
  return $form;
}

/**
 * Deletes the user-defined mapping
 */
function openid_profile_delete() {
  $openid = base64_decode(arg(2));
  $mapping = array();
  $source = openid_profile_get_mapping($mapping);
  if ($source == OPENID_PROFILE_DEFAULT) {
    variable_set('openid_profile_map', $mapping);
  }
  $mapping = variable_get('openid_profile_map', array());
  unset($mapping[$openid]);
  variable_set('openid_profile_map', $mapping);
  drupal_goto('admin/config/people/accounts/openid_ax_mapping/edit');
}

function _openid_profile_read_only_prepare($attributes) {
  $read_only = array();
  foreach ($attributes as $name => $label) {
    $field_elements = explode('::', $name);
    $field_name = array_shift($field_elements);
    if ($field_instance = field_info_instance('user', $field_name, 'user')) {
      $read_only[$field_name] = t('Field: @label', array('@label' => $field_instance['label']));
    }
    else {
      $read_only[$name] = $label;
    }
  }
  return $read_only;
}
