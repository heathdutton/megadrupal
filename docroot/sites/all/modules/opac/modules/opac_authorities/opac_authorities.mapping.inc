<?php
/**
 * @file
 * Contains function related to authorities mapping building.
 */

/**
 * Form constructor for authority types / server form.
 *
 * @ingroup forms
 */
function opac_authorities_admin_authority_types_overview_form($form, &$form_state, $opac_server) {
  module_load_include('inc', 'opac', "includes/opac.db");
  module_load_include('inc', 'opac_authorities', "opac_authorities.db");
  // Get all authority types for $opac_server.
  $server_types = opac_authorities_get_server_types($opac_server->serv_id);

  // Get connector fields.
  $connector = opac_get_instance($opac_server);
  $connector_fields = $connector->authFields();

  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $opac_server->serv_id,
  );

  $form['#tree'] = TRUE;
  foreach ($server_types as $name => $type) {
    $authority_type = opac_authority_type_load($name);
    $form[$name]['authtype'] = array('#markup' => check_plain($authority_type->label));
    $form[$name]['matching_field'] = array('#markup' => check_plain($connector_fields[$type['matching_field']]['label']));
    $form[$name]['value'] = array('#markup' => check_plain($type['value']));

    $form[$name]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $type['authtype'])),
      '#title_display' => 'invisible',
      '#delta' => 10,
      '#default_value' => $type['weight'],
    );
    $form[$name]['edit'] = array(
      '#type' => 'link',
      '#title' => t('Edit'),
      '#href' => 'admin/config/opac/servers/manage/' . $opac_server->serv_id . '/authority-types/' . $type['authtype'] . '/edit',
    );
    $form[$name]['mapping'] = array(
      '#type' => 'link',
      '#title' => t('Mapping', array(), array('context' => 'libraries')),
      '#href' => 'admin/config/opac/servers/manage/' . $opac_server->serv_id . '/authority-mapping/' . $type['authtype'],
    );
    $form[$name]['delete'] = array(
      '#type' => 'link',
      '#title' => t('Delete'),
      '#href' => 'admin/config/opac/servers/manage/' . $opac_server->serv_id . '/authority-types/' . $type['authtype'] . '/delete',
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Form constructor for opac authority types edit form.
 *
 * @param object $opac_server
 *   ILS server.
 *
 * @param string $authority_name
 *   Authority type.
 *
 * @see opac_authorities_server_type_edit_form_submit()
 * @ingroup forms
 */
function opac_authorities_server_type_edit_form($form, $form_state, $opac_server, $authority_name = NULL) {
  $authority_types = opac_authority_types();
  if (!count($authority_types)) {
    $form['no_available_authtype'] = array(
      '#markup' => t('No available authority type for now. You can create one at Admin > Structure > Authority Types.', array(), array('context' => 'libraries')),
    );
    return $form;
  }
  $types_options = array('' => 'None');
  foreach ($authority_types as $id => $type) {
    $types_options[$id] = $type->label;
  }

  // Get connector fields.
  $connector = opac_get_instance($opac_server);
  $connector_fields = $connector->authFields();
  $matching_fields = array('' => 'None');
  foreach ($connector_fields as $name => $field) {
    $matching_fields[$name] = $field['label'];
  }

  $server_types = array();
  $default_matching_field = '';
  $default_value = '';
  $default_weight = '';
  if ($authority_name) {
    module_load_include('inc', 'opac_authorities', "opac_authorities.db");
    $server_types = opac_authorities_get_server_types($opac_server->serv_id);
    $default_matching_field = $server_types[$authority_name]['matching_field'];
    $default_value = $server_types[$authority_name]['value'];
    $default_weight = $server_types[$authority_name]['weight'];
    $form['edit'] = array('#type' => 'hidden', '#value' => 1);
  }

  $form['serv_id'] = array('#type' => 'hidden', '#value' => $opac_server->serv_id);
  $form['authtype'] = array(
    '#type' => 'select',
    '#title' => t('Authority type', array(), array('context' => 'libraries')),
    '#options' => $types_options,
    '#default_value' => $authority_name ? $authority_name : '',
    '#description' => t('Add an authority type for @opac_server server.', array('@opac_server' => $opac_server->serv_name)),
    '#required' => TRUE,
    '#disabled' => $authority_name ? TRUE : FALSE,
  );

  $form['matching_field'] = array(
    '#type' => 'select',
    '#title' => t("Matching field", array(), array('context' => 'libraries')),
    '#description' => t("The field which will be used to match this content type"),
    // Retrieve all fields for opac_node_type value changes and then
    // the form is rebuilt during ajax processing.
    '#options' => $matching_fields,
    '#default_value' => $default_matching_field ? $default_matching_field : '',
    '#required' => TRUE,
    '#prefix' => '<div id="replace_fields_choice_div">',
    '#suffix' => '</div>',
  );
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Field value(s)'),
    '#description' => t("Enter here value(s) that should match the selected field."),
    '#default_value' => $default_value ? $default_value : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $default_weight ? $default_weight : 0,
    '#delta' => 10,
  );
  $form['save'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

/**
 * Form submission handler for opac_authorities_server_type_edit_form().
 */
function opac_authorities_server_type_edit_form_submit($form, $form_state) {
  module_load_include('inc', 'opac_authorities', "opac_authorities.db");
  $values = $form_state['values'];
  $serv_id = $values['serv_id'];

  if (isset($values['edit'])) {
    // Change a authority/server type.
    opac_authorities_server_type_edit($values);
  }
  else {
    // Save a new authority/server type.
    opac_authorities_server_type_add($values);
  }

  drupal_goto("admin/config/opac/servers/manage/$serv_id/authority-types");
}

/**
 * Form confirmation for delete opac authority/server type.
 *
 * @param object $opac_server
 *   ILS server.
 *
 * @param string $machine_name
 *   Authority type machine name.
 *
 * @see opac_authorities_server_type_delete_confirm_submit()
 * @ingroup forms
 */
function opac_authorities_server_type_delete_confirm($form, &$form_state, $opac_server, $machine_name) {
  $form['serv_id'] = array(
    '#type' => 'value',
    '#value' => $opac_server->serv_id,
  );
  $form['authtype'] = array(
    '#type' => 'value',
    '#value' => $machine_name,
  );

  return confirm_form($form,
  t('Are you sure you want to delete this authority type for @server?', array('@server' => $opac_server->serv_name)),
  isset($_GET['destination']) ? $_GET['destination'] : "admin/config/opac/servers/manage/$opac_server->serv_id/authority-types",
  t('This action cannot be undone.'),
  t('Delete'),
  t('Cancel'));
}

/**
 * Form submission handler for opac_authorities_server_type_delete_confirm().
 */
function opac_authorities_server_type_delete_confirm_submit($form, &$form_state) {
  $values = $form_state['values'];
  $serv_id = $values['serv_id'];
  if ($form_state['values']['confirm']) {
    module_load_include('inc', 'opac_authorities', "opac_authorities.db");
    opac_authorities_server_type_delete($values);
    drupal_set_message(t('Authority type has been deleted successfully.', array(), array('context' => 'libraries')));
  }
  drupal_goto("admin/config/opac/servers/manage/$serv_id/authority-types");
}

/**
 * Form constructor for authority mapping form.
 *
 * @param object $opac_server
 *   ILS server.
 *
 * @param string $authority_name
 *   Content type machine name.
 *
 * @see opac_authorities_mapping_form_submit()
 * @ingroup forms
 */
function opac_authorities_mapping_form($form, &$form_state, $opac_server, $authority_name) {
  module_load_include('inc', 'opac_authorities', "opac_authorities.db");

  // Get a list of mapped and unmapped fields
  // for the current server.
  $mapping = opac_authorities_build_fields_mapping($opac_server->serv_id, $authority_name);

  // Retrieves server values and create an
  // instance of the corresponding connector.
  // So we can call biblioFields method to
  // to get ILS available fields.
  $connector = opac_get_instance($opac_server);
  $connector_fields = $connector->authFields();

  $header = array(
    'authtype' => 'Authority type',
    'authority_field' => 'Authority field',
    'mapped_with' => 'Mapped with',
    'vocabulary' => 'Vocabulary',
    'term_field' => 'Term field to match',
    'nomatch_rule' => 'No match rule',
    'operations' => 'Operations',
  );

  // This section is for fields that already
  // have been mapped.
  $data = array();
  foreach ($mapping['mapped'] as $values) {
    // The label of the cuurent field.
    $auth_label = $values['authority_field_label'];
    // The machine name of the cuurent field.
    $auth_name = $values['authority_field_name'];
    $authtype = $values['authtype'];

    // No link for title field because
    // we can't edit the title.
    if ($auth_name != 'title') {
      // This provide a link to field settings page.
      $field_link = l(t("@auth_label (@auth_name)", array('@auth_label' => $auth_label, '@auth_name' => $auth_name)), "admin/structure/authority-types/manage/$authtype/fields/$auth_name");
    }
    else {
      $field_link = t('Title (@auth_name)', array('@auth_name' => $auth_label));
    }

    // The connector_label var is the label for
    // ILS/connector field.
    $connector_label = $connector_fields[$values['mapped_with']]['label'];
    $connector_name = $values['mapped_with'];
    $connector_html = "$connector_label ($connector_name)";

    $field_vocabulary = '';
    $nomatch_rule = '';
    $term_field = '';
    // Node fields can be related to vocabulary.
    // This informations are added in 'VOCABULARY' column.
    if (isset($values['vocabulary_machine_name'])) {
      $vocabulary_machine_name = $values['vocabulary_machine_name'];
      $vocabulary_name = $values['vocabulary_name'];
      // Provide a link to edit vocabulary.
      $field_vocabulary = l(t($vocabulary_name), "admin/structure/taxonomy/$vocabulary_machine_name/edit");
      // Also provide a link to view terms list.
      $field_vocabulary .= "  -  " . l(t('view terms list'), "admin/structure/taxonomy/$vocabulary_machine_name");
      // Nomatch_rule is a mapping options telling
      // what to do when harvesting biblios and opac module
      // attempts to match field values with existing terms.
      // So we show this option in mapping overview.
      $nomatch_rule = $values['nomatch_rule'] == 'create' ? "Create new term" : "Leave empty";

      if ($values['term_field'] == 'name') {
        $term_field = 'Term name textfield';
      }
      else {
        $term_field = l($values['term_field_label'] . " (" . $values['term_field_label'] . ")",
          "admin/structure/taxonomy/$vocabulary_machine_name/fields/" . $values['term_field']);
      }
    }

    $row['authtype'] = $authority_name;
    $row['authority_field'] = $field_link;
    $row['mapped_with'] = $connector_html;
    $row['vocabulary'] = $field_vocabulary;
    $row['term_field'] = $term_field;
    $row['nomatch_rule'] = $nomatch_rule;
    $row['operations'] = $values['operations'];

    $key = $authority_name . ' ' . $auth_name;

    $data[$key] = $row;
  }

  $info = t('<h2>You are editing mapping for the authority type <strong>@authtype</strong> and server @opac_server.</h2>', array('@authtype' => $authority_name, '@opac_server' => $opac_server->serv_id));
  $form['info'] = array(
    '#markup' => $info,
  );

  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $opac_server->serv_id,
  );
  $form['fields_mapped'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $data,
    '#empty' => t('No fields mapped yet', array(), array('context' => 'libraries')),
  );

  if (count($mapping['mapped'])) {
    $form['unmap'] = array(
      '#type' => 'submit',
      '#value' => t('Unmap', array(), array('context' => 'libraries')),
    );
  }
  // Section for unmapped field.
  $form['fieldset_unmapped'] = array(
    '#type' => 'fieldset',
    '#title' => t('View unmapped Fields', array(), array('context' => 'libraries')),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['fieldset_unmapped']['fields_unmapped'] = array(
    '#markup' => theme('opac_authorities_mapping_overview', array('mapping' => $mapping['unmapped'], 'serv_id' => $opac_server->serv_id)),
  );

  return $form;
}

/**
 * Form submission handler for opac_authorities_mapping_form().
 */
function opac_authorities_mapping_form_submit($form, &$form_state) {
  module_load_include('inc', 'opac_authorities', "opac_authorities.db");
  $values = $form_state['values'];
  // Loop through all checked fields to unmap them.
  foreach ($values['fields_mapped'] as $key => $delete) {
    if ($delete) {
      list ($authtype, $auth_field_name) = explode(' ', $key);
      opac_authorities_mapping_del(
        array(
          'serv_id' => $values['serv_id'],
          'authtype' => $authtype,
          'authority_field_name' => $auth_field_name,
        )
      );
    }
  }
  // Clear cache related to opac_authorities_fields_mapping.
  cache_clear_all('opac_authorities_fields_mapping', 'cache', TRUE);
}

/**
 * Form constructor for mapping editing/adding form.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $authority_type_name
 *   Authority type name.
 *
 * @param string $fieldname
 *   Field name to map.
 *
 * @see opac_admin_server_mapping_edit_form_submit()
 * @ingroup forms
 */
function opac_authorities_admin_server_mapping_edit_form($form, &$form_state, $serv_id, $authority_type_name, $fieldname) {
  module_load_include('inc', 'opac_authorities', "opac_authorities.db");
  module_load_include('inc', 'opac', "includes/opac.db");

  // Get mapping information for $fieldname and $serv_id.
  $mapping = opac_authorities_build_fields_mapping($serv_id, $authority_type_name);

  // Are we editing a mapped or unmapped field?
  $new = isset($mapping['mapped'][$fieldname]) ? 0 : 1;

  // Retrieves server values and create an
  // instance of the corresponding connector.
  // So we can call authFields method to
  // to get ILS available fields.
  $server = opac_server_load($serv_id);
  $connector = opac_get_instance($server);
  $connector_fields = $connector->authFields();
  $fields_options = array();
  foreach ($connector_fields as $fieldid => $field) {
    $fields_options[$fieldid] = $field['label'];
  }
  // Retrives more informations about the current field.
  $field_settings = field_info_field($fieldname);

  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $serv_id,
  );
  $form['authtype'] = array(
    '#type' => 'hidden',
    '#value' => $authority_type_name,
  );
  $form['new'] = array(
    '#type' => 'hidden',
    '#value' => $new,
  );
  $form['authority_field_name'] = array(
    '#type' => 'hidden',
    '#value' => $fieldname,
  );

  $auth_field_label = $new ? $mapping['unmapped'][$fieldname]['label'] : $mapping['mapped'][$fieldname]['authority_field_label'];
  $form['authority_field_label'] = array(
    '#type' => 'hidden',
    '#value' => $auth_field_label,
  );

  if ($fieldname != 'title') {
    // Here is a message to the user on how to use this form.
    $auth_field_link = l(t("$auth_field_label ($fieldname)",
      array("@auth_field_label" => $auth_field_label, "@fieldname" => $fieldname)), "admin/structure/authority-types/manage/$authority_type_name/fields/$fieldname");
  }
  else {
    $auth_field_link = t('Title (@fieldname)', array('@fieldname' => $fieldname));
  }
  $html = '<div>' . t('Editing mapping for authority field @auth_field_link.', array('@auth_field_link' => $auth_field_link));
  $html .= '<span>' . t('Be careful of multivalued fields. Some ILS fields send back more than one value.') . '<br />';
  $html .= t('Authority fields that accept only one value and are mapped with a multivalued field will contain only the first one.') . '<br />';
  $html .= t('So if it is, you may edit this authority field to make it accept multiple values.') . '</span></div>';
  $form['mapping_information'] = array(
    '#markup' => $html,
  );

  $field_mapped_with = $new ? NULL : $mapping['mapped'][$fieldname]['mapped_with'];
  $form['mapping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mapping', array(), array('context' => 'libraries')),
  );
  // Build a list with all available ILS/connector fields.
  $form['mapping']['mapped_with'] = array(
    '#type' => 'select',
    '#title' => t('Mapped with', array(), array('context' => 'libraries')),
    '#default_value' => $field_mapped_with,
    '#options' => $fields_options,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'opac_authorities_mapped_with_description_callback',
      'wrapper' => 'replace_description_field',
    ),
  );

  // Connectors should provide a description
  // for each fields it proposes.
  if (isset($form_state['values'])) {
    $description = opac_authorities_get_field_descrition($form_state['values']['serv_id'], $form_state['values']['mapped_with']);
  }
  else {
    $description = opac_authorities_get_field_descrition($serv_id, $field_mapped_with);
  }
  $form['mapping']['field_description'] = array(
    // Populate this form element with field description.
    // The form is rebuilt during ajax processing.
    '#markup' => $description,
    '#prefix' => '<div id="replace_description_field">',
    '#suffix' => '</div>',
  );

  // Provide additional form elements if
  // current node field to map is related
  // to a vocabuary.
  if ($field_settings['type'] == 'taxonomy_term_reference') {
    $vocabulary_machine_name = $field_settings['settings']['allowed_values'][0]['vocabulary'];
    $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_machine_name);
    $vocabulary_name = $vocabulary->name;

    $vocabulary_fields = field_info_instances('taxonomy_term', $vocabulary_machine_name);
    $vocabulary_fields_opt = array('none' => t('select a term field'), 'name' => t('Term name'));
    foreach ($vocabulary_fields as $name => $field) {
      $vocabulary_fields_opt[$name] = $field['label'];
    }

    $form['vocabulary_machine_name'] = array(
      '#type' => 'hidden',
      '#value' => $vocabulary_machine_name,
    );
    $form['vocabulary_name'] = array(
      '#type' => 'hidden',
      '#value' => $vocabulary_name,
    );
    $form['taxonomy_rule'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy rule for vocabulary @vocabulary_name', array('@vocabulary_name' => $vocabulary_name)),
      '#attributes' => array('class' => array('container-inline')),
    );
    $form['taxonomy_rule']['term_field'] = array(
      '#type' => 'select',
      '#title' => t('Term field to compare', array(), array('context' => 'libraries')),
      '#default_value' => $new ? 'none' : $mapping['mapped'][$fieldname]['term_field'],
      '#options' => $vocabulary_fields_opt,
    );
    $form['taxonomy_rule']['nomatch_rule'] = array(
      '#title' => t('No match rule'),
      '#type' => 'select',
      '#options' => array('none' => t('Leave empty'), 'create' => t('Create new term')),
      '#default_value' => $new ? 'none' : $mapping['mapped'][$fieldname]['nomatch_rule'],
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Get a biblio field description from connector.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $name
 *   field name for which get the description.
 *
 * @return string
 *   HTML output.
 */
function opac_authorities_get_field_descrition($serv_id, $name) {
  if (!$name) {
    return '<div class="description">' . t('Select a field to be mapped with current authority field.') . '</div>';
  }
  $server = opac_server_load($serv_id);
  $connector = opac_get_instance($server);
  $connector_fields = $connector->authFields();
  $label = $connector_fields[$name]['label'];
  $description = $connector_fields[$name]['description'];

  $return = '<div><strong>' . t('Description of @label field: ', array('@label' => $label));
  $return .= '</strong><div class="description">' . t($description) . '</div>';
  return $return;
}

/**
 * Callback for opac_admin_server_mapping_edit_form.
 */
function opac_authorities_mapped_with_description_callback($form, &$form_state) {
  return $form['mapping']['field_description'];
}

/**
 * Form submit handler for opac_authorities_admin_server_mapping_edit_form().
 */
function opac_authorities_admin_server_mapping_edit_form_submit($form, &$form_state) {
  module_load_include('inc', 'opac_authorities', "opac_authorities.db");

  // The following values can be NULL be have to be set
  // in the values array. Else this causes errors in
  // mapping add or del functions.
  $values = $form_state['values'];
  if (!isset($values['term_field'])) {
    $values['term_field'] = NULL;
  }
  if (!isset($values['term_field_label'])) {
    $values['term_field_label'] = NULL;
  }
  if (!isset($values['nomatch_rule'])) {
    $values['nomatch_rule'] = NULL;
  }
  if (!isset($values['vocabulary_machine_name'])) {
    $values['vocabulary_machine_name'] = NULL;
  }
  if (!isset($values['vocabulary_name'])) {
    $values['vocabulary_name'] = NULL;
  }

  // In the case the node field is linked to a vocabulary,
  // we get the vid and the term_field_label to store it
  // with the mapping.
  // The goal is to avoid to repeat this operation for each
  // record in the harvesting process.
  $values['vid'] = NULL;
  if (isset($form_state['values']['vocabulary_machine_name'])) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($values['vocabulary_machine_name']);
    $values['vid'] = $vocabulary->vid;
    if ($values['term_field'] != 'name') {
      $vocabulary_fields = field_info_instances('taxonomy_term', $form_state['values']['vocabulary_machine_name']);
      $values['term_field_label'] = $vocabulary_fields[$values['term_field']]['label'];
    }
  }

  if ($values['new'] && $values['mapped_with']) {
    opac_authorities_mapping_add($values);
  }
  elseif (!$values['new'] && !$values['mapped_with']) {
    opac_authorities_mapping_del($values);
  }
  elseif (!$values['new']) {
    opac_authorities_mapping_mod($values);
  }

  // Clear cache related to opac_authorities_fields_mapping.
  cache_clear_all('opac_authorities_fields_mapping', 'cache', TRUE);

  drupal_goto("admin/config/opac/servers/manage/" . $values['serv_id'] . "/authority-mapping/" . $values['authtype']);
}

/**
 * Delete callback: delete mapping.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $auth_type_name
 *   ILS server identifier.
 *
 * @param string $fieldname
 *   Field name for which delete mapping.
 */
function opac_authority_mapping_delete_callback($serv_id, $auth_type_name, $fieldname) {
  module_load_include('inc', 'opac_authorities', "opac_authorities.db");

  opac_authorities_mapping_del(
    array(
      'serv_id' => $serv_id,
      'authtype' => $auth_type_name,
      'authority_field_name' => $fieldname,
    )
  );
  // Clear cache related to opac_authorities_fields_mapping.
  cache_clear_all('opac_authorities_fields_mapping', 'cache', TRUE);

  drupal_goto("admin/config/opac/servers/manage/$serv_id/authority-mapping/$auth_type_name");
}
