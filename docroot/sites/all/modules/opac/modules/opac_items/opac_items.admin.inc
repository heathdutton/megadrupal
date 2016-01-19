<?php
/**
 * @file
 * This file contains configuration forms for opac items module.
 */

/**
 * Form constructor for OPAC items settings.
 *
 * @see opac_items_settings_form_submit()
 * @ingroup forms
 */
function opac_items_settings_form($form, $form_state) {
  $entity_info = entity_get_info('node');
  $view_modes = $entity_info['view modes'];

  $form['opac_items_options'] = array(
    '#markup' => '<b>' . t('Items options:') . '</b>',
  );

  $form['opac_items_show_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a tab on bibliographic record pages.', array(), array('context' => 'libraries')),
    '#default_value' => variable_get('opac_items_show_tab', 'TRUE'),
  );

  $form['opac_items_hide_no_items'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide items tab when there are no items?', array(), array('context' => 'libraries')),
    '#default_value' => variable_get('opac_items_hide_no_items', 'FALSE'),
  );

  $form['opac_items_hide_availability'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide items availibility', array(), array('context' => 'libraries')),
    '#description' => t('Check this to hide availibility column in items tables'),
    '#default_value' => variable_get('opac_items_hide_availability', FALSE),
  );

  $form['opac_items_display'] = array(
    '#markup' => '<b>' . t('Items display:') . '</b>',
  );
  foreach ($view_modes as $view_mode => $mode) {
    $form["display_settings_$view_mode"] = array(
      '#type' => 'fieldset',
      '#title' => t('@mode display settings', array('@mode' => t($mode['label']))),
      '#states' => array(
        'visible' => array(
          ':input[name="opac_items_in_nodes"]' => array('checked' => TRUE),
        ),
      ),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form["display_settings_$view_mode"]["opac_items_enable_field_$view_mode"] = array(
      '#type' => 'checkbox',
      '#title' => t('Show items in node (dynamic field) for this view mode.', array(), array('context' => 'libraries')),
      '#default_value' => variable_get("opac_items_enable_field_$view_mode", 'FALSE'),
    );

    $form["display_settings_$view_mode"]["opac_items_max_in_field_$view_mode"] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum number of items to display', array(), array('context' => 'libraries')),
      '#default_value' => variable_get("opac_items_max_in_field_$view_mode", 5),
      '#element_validate' => array('element_validate_integer_positive'),
      '#size' => 15,
      '#required' => TRUE,
    );
    $form["display_settings_$view_mode"]["opac_items_fieldset_$view_mode"] = array(
      '#type' => 'radios',
      '#default_value' => variable_get("opac_items_fieldset_$view_mode", 'fieldset'),
      '#title' => t('Items displaying', array(), array('context' => 'libraries')),
      '#options' => array(
        'fieldset' => t('Show items in a collapsible fieldset.', array(), array('context' => 'libraries')),
        'nofieldset' => t('Show items without fieldset', array(), array('context' => 'libraries')),
      ),
    );
    $form["display_settings_$view_mode"]["opac_items_fieldset_title_$view_mode"] = array(
      '#type' => 'textfield',
      '#title' => t('Fieldset title'),
      '#default_value' => variable_get("opac_items_fieldset_title_$view_mode", 'Items'),
      '#description' => t('Chose the title you want in the fieldset legend.'),
      '#size' => 60,
      '#maxlength' => 128,
      '#states' => array(
        'visible' => array(
          ':input[name="opac_items_fieldset"]' => array('value' => 'fieldset'),
        ),
      ),
    );
    $form["display_settings_$view_mode"]["opac_items_fieldset_collapsed_$view_mode"] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get("opac_items_fieldset_collapsed_$view_mode", FALSE),
      '#title' => t('Collapsed ?'),
      '#states' => array(
        'visible' => array(
          ':input[name="opac_items_fieldset"]' => array('value' => 'fieldset'),
        ),
      ),
    );
    $form["display_settings_$view_mode"]["opac_items_title_$view_mode"] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('Type here the label that will appear above items table.', array(), array('context' => 'libraries')),
      '#default_value' => variable_get("opac_items_title_$view_mode", 'Items'),
      '#size' => 60,
      '#maxlength' => 128,
      '#states' => array(
        'visible' => array(
          ':input[name="opac_items_fieldset"]' => array('value' => 'nofieldset'),
        ),
      ),
    );
  }
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Form validation handler for opac_items_settings_form().
 */
