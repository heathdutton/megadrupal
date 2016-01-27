<?php
/**
 * @file
 * Admin form for opac module.
 */

/**
 * Form constructor for the opac configuration form.
 *
 * This form allows to configure which fields use for record and server
 * identifier, and harvest batch size.
 *
 * @see opac_admin_form_validate()
 * @ingroup forms
 */
function opac_admin_form($form, &$form_state) {
  $form['harvest'] = array(
    '#type' => 'fieldset',
    '#title' => t('Harvest settings', array(), array('context' => 'libraries')),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['harvest']['harvest_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Harvest Batch size', array(), array('context' => 'libraries')),
    '#default_value' => variable_get('harvest_batch_size', 1000),
    '#size' => 20,
    '#required' => TRUE,
  );

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $date_types = system_get_date_types();
  foreach ($date_types as $code => $values) {
    $opac_date_options[$code] = $values['title'];
  }
  $form['display']['opac_date_type'] = array(
    '#type' => 'select',
    '#title' => t('Date type'),
    '#options' => $opac_date_options,
    '#default_value' => variable_get('opac_date_type'),
    '#description' => t("You can add new date types in Configuration -> Regional and language -> Date and time."),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for opac_admin_form().
 */
function opac_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  // Harvest_batch_size is the number of records requested
  // for each request. It must be a numeric value and
  // greater than zero.
  if (!is_numeric($values['harvest_batch_size'])) {
    form_set_error('opac', t('You must enter an integer for the Harvest Batch size.', array(), array('context' => 'libraries')));
  }
  elseif ($values['harvest_batch_size'] <= 0) {
    form_set_error('opac', t('Harvest Batch size must be positive.', array(), array('context' => 'libraries')));
  }
}

/**
 * Retrieve all fields of a given node_type.
 *
 * @param string $node_type
 *   Node type.
 *
 * @return array
 *   Fields list related to the node type.
 */
function opac_get_fields_options($node_type) {
  // Get all fields for the node_type.
  $fields = field_info_instances('node', $node_type);
  // Add 'none' default choice with others.
  $fields_options = array('none' => 'None');
  foreach ($fields as $name => $field) {
    $fields_options[$name] = $field['label'];
  }
  return $fields_options;
}

/**
 * Callback for opac_admin_form.
 */
function opac_admin_form_callback($form, $form_state) {
  return $form['field_settings'];
}

/**
 * Build a server view page.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @return string
 *   HTML output.
 */
function opac_server_view($serv_id) {
  module_load_include('inc', 'opac', "includes/opac.db");

  $server = opac_server_load($serv_id);
  $connector = opac_get_instance($server);
  $return_string = "<h2>" . $server->serv_name . "</h2>";

  // Getting webservices informations.
  $wsinfos = $connector->getInfos();

  // Looping through results.
  foreach ($wsinfos as $ws) {

    // Basic informations.
    $return_string .= '<h3>' . t('ILS Connection test through @ws_name', array('@ws_name' => $ws['name'])) . '</h3>';
    $url = '<a href="' . $ws['url'] . '">' . $ws['url'] . '</a></li>';
    $return_string .= '<ul><li>' . t('Called url: @url', array('@url' => $url));
    $return_string .= '<li>' . t('HTTP response code: @code', array('@code' => $ws['code'])) . '</li>';

    // Additional informations.
    if (array_key_exists('infos', $ws)) {
      $return_string .= '<li>' . t('Additional informations:') . '<ul>';
      foreach ($ws['infos'] as $key => $value) {
        $return_string .= "<li>$key: $value</li>";
      }
      $return_string .= "</ul></li>";
    }
    // Raw response.
    if (array_key_exists('raw', $ws)) {
      $response = '<pre>' . htmlentities(print_r($ws['raw'], 1)) . '</pre></li>';
      $return_string .= '<li>' . t('Raw response: @response', array('@response' => $response));
    }
    $return_string .= "</ul>";
  }
  return $return_string;
}


/**
 * Form constructor for the server editing/adding form.
 *
 * @param objcet $server
 *   ILS server object.
 *
 * @param string $op
 *   Telling if we are adding or editing a server.
 *
 * @see opac_admin_server_edit_form_validate()
 * @see opac_admin_server_edit_form_submit()
 * @ingroup forms
 */
function opac_server_form($form, &$form_state, $server, $op = 'edit') {
  module_load_include('inc', 'opac', "includes/opac.db");

  $new = ($op == 'add');

  $form['serv_name'] = array(
    '#type' => 'textfield',
    '#size' => 25,
    '#title' => t('OPAC server name', array(), array('context' => 'libraries')),
    '#default_value' => $new ? '' : $server->serv_name,
    '#description' => t('Enter here an arbitrary name for the server.'),
    '#required' => TRUE,
  );
  $form['serv_id'] = array(
    '#type' => 'machine_name',
    '#default_value' => $new ? '' : $server->serv_id,
    '#maxlength' => 32,
    '#disabled' => $new ? FALSE : TRUE,
    '#machine_name' => array(
      'exists' => 'opac_server_load',
      'source' => array('serv_name'),
    ),
  );
  $form['serv_host'] = array(
    '#type' => 'textfield',
    '#title' => t('host', array(), array('context' => 'libraries')),
    '#default_value' => $new ? '' : $server->serv_host,
    '#description' => t('Url of the server that opac module will contact. I.e: http://my-library.org'),
    '#required' => TRUE,
  );

  // Get all available connectors.
  // The opac_available_connectors function
  // return an array like (id => (id => name)).
  // So we need to build a different array
  // like (id => label) to populate #options key.
  $connectors = opac_available_connectors();
  $connectors_options = array('none' => 'None');
  foreach ($connectors as $id => $connector) {
    $connectors_options[$id] = $connector['name'];
  }

  $form['serv_connector'] = array(
    '#type' => 'select',
    '#title' => t('ILS Connector', array(), array('context' => 'libraries')),
    '#default_value' => $new ? 'none' : $server->serv_connector,
    '#options' => $connectors_options,
    '#description' => t('Choose a connector', array(), array('context' => 'libraries')),
    '#required' => TRUE,
  );
  $form['first_bib_num'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('First record number', array(), array('context' => 'libraries')),
    '#default_value' => $new ? 0 : $server->first_bib_num,
    '#description' => t('Number of the first record in the server.', array(), array('context' => 'libraries')),
  );
  $form['last_bib_num'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Last record number', array(), array('context' => 'libraries')),
    '#default_value' => $new ? 0 : $server->last_bib_num,
    '#description' => t('Number of the last record in the server.', array(), array('context' => 'libraries')),
  );
  $form['serv_enabled'] = array(
    '#type' => 'checkbox',
    '#default_value' => $new ? 0 : $server->serv_enabled,
    '#title' => t('Enabled ?'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Form validation handler for opac_admin_server_edit_form().
 *
 * @see opac_admin_server_edit_form_submit()
 */
function opac_server_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  // Check required parameters.
  if (!filter_var($values['serv_host'], FILTER_VALIDATE_URL)) {
    form_set_error('opac', t('Server host is not valid url. Must looks like http://catalog.yourlibrary.com'));
  }
  if (!ctype_digit($values['first_bib_num'])) {
    form_set_error('opac', t('First record number must be an integer value', array(), array('context' => 'libraries')));
  }
  if (!ctype_digit($values['last_bib_num'])) {
    form_set_error('opac', t('First record number must be an integer value', array(), array('context' => 'libraries')));
  }
}

/**
 * Form API submit callback for the type form.
 */
function opac_server_form_submit(&$form, &$form_state) {
  $opac_server = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $opac_server->save();
  $form_state['redirect'] = 'admin/config/opac/servers';
}

/**
 * Form constructor for global mapping form.
 *
 * @param object $opac_server
 *   ILS server.
 *
 * @param string $content_type
 *   Content type machine name.
 *
 * @see opac_admin_server_mapping_form_submit()
 * @ingroup forms
 */
function opac_admin_server_mapping_form($form, &$form_state, $opac_server, $content_type) {
  module_load_include('inc', 'opac', "includes/opac.db");
  module_load_include('inc', 'opac', "includes/opac.harvest");

  // Get a list of mapped and unmapped fields
  // for the current server.
  $mapping = opac_build_fields_mapping($opac_server->serv_id, $content_type, NULL, TRUE);

  // Retrieves server values and create an
  // instance of the corresponding connector.
  // So we can call biblioFields method to
  // to get ILS available fields.
  $connector = opac_get_instance($opac_server);
  $connector_fields = $connector->biblioFields();

  $header = array(
    'content_type' => 'Content type',
    'node_field' => 'Node field',
    'mapped_with' => 'Mapped with',
    'plugin' => 'Plugin',
    'operations' => 'Operations',
  );

  // This section is for fields that already
  // have been mapped.
  $data = array();
  foreach ($mapping['mapped'] as $values) {
    $name = $values['node_field_name'];
    // The label of the cuurent field.
    $node_label = $values['node_field_label'];
    // The machine name of the cuurent field.
    $node_name = $values['node_field_name'];
    $content_type = $values['content_type'];
    // This provide a link to field settings page.
    $field_link = l(t("@node_label (@node_name)", array("@node_label" => $node_label, "@node_name" => $node_name)), "admin/structure/types/manage/$content_type/fields/$node_name");

    // The connector_label var is the label for
    // ILS/connector field.
    $connector_label = $connector_fields[$values['mapped_with']]['label'];
    $connector_name = $values['mapped_with'];
    $connector_html = "$connector_label ($connector_name)";

    $plugin = $values['plugin'];

    $row['content_type'] = $content_type;
    $row['node_field'] = $field_link;
    $row['mapped_with'] = $connector_html;
    $row['plugin'] = $plugin;
    $row['operations'] = $values['operations'];

    $key = $content_type . ' ' . $name;

    $data[$key] = $row;
  }

  $info = t('<h2>You are editing mapping for the content type <strong>@content_type</strong> and server @opac_server.</h2>', array('@content_type' => $content_type, '@opac_server' => $opac_server->serv_id));
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
    '#markup' => theme('opac_mapping_overview', array(
      'mapping' => $mapping['unmapped'],
      'serv_id' => $opac_server->serv_id,
    )),
  );

  return $form;
}

/**
 * Form submission handler for opac_admin_server_mapping_form().
 */
function opac_admin_server_mapping_form_submit($form, &$form_state) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $values = $form_state['values'];
  // Loop through all checked fields to unmap them.
  foreach ($values['fields_mapped'] as $key => $delete) {
    if ($delete) {
      list ($content_type, $node_field_name) = explode(' ', $key);
      opac_mapping_del(
        array(
          'serv_id' => $values['serv_id'],
          'content_type' => $content_type,
          'node_field_name' => $node_field_name,
        )
      );
    }
  }
  // Clear cache related to opac_fields_mapping.
  cache_clear_all('opac_fields_mapping', 'cache', TRUE);
}

/**
 * Returns the list of available plugins.
 *
 * @param OpacServer $server
 *   OpacServer object.
 *
 * @param array $field
 *   Field informations.
 */
function opac_available_plugins($server, $field) {
  module_load_include('inc', 'opac', 'includes/opac.plugin');
  $plugins = opac_plugin_get_all();
  $available_plugins = array();
  foreach ($plugins as $name => $plugin) {
    opac_plugin_require_file($plugin);
    $available = FALSE;
    $available_cb = $plugin['available callback'];
    if (function_exists($available_cb)) {
      $available = $available_cb($server, $field);
    }
    if ($available) {
      $available_plugins[$name] = $plugin['label'];
    }
  }
  return $available_plugins;
}

/**
 * Form constructor for mapping editing/adding form.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $fieldname
 *   Field name to map.
 *
 * @see opac_admin_server_mapping_edit_form_submit()
 * @ingroup forms
 */
function opac_admin_server_mapping_edit_form($form, &$form_state, $serv_id, $content_type, $fieldname) {
  module_load_include('inc', 'opac', "includes/opac.harvest");
  module_load_include('inc', 'opac', "includes/opac.db");
  // Get mapping information for $fieldname and $serv_id.
  $mapping = opac_build_fields_mapping($serv_id, $content_type, $fieldname);
  // Are we editing a mapped or unmapped field?
  $new = count($mapping['mapped']) ? 0 : 1;

  // Retrieves server values and create an
  // instance of the corresponding connector.
  // So we can call biblioFields method to
  // to get ILS available fields.
  $server = opac_server_load($serv_id);
  $connector = opac_get_instance($server);
  $connector_fields = $connector->biblioFields();
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
  $form['content_type'] = array(
    '#type' => 'hidden',
    '#value' => $content_type,
  );
  $form['new'] = array(
    '#type' => 'hidden',
    '#value' => $new,
  );
  $form['node_field_name'] = array(
    '#type' => 'hidden',
    '#value' => $fieldname,
  );

  $node_field_label = $new ? $mapping['unmapped'][0]['label'] : $mapping['mapped'][0]['node_field_label'];
  $form['node_field_label'] = array(
    '#type' => 'hidden',
    '#value' => $node_field_label,
  );
  $form['content_type'] = array(
    '#type' => 'hidden',
    '#value' => $new ? $mapping['unmapped'][0]['content_type'] : $mapping['mapped'][0]['content_type'],
  );

  // Here is a message to the user on how to use this form.
  $node_field_link = l("$node_field_label ($fieldname)", "admin/structure/types/manage/$content_type/fields/$fieldname");
  $html = '<div>' . t('Editing mapping for node field @node_field_link.', array('@node_field_link' => $node_field_link));
  $html .= '<span>' . t('Be careful of multivalued fields. Some ILS fields send back more than one value.') . '<br />';
  $html .= t('A node field than accept only one value and which is mapped with a multivalued filed will contain only the first one.') . '<br />';
  $html .= t('So if it is, you may edit this node field to make accept multiple values.') . '</span></div>';
  $form['mapping_information'] = array(
    '#markup' => $html,
  );

  $field_mapped_with = $new ? NULL : $mapping['mapped'][0]['mapped_with'];
  $form['mapping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mapping', array(), array('context' => 'libraries')),
  );
  // Build a list with all available ILS/connector fields.
  $form['mapping']['mapped_with'] = array(
    '#type' => 'select',
    '#title' => t('Mapped with', array(), array('context' => 'libraries')),
    '#required' => TRUE,
    '#default_value' => $field_mapped_with,
    '#options' => $fields_options,
    '#ajax' => array(
      'callback' => 'opac_mapped_with_description_callback',
      'wrapper' => 'replace_description_field',
    ),
  );

  // Connectors should provide a description
  // for each fields it proposes.
  $form['mapping']['field_description'] = array(
    // Populate this form element with field description.
    // The form is rebuilt during ajax processing.
    '#markup' => isset($form_state['values']) ?
    opac_get_field_descrition($form_state['values']['serv_id'], $form_state['values']['mapped_with']) :
    opac_get_field_descrition($serv_id, $field_mapped_with),
    '#prefix' => '<div id="replace_description_field">',
    '#suffix' => '</div>',
  );

  $form['plugin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plugin', array(), array('context' => 'libraries')),
  );
  $plugin_default_value = $new ? '' : $mapping['mapped'][0]['plugin'];
  $available_plugins = opac_available_plugins($server, $field_settings);
  $form['plugin']['plugin'] = array(
    '#type' => 'select',
    '#title' => t('Plugin', array(), array('context' => 'libraries')),
    '#default_value' => $plugin_default_value,
    '#options' => array('' => t('None')) + $available_plugins,
    '#ajax' => array(
      'callback' => 'opac_plugin_options_callback',
      'wrapper' => 'plugin_options',
    ),
  );
  $form['plugin']['options'] = array(
    '#prefix' => '<div id="plugin_options">',
    '#suffix' => '</div>',
  );

  $plugin;
  if (isset($form_state['values'])) {
    $plugin = $form_state['values']['plugin'];
  }
  else {
    $plugin = $plugin_default_value;
  }
  if ($plugin) {
    $plugin = opac_plugin_get($plugin);
    opac_plugin_require_file($plugin);
    if ($plugin && isset($plugin['form callback']) && function_exists($plugin['form callback'])) {
      $form_plugin = $plugin['form callback']($form_state, $field_settings, $mapping);
      $form['plugin']['options'] = array_merge($form['plugin']['options'], $form_plugin);
    }
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
function opac_get_field_descrition($serv_id, $name) {
  if (!$name) {
    return '<div class="description">' . t('Select a field to be mapped with current node field.') . '</div>';
  }
  $server = opac_server_load($serv_id);
  $connector = opac_get_instance($server);
  $connector_fields = $connector->biblioFields();
  $label = $connector_fields[$name]['label'];
  $description = $connector_fields[$name]['description'];

  $return = '<div><strong>' . t('Description of @label field:', array('@label' => $label));
  $return .= '</strong><div class="description">' . t($description) . '</div>';
  return $return;
}

/**
 * Callback for opac_admin_server_mapping_edit_form.
 */
function opac_mapped_with_description_callback($form, &$form_state) {
  return $form['mapping']['field_description'];
}

/**
 * Callback for opac_admin_server_mapping_edit_form.
 */
function opac_plugin_options_callback($form, &$form_state) {
  return $form['plugin']['options'];
}
/**
 * Form submission handler for opac_admin_server_mapping_edit_form().
 */
function opac_admin_server_mapping_edit_form_submit($form, &$form_state) {
  module_load_include('inc', 'opac', "includes/opac.db");

  $values = $form_state['values'];

  if (!$values['plugin']) {
    $values['plugin'] = NULL;
  }

  // Remove all known values to have only plugin values and then serialize them.
  $plugin_options = $values;
  $mapping_keys = array('serv_id', 'content_type', 'node_field_name', 'node_field_label',
    'mapped_with', 'plugin', 'new', 'submit', 'form_build_id', 'form_token', 'form_id', 'op');
  foreach ($mapping_keys as $key) {
    unset($plugin_options[$key]);
  }
  $values['plugin_options'] = serialize($plugin_options);

  if ($values['new'] && $values['mapped_with']) {
    opac_mapping_add($values);
  }
  elseif (!$values['new'] && !$values['mapped_with']) {
    opac_mapping_del($values);
  }
  elseif (!$values['new']) {
    opac_mapping_mod($values);
  }

  // Clear cache related to opac_fields_mapping.
  cache_clear_all('opac_fields_mapping', 'cache', TRUE);

  drupal_goto("admin/config/opac/servers/manage/" . $values['serv_id'] . "/biblio-mapping/" . $values['content_type']);
}

function opac_mapping_delete_confirm($form, &$form_state, $serv_id, $content_type, $fieldname) {
  $form['serv_id'] = array(
    '#type' => 'value',
    '#value' => $serv_id,
  );
  $form['content_type'] = array(
    '#type' => 'value',
    '#value' => $content_type,
  );
  $form['fieldname'] = array(
    '#type' => 'value',
    '#value' => $fieldname,
  );

  return confirm_form($form,
    t('Are you sure you want to delete this mapping?'),
    "admin/config/opac/servers/manage/$serv_id/biblio-mapping/$content_type",
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function opac_mapping_delete_confirm_submit($form, &$form_state) {
  $values = $form_state['values'];
  $serv_id = $values['serv_id'];
  $content_type = $values['content_type'];
  $fieldname = $values['fieldname'];

  if ($form_state['values']['confirm']) {
    module_load_include('inc', 'opac', "includes/opac.db");

    opac_mapping_del(array(
      'serv_id' => $serv_id,
      'content_type' => $content_type,
      'node_field_name' => $fieldname,
    ));

    // Clear cache related to opac_fields_mapping.
    cache_clear_all('opac_fields_mapping', 'cache', TRUE);
    drupal_set_message(t('Content type mapping has been deleted successfully.'));
  }

  drupal_goto("admin/config/opac/servers/manage/$serv_id/biblio-mapping/$content_type");
}
