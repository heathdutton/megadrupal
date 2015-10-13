<?php

/**
 * @file
 * Contextual View Modes Administrative Screens
 */

/**
 * [contextual_view_modes_settings description]
 * @param  [type] $form       [description]
 * @param  [type] $form_state [description]
 * @return [type]             [description]
 */
function contextual_view_modes_settings($form, &$form_state) {


  // If the gotoconf variable is set then show the user the conf variable.
  if (isset($form_state['values']['gotoconf']) && $form_state['values']['gotoconf'] == "true") {
    $form_state['rebuild'] = TRUE;
    return confirm_form(
      $form,
      t("Are you sure you want to do this?"),
      'admin/structure/cvm',
      t("You have marked a content type to be disabled. Any previously checked and now unchecked content types will have their contextual view mode fields removed from them. This will destroy their data and cannot be recovered. Click yes to confirm that you know the consequences and wish to proceed."),
      t('Yes, Proceed')
    );
  }

  // Trigger for going to confirmation form.
  $form['gotoconf'] = array(
    '#type' => 'hidden',
    '#value' => "false"
  );

  // Enabled content types.
  $type_names = node_type_get_names();
  $enabled_types = variable_get('cvm_enabled_content_types', array());

  // Enabled and active contexts.
  $contexts = context_context_list();
  foreach ($contexts as $key => $name) {
    $contexts[$key] = ucwords(str_replace('_', " ", $name));
  }

  // Enabled and active display modes.
  $ds_modes = ds_entity_view_modes('node');
  $ds_modes_formatted = array('none' => '- None -');
  foreach ($ds_modes as $name => $values) {
    $ds_modes_formatted[$name] = ucwords(str_replace('_', " ", $values['label']));
  }


  $form['cvm_enabled_content_types'] = array(
    '#title' => t("Check Box To Enable Content Type"),
    '#type' => 'checkboxes',
    '#options' => $type_names,
    '#default_value' => $enabled_types,
  );

  // Default content type contextual view mode settings.

  $global_cvm = variable_get('cvm_global_content_type_modes', array());

  foreach ($enabled_types as $k => $name) {
    $form[$name] = array(
      '#type' => 'fieldset',
      '#title' => $type_names[$name],
      '#description' => t('Provide default global contextual view mode switching for this content type.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    foreach ($contexts as $context_name => $context) {
      $form[$name]['cvmglobal_' . $name . '_' . $context_name] = array(
        '#type' => 'select',
        '#title' => 'Default view mode for context: ' . $context,
        '#options' => $ds_modes_formatted,
        '#default_value' => (isset($global_cvm[$name][$context_name])) ? $global_cvm[$name][$context_name] : "none",
      );
    }

  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Configuration',
  );

  $form["#submit"][] = "contextual_view_modes_settings_submit";
  $form["#validate"][] = "contextual_view_modes_settings_validate";
  return $form;
}

/**
 * [contextual_view_modes_settings_validate description]
 * @param  [type] $form       [description]
 * @param  [type] $form_state [description]
 * @return [type]             [description]
 */
function contextual_view_modes_settings_validate($form, &$form_state) {


  // Store the values of the submission for later submit hooks.
  foreach ($form_state['values'] as $k => $v) {
    $form_state['storage'][$k] = $v;
  }

  // Compare enabled content types to the form submission.
  $disabled = _has_disabled_types($form_state);

  // There are disabled types and the form hasnt been confirmed
  // Boot the user over to a confirm form.
  if ($disabled
    && !isset($form_state['values']['confirm'])) {
    form_set_value($form['gotoconf'], "true", $form_state);
    $form_state['rebuild'] = TRUE;
    return;
    // We are going to a cofirm form. End here so that additional submit handles
    // do not get added.
  }

  // There are disabled types but the user has confirmed they want to process.
  if ($disabled && !empty($form_state['values']['confirm'])) {
    $form["#submit"][] = "contextual_view_modes_settings_submit";
  }

  // No disabled types lets process and save.
  if (!$disabled) {
    $form["#submit"][] = "contextual_view_modes_settings_submit";
  }


}

/**
 * [contextual_view_modes_settings_submit description]
 * @param  [type] $form       [description]
 * @param  [type] $form_state [description]
 * @return [type]             [description]
 */
function contextual_view_modes_settings_submit($form, &$form_state) {

  $values = array_merge($form_state['values'], $form_state['storage']);
  $disabled_types = array_filter($values['cvm_enabled_content_types'], '_give_zero_values');

  // LOOP THROUGH DISABLED TYPES AND REMOVE FIELD INSTANCES.
  foreach ($disabled_types as $type => $nothing) {
    $cvm_instance = field_read_instance('node', CVM_FIELD_NAME, $type);
    if ($cvm_instance) {
      $instance = _get_cvm_field_instance($type);
      field_delete_instance($instance, FALSE);
    }
  }

  // Remove content types from the list that arent enabled and then
  // store the values of the form.
  $filtered = array_filter($values['cvm_enabled_content_types'], '_remove_zero_values');
  $keys = array_keys($filtered);
  variable_set('cvm_enabled_content_types', $keys);


  // Attach CVM field instances to the enabled content types
  // We now have a list of content types (bundles) that we need to attach an
  // instance of the fields to. Loop through them and attach them.
  $cvm_field = field_info_field(CVM_FIELD_NAME);

  foreach ($keys as $k => $v) {

    $cvm_instance = field_read_instance('node', CVM_FIELD_NAME, $v);
    if (!$cvm_instance) {
      // Create it.
      $instance = _get_cvm_field_instance($v);
      field_create_instance($instance);
    }

  }

  // HANDLE THE DEFAULT VIEW MODE SETTINGS.
  // We want to store these in a nice big arrary instead of individual variables
  // in the systems form. Lets make sure we parse them and unset them form the
  // form state.

  $global_cvm = array();
  foreach ($values as $field_name => $value) {
    if (substr($field_name, 0, 10) == "cvmglobal_") {

      // Remove from form_state.
      unset($form_state['values'][$field_name]);

      // We dont care about no values...
      if ($value == "none") {
        continue;
      }

      $xp = explode("_", $field_name);
      unset($xp[0]); // Dont need this.
      $content_type = $xp[1];
      unset($xp[1]);
      $context_name = implode("_", $xp);

      // If field was disabled we need to not store it.
      if (array_key_exists($content_type, $disabled_types)) {
        continue;
      }

      $global_cvm[$content_type][$context_name] = $value;
    }
  }
  variable_set('cvm_global_content_type_modes', $global_cvm);

  drupal_set_message('Configuration Settings Saved');

  // Clear all the caches.
  drupal_flush_all_caches();
}


// HELPER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * @param  $bundle  The bundle this instance belongs to
 * @return  Array - an array describing the instance of the reference field
 */

function _get_cvm_field_instance($bundle) {

  if (!$bundle) {
    return FALSE;
  }

  $ret = array();
  $ret['entity_type'] = "node";
  $ret['bundle'] = $bundle;
  $ret['field_name'] = CVM_FIELD_NAME;
  $ret['label'] = t('Contextual View Mode');
  $ret['widget']['type'] = "contextual_view_modes_picker";
  $ret['display']['default']['type'] = "hidden";
  $ret['display']['teaser']['type'] = "hidden";

  return $ret;
}


/**
 * _remove_zero_values
 * Helper function to remove items from an assoc array with value == 0
 * @param  $input the input array
 * @return  boolean true if value is valid | false if value is 0
 */

function _remove_zero_values($input) {
  if ($input === 0 ||
     $input === FALSE ||
     strlen($input) == 0 ||
     $input === "0") {
    return FALSE;
  }
  return TRUE;
}


/**
 * _give_zero_values
 * Helper function to remove items from an assoc array with value == 0
 * @param  $input the input array
 * @return  boolean true if value is valid | false if value is 0
 */

function _give_zero_values($input) {
  if ($input === 0 ||
     $input === FALSE ||
     strlen($input) == 0 ||
     $input === "0") {
    return TRUE;
  }
  return FALSE;
}



/**
 * Checks the form_state array against the saved enabled content types
 * for disabled types
 * @param  array  $form_state | The form state_array
 * @return boolean             false if there are no disabled types
 */
function _has_disabled_types($form_state) {

  $enabled_types = variable_get('cvm_enabled_content_types', array());
  $values = array_merge($form_state['values'], $form_state['storage']);
  $filtered = array_filter($values['cvm_enabled_content_types'], '_remove_zero_values');
  $keys = array_keys($filtered);

  // Loop through current enabled types and check for it being enabled in the
  // submitted values.
  foreach ($enabled_types as $k => $name) {
    if (!in_array($name, $keys)) {
      return TRUE;
    }
  }

  // Nothing disabled :)
  return FALSE;
}