function opac_items_settings_form_validate($form, $form_state) {
}

/**
 * Form submission handler for opac_items_settings_form().
 */
function opac_items_settings_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $entity_info = entity_get_info('node');
  $view_modes = $entity_info['view modes'];

  variable_set('opac_items_show_tab', $values['opac_items_show_tab']);
  variable_set('opac_items_hide_no_items', $values['opac_items_hide_no_items']);
  variable_set('opac_items_hide_availability', $values['opac_items_hide_availability']);

  foreach ($view_modes as $view_mode => $mode) {
    variable_set("opac_items_enable_field_$view_mode", $values["opac_items_enable_field_$view_mode"]);
    variable_set("opac_items_max_in_field_$view_mode", $values["opac_items_max_in_field_$view_mode"]);
    variable_set("opac_items_fieldset_$view_mode", $values["opac_items_fieldset_$view_mode"]);
    variable_set("opac_items_fieldset_title_$view_mode", $values["opac_items_fieldset_title_$view_mode"]);
    variable_set("opac_items_fieldset_collapsed_$view_mode", $values["opac_items_fieldset_collapsed_$view_mode"]);
    variable_set("opac_items_title_$view_mode", $values["opac_items_title_$view_mode"]);
  }

  menu_rebuild();
  drupal_set_message(t('Menu rebuilt'));
}

/**
 * Form constructor for OPAC items server settings.
 */
function opac_items_server_settings_form($form, $form_state, $serv_id) {
  $server = opac_server_load($serv_id);

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'opac_items') . '/js/server_settings_form.js',
  );

  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $server->serv_id,
  );

  $settings = variable_get("opac_items_server_settings_$serv_id");
  $and_or = $settings ? $settings['and_or'] : NULL;
  $criteria = $settings ? $settings['criteria'] : NULL;

  $form['and_or'] = array(
    '#type' => 'select',
    '#title' => t('Hide @server_name items if', array('@server_name' => $server->serv_name), array('context' => 'libraries')),
    '#options' => array(
      'and' => t('all the selected criteria are met'),
      'or' => t('at least one of the selected criteria is met'),
    ),
    '#default_value' => $and_or ? $and_or : 'and',
  );

  $connector = opac_get_instance($server);
  $item_fields = $connector->itemFields();
  $options = array();
  $form['criteria'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#theme' => 'opac_items_server_settings_table',
  );
  foreach ($item_fields as $fieldname => $item_field) {
    $crit = (isset($criteria) && isset($criteria[$fieldname])) ? $criteria[$fieldname] : NULL;
    $form['criteria'][$fieldname]['use'] = array(
      '#type' => 'checkbox',
      '#default_value' => $crit ? $crit['use'] : 0,
      '#attributes' => array(
        'class' => array('criteria-use'),
      ),
    );
    $form['criteria'][$fieldname]['field'] = array(
      '#markup' => $item_field['label'],
    );
    $form['criteria'][$fieldname]['operator'] = array(
      '#type' => 'select',
      '#options' => array(
        'eq' => t('is equal to'),
        'ne' => t('is not equal to'),
        '==' => t('is equal to (numeric)'),
        '!=' => t('is not equal to (numeric)'),
        '>' => t('is greater than (numeric)'),
        '<' => t('is lesser than (numeric)'),
        'match' => t('matches (regex)'),
        'not match' => t('does not match (regex)'),
      ),
      '#default_value' => $crit ? $crit['operator'] : 'equals',
      '#attributes' => array(
        'class' => array('criteria-operator'),
      ),
    );
    $form['criteria'][$fieldname]['value'] = array(
      '#type' => 'textfield',
      '#default_value' => $crit ? $crit['value'] : '',
      '#attributes' => array(
        'class' => array('criteria-value'),
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate callback for opac_items_server_settings_form.
 */
function opac_items_server_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $numeric_msg = "Value for @fieldname must be numeric.";
  $regex_msg = "Value for @fieldname is not a valid regular expression. Don't forget to add '/' at the beginning and the end of expression.";

  foreach ($values['criteria'] as $fieldname => $criterion) {
    if ($criterion['use']) {
      switch ($criterion['operator']) {
        case '==':
        case '!=':
        case '>':
        case '<':
          if (!is_numeric($criterion['value'])) {
            form_set_error("criteria][$fieldname][value",
              t($numeric_msg, array('@fieldname' => $fieldname)));
          }
          break;

        case 'match':
        case 'not match':
          if (@preg_match($criterion['value'], '') === FALSE) {
            form_set_error("criteria][$fieldname][value",
              t($regex_msg, array('@fieldname' => $fieldname)));
          }
          break;
      }
    }
  }
}

/**
 * Submit callback for opac_items_server_settings_form.
 */
function opac_items_server_settings_form_submit($form, $form_state) {
  $values = $form_state['values'];

  $serv_id = $values['serv_id'];

  variable_set("opac_items_server_settings_$serv_id", array(
    'and_or' => $values['and_or'],
    'criteria' => $values['criteria'],
  ));

  drupal_set_message(t('Your settings have been saved.'));
}

/**
 * Form constructor for the items mapping overview form.
 *
 * @param string $serv_id
 *   ILS server machine name for which displaying mapping.
 *
 * @see opac_items_mapping_overview_form_submit()
 * @ingroup forms
 */
function opac_items_mapping_overview_form($form, $form_state, $serv_id) {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");

  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $serv_id,
  );
  $form['add'] = array(
    '#type' => 'link',
    '#title' => '+ ' . t('Add rule'),
    '#href' => "admin/config/opac/items/fields/add/$serv_id",
    '#weight' => -15,
  );
  $form['markup'] = array('#markup' => '<br /><br />');

  // Get all mapped fields to dysplay them.
  $mapping = opac_items_get_fields_mapping($serv_id);
  $form['#tree'] = TRUE;
  foreach ($mapping as $rule) {
    $key = $rule['item_field'];
    $form[$key]['item_field'] = array('#markup' => check_plain($rule['item_field']));
    $form[$key]['display_header'] = array('#markup' => check_plain($rule['display_header']));
    $form[$key]['vocabulary_name'] = array(
      '#markup' => $rule['vocabulary_machine_name'] ?
      l($rule['vocabulary_name'], 'admin/structure/taxonomy/' . $rule['vocabulary_machine_name'] . '/edit')
      . ' - ' . l(t('view terms list'), 'admin/structure/taxonomy/' . $rule['vocabulary_machine_name']) : '',
    );

    $term_string = '';
    if ($rule['term_field'] == 'name') {
      $term_string = 'Term name';
    }
    elseif ($rule['term_field'] != NULL) {
      $term_string = l($rule['term_field_label'] . ' (' . $rule['term_field'] . ')', 'admin/structure/taxonomy/'
      . $rule['vocabulary_machine_name'] . '/fields/' . $rule['term_field']);
    }
    $form[$key]['term_field'] = array('#markup' => $term_string);

    if ($rule['nomatch_rule'] != NULL) {
      $form[$key]['nomatch_rule'] = array(
        '#markup' => $rule['nomatch_rule'] == 'none' ? t('Leave empty') : t('Create new term'),
      );
    }

    $form[$key]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $rule['item_field'])),
      '#title_display' => 'invisible',
      '#delta' => 10,
      '#default_value' => $rule['weight'],
    );
    $form[$key]['edit'] = array(
      '#type' => 'link',
      '#title' => t('Edit'),
      '#href' => "admin/config/opac/items/fields/edit/$serv_id/" . $rule['item_field'],
    );
    $form[$key]['delete'] = array(
      '#type' => 'link',
      '#title' => t('Delete'),
      '#href' => "admin/config/opac/items/fields/delete/$serv_id/" . $rule['item_field'],
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Form submission handler for opac_items_mapping_overview_form().
 */
function opac_items_mapping_overview_form_submit($form, $form_state) {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");
  $values = $form_state['values'];
  $serv_id = $values['serv_id'];
  // Loop through mapped fields to check their new weight.
  foreach (element_children($values) as $key) {
    if (isset($values[$key]['weight'])) {
      opac_items_field_mapping_mod_weight($serv_id, $key, $values[$key]['weight']);
    }
  }
}

/**
 * Form constructor for the items mapping edit/add form.
 *
 * @param string $serv_id
 *   ILS server machine name for which displaying mapping.
 *
 * @param string $item_field
 *   Existing item field mapping.
 *
 * @see opac_items_admin_field_edit_form_validate()
 * @see opac_items_admin_field_edit_form_submit()
 * @ingroup forms
 */
function opac_items_admin_field_edit_form($form, $form_state, $serv_id, $item_field = NULL) {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");
  module_load_include('inc', 'opac', "includes/opac.db");

  $mapping = array();
  $new = TRUE;
  // If we are editing an existing mapping, we retrieve it.
  if ($item_field) {
    $mapping = opac_items_get_field_mapping($serv_id, $item_field);
    $new = FALSE;
  }

  // Create an instance of the connector and
  // get all available fields via itemFields method.
  $server = opac_server_load($serv_id);
  $connector = opac_get_instance($server);
  $connector_fields = $connector->itemFields();
  // Build fields options for item_field dropdown list
  // with 'none' default choice.
  $fields_options = array('none' => t('Select a field'));
  foreach ($connector_fields as $fieldid => $field) {
    $fields_options[$fieldid] = $field['label'];
  }
  // For a new one, remove fields that are
  // already mapped.
  if ($new) {
    $existing = opac_items_get_fields_mapping($serv_id);
    foreach ($existing as $rule) {
      unset($fields_options[$rule['item_field']]);
    }
  }

  $form['new'] = array(
    '#type' => 'hidden',
    '#value' => $new,
  );
  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $serv_id,
  );
  $form['item_field'] = array(
    '#type' => 'select',
    '#title' => t('Item field', array(), array('context' => 'libraries')),
    '#default_value' => isset($mapping['item_field']) ? $mapping['item_field'] : 'none',
    '#options' => $fields_options,
    '#disabled' => !$new,
    '#description' => t("Item field name. This is the field the connector will return. i.e 'location'.", array(), array('context' => 'libraries')),
    '#ajax' => array(
      'callback' => 'opac_items_field_description_callback',
      'wrapper' => 'replace_description_field',
    ),
  );
  if (!isset($mapping['item_field'])) {
    $mapping['item_field'] = 'none';
  }
  $form['field_description'] = array(
    // Populate this form element with field description.
    // The form is rebuilt during ajax processing.
    '#markup' => isset($form_state['values']) ?
    opac_items_get_field_description($serv_id, $form_state['values']['item_field']) :
    opac_items_get_field_description($serv_id, $mapping['item_field']),
    '#prefix' => '<div id="replace_description_field">',
    '#suffix' => '</div>',
  );
  $form['display_header'] = array(
    '#type' => 'textfield',
    '#size' => 25,
    '#title' => t('Displayed name'),
    '#default_value' => isset($mapping['display_header']) ? $mapping['display_header'] : '',
    '#description' => t('The value which will be displayed in table header. i.e "Item Location".', array(), array('context' => 'libraries')),
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => isset($mapping['weight']) ? $mapping['weight'] : 0,
    '#delta' => 10,
    '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.', array(), array('context' => 'libraries')),
  );

  // Taxonomy options.
  // Get existing vocabularies.
  $vocabularies = taxonomy_get_vocabularies();

  // Build vocabularies options for the select element.
  // And keep vocabulary machine name.
  $vocabularies_options = array(0 => t('None'));
  foreach ($vocabularies as $vocabulary) {
    $vocabularies_options[$vocabulary->machine_name] = $vocabulary->name;
  }

  $form['has_taxonomy'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($mapping['vocabulary_machine_name']) ? 1 : 0,
    '#title' => t('linked to a vocabulary'),
    '#description' => t('Is this field linked to taxonomy/vocablary.'),
  );
  $form['taxonomy_rule'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy rule'),
    '#attributes' => array('class' => array('container-inline')),
    '#states' => array(
      'visible' => array(
        ':input[name="has_taxonomy"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['taxonomy_rule']['vocabulary_machine_name'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#default_value' => isset($mapping['vocabulary_machine_name']) ? $mapping['vocabulary_machine_name'] : 0,
    '#options' => $vocabularies_options,
    '#ajax' => array(
      'callback' => 'opac_items_vocabulary_fields_callback',
      'wrapper' => 'replace_fields_select',
    ),
  );

  // Get fields options.
  $fields_options = array();
  if (isset($form_state['values']) && $form_state['values']['vocabulary_machine_name'] != '0') {
    $current_vocabulary = $form_state['values']['vocabulary_machine_name'];
    $fields_options = opac_items_get_vocabulary_fields($current_vocabulary);
  }
  if (isset($mapping['term_field'])) {
    $fields_options = opac_items_get_vocabulary_fields($mapping['vocabulary_machine_name']);
  }

  $form['taxonomy_rule']['term_field'] = array(
    '#type' => 'select',
    '#title' => t('Term field to compare'),
    '#default_value' => isset($mapping['term_field']) ? $mapping['term_field'] : 'none',
    '#options' => $fields_options,
    '#prefix' => '<div id="replace_fields_select">',
    '#suffix' => '</div>',
    '#states' => array(
      'invisible' => array(
        ':input[name="vocabulary"]' => array('value' => 0),
      ),
    ),
  );
  $form['taxonomy_rule']['nomatch_rule'] = array(
    '#title' => t('No match rule'),
    '#type' => 'select',
    '#options' => array('none' => 'Leave empty', 'create' => 'Create new term'),
    '#default_value' => isset($mapping['nomatch_rule']) ? $mapping['nomatch_rule'] : 'none',
    '#states' => array(
      'invisible' => array(
        ':input[name="vocabulary"]' => array('value' => 0),
      ),
    ),
  );

  $form['taxonomy_rule']['show_as_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show term as link.'),
    '#prefix' => '<br /><br />',
    '#default_value' => isset($mapping['show_as_link']) ? $mapping['show_as_link'] : 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Callback for opac_items_admin_field_edit_form.
 */
function opac_items_vocabulary_fields_callback($form, &$form_state) {
  return $form['taxonomy_rule']['term_field'];
}

/**
 * Get fields attached to vocabulary.
 *
 * @param string $vocabulary_machine_name
 *   Vocabulary machine name.
 *
 * @return array
 *   All the vocabulary fields.
 */
function opac_items_get_vocabulary_fields($vocabulary_machine_name) {
  $fields_options = array();
  if ($vocabulary_machine_name) {
    $fields_options['name'] = 'Term name';
    $fields = field_info_instances('taxonomy_term', $vocabulary_machine_name);
    foreach ($fields as $name => $field) {
      $fields_options[$name] = $field['label'];
    }
  }
  return $fields_options;
}

/**
 * Callback for opac_items_admin_field_edit_form.
 */
function opac_items_field_description_callback($form, &$form_state) {
  return $form['field_description'];
}

/**
 * Get field description from the connector used by the ILS server.
 *
 * @param string $serv_id
 *   ILS server machine name.
 *
 * @param string $item_field
 *   Name of the item field.
 *
 * @return string
 *   html content.
 */
function opac_items_get_field_description($serv_id, $item_field) {
  if ($item_field == 'none') {
    $return = '<div class="descriptionr">';
    $return .= t('Select an item field to be displayed.', array('context' => 'libraries'));
    $return .= '</div>';
    return $return;
  }
  // Create an instance of the connector.
  $server = opac_server_load($serv_id);
  $connector = opac_get_instance($server);
  // Call itemFields method.
  $connector_fields = $connector->itemFields();
  // Get label and description of item_field.
  $label = $connector_fields[$item_field]['label'];
  $description = $connector_fields[$item_field]['description'];

  $return = '<div><strong>';
  $return .= t('Description of @label : ', array('@label' => $label));
  $return .= '</strong><div class="description">';
  $return .= t($description);
  $return .= '</div></div>';
  return $return;

}

/**
 * Form validation handler for opac_items_admin_field_edit_form().
 *
 * @see opac_items_admin_field_edit_form_submit()
 */
function opac_items_admin_field_edit_form_validate($form, $form_state) {
  $values = $form_state['values'];
  // Check empty values.
  if ($values['item_field'] == 'none' || !$values['display_header']) {
    form_set_error('items_admin_field_edit_form', t('Some required fields are missing'));
  }
  if ($values['has_taxonomy'] != 0 && !$values['vocabulary_machine_name']) {
    form_set_error('items_admin_field_edit_form', t('Some required taxonomy fields are missing'));
  }
}

/**
 * Form submission handler for opac_items_admin_field_edit_form().
 *
 * @see opac_items_admin_field_edit_form_validate()
 */
function opac_items_admin_field_edit_form_submit($form, $form_state) {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");
  $values = $form_state['values'];

  if ($values['has_taxonomy']) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($values['vocabulary_machine_name']);
    $values['vocabulary_name'] = $vocabulary->name;

    $vocabulary_fields = field_info_instances('taxonomy_term', $values['vocabulary_machine_name']);
    $values['term_field_label'] = $vocabulary_fields[$values['term_field']]['label'];
  }
  else {
    $values['vocabulary_machine_name'] = NULL;
    $values['vocabulary_name'] = NULL;
    $values['term_field_label'] = NULL;
    $values['term_field'] = NULL;
    $values['nomatch_rule'] = NULL;
  }

  // Add or modify submitted mapping.
  if ($values['new']) {
    opac_items_field_mapping_add($values);
  }
  else {
    opac_items_field_mapping_mod($values);
  }
  drupal_goto('admin/config/opac/servers/manage/' . $values['serv_id'] . '/items-mapping');
}

/**
 * Form constructor which is a confirmation step when deleting item mapping.
 *
 * @param string $serv_id
 *   ILS server machine name for which displaying mapping.
 *
 * @param string $item_field
 *   Existing item field mapping.
 *
 * @see opac_items_admin_field_delete_confirm_form_submit()
 * @ingroup forms
 */
function opac_items_admin_field_delete_confirm_form($form, $form_state, $serv_id, $item_field) {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");
  $mapping = opac_items_get_field_mapping($serv_id, $item_field);
  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $serv_id,
  );
  $form['item_field'] = array(
    '#type' => 'hidden',
    '#value' => $item_field,
  );
  $form['infos'] = array(
    '#markup' => t('Are you sure you want delete this mapping: "@item_field => @display_header" ?', array('@item_field' => $mapping['item_field'], '@display_header' => $mapping['display_header'])),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['confirm'] = array('#type' => 'submit', '#value' => t('Confirm'));
  $form['actions']['cancel'] = array('#type' => 'submit', '#value' => t('Cancel'));
  return $form;
}

/**
 * Form submission handler for opac_items_admin_field_delete_confirm_form().
 */
function opac_items_admin_field_delete_confirm_form_submit($form, &$form_state) {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");
  $values = $form_state['values'];

  // Delete mapping if the clicked button equals 'confirm'.
  if ($form_state['clicked_button']['#value'] == $values['confirm']) {
    opac_items_field_mapping_del($values['serv_id'], $values['item_field']);
  }
  drupal_goto('admin/config/opac/servers/manage/' . $values['serv_id'] . '/items-mapping');
}
