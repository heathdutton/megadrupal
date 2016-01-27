<?php

/**
 * Creates the general admin form.
 */
function ext_admin_form() {
  $form = array();

  $form['ext_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable.'),
    '#default_value' => variable_get('ext_enabled', 1),
    '#description' => t("Uncheck to prevent loading Ext."),
  );

  $path = ext_get_path();
  // If the Ext library was found.
  if ($path) {
    // Retrieve all .js files from the Ext directory.
    $libraries = file_scan_directory($path, '/\.js$/', array('key' => 'filename', 'recurse' => FALSE));
    if ($libraries) {
      // Add options for all available Ext libraries.
      foreach ($libraries as $library) {
        $library_options[$library->filename] = $library->filename;
      }
      $form['ext_library_file'] = array(
        '#type' => 'select',
        '#title' => t('Library file to use'),
        '#description' => t("Select which Ext library file to use. Leave this one alone unless you know what you're doing."),
        '#options' => $library_options,
        '#default_value' => variable_get('ext_library_file', 'ext-all.js'),
      );
    }
  }

  $form['ext_namespace'] = array(
    '#type' => 'textfield',
    '#title' => t('Ext application name'),
    '#default_value' => ext_get_namespace(),
    '#description' => t("The top-level namespace to put all generated classes in. Leave this one alone unless you know what you're doing."),
  );

  $form['#submit'][] = 'ext_admin_form_submit';

  return system_settings_form($form);
}

function ext_admin_form_submit($form, &$form_state) {
  ext_regenerate_js();
}


/**
 * Creates the Ext Model admin form.
 */
function ext_admin_models_page() {
  $output = '';

  // List existing Models.
  $models = ext_get_models();
  $table_header = array(t('Name, Type, Extends, Module'), t('Status'), t('Fields'), t('Associations'), t('Operations'));
  $table_rows = array();
  foreach ($models as $model_name => $model) {
    $fields = array();
    if (!empty($model['fields'])) {
      foreach ($model['fields'] as $field) {
        $title_text = "";
        foreach ($field as $key => $value) {
          $title_text .= "$key: $value\n";
        }
        $fields[] = '<span class="ext-collection-item" title="'. $title_text. '">'. $field['name']. '</span>';
      }
    }
    
    $associations = array();
    if (!empty($model['associations'])) {
      foreach ($model['associations'] as $association) {
        $title_text = "";
        foreach ($association as $key => $value) {
          $title_text .= "$key: $value\n"; 
        }
        $associations[] = '<span class="ext-collection-item" title="'. $title_text. '">'. t("!type: !model", array('!type' => $association['type'], '!model' => $association['model'])). '</span>';
      }
    }
    if( array_key_exists('#type', $model))
    {
    $ops = array(
      l(t("edit"), "admin/build/ext/models/edit/{$model['#type']}/$model_name"),
    );
    $type_info = ext_get_model_type($model['#type']);
    if ($type_info['user_created']) {
      $ops[] = l(t("delete"), "admin/build/ext/models/delete/{$model['#type']}/$model_name");
    }
    }
    else 
    {
    	watchdog('ext', '#type not defined in model !model.', array('!model' => $model_name));
    }

    $table_rows[] = array(
      t('Name:&nbsp;'). $model_name. '<br />'.
      (array_key_exists('#type', $model)? t('Type:&nbsp;').$model['#type']. '<br />':'').
      (array_key_exists('#extends', $model)? t('Extends:&nbsp;').$model['#extends']. '<br />':'').
      t('Module:&nbsp;'). (array_key_exists('$module', $model)?$model['#module']:''). '<br />',
      ext_model_is_enabled($model_name) ? t('Enabled') : t('Disabled'),
      implode('', $fields),
      implode('', $associations),
      implode(", ", $ops),
    );
  }

  $output .= '<strong>'. t("Hover over a field or association to see more information about it."). '</strong>'; 
  $output .= theme_table(array('header' => $table_header, 'rows' => $table_rows, 'attributes'=> array(), 'caption' => null, 'colgroups' => array(), 'sticky' => null, 'empty' => null));

  return $output;
}


/**
 * Creates the Ext Store admin form.
 */
function ext_admin_stores_page() {
  $output = '';

  // List existing Stores.
  $stores = ext_get_stores();
  $table_header = array(t('Name'), t('Status'), t('Type'), t('Model'));
  $table_rows = array();
  foreach ($stores as $store_name => $store) {
    $ops = array(
      l(t("edit"), "admin/build/ext/stores/edit/{$store['#type']}/$store_name"),
    );
    $type_info = ext_get_store_type($store['#type']);
    if ($type_info['user_created']) {
      $ops[] = l(t("delete"), "admin/build/ext/stores/delete/{$store['#type']}/$store_name");
    }

    $table_rows[] = array(
      $store_name,
      ext_store_is_enabled($store_name) ? t('Enabled') : t('Disabled'),
      $store['#type'],
      $store['model'],
      implode(", ", $ops),
    );
  }

  $output .= theme_table(array('header' => $table_header, 'rows' => $table_rows,
   'attributes'=> array(), 'caption' => null, 'colgroups' => array(), 'sticky' => null, 'empty' => null));

  return $output;
}




function ext_admin_add_type_form($form, &$form_state, $def_type) {
  $types = _ext_get_type($def_type);
  $options = array();
  foreach ($types as $type_name => $type_info) {
    if ($type_info['user_created']) {
      $options[$type_name] = ucfirst($type_name);
    }
  }

  if (!empty($options)) {
    $form['type'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t("Select !def_type type to create", array('!def_type' => ucfirst($def_type))),
      '#required' => TRUE,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t("Select"),
    );
     $form['def_type'] = array(
      '#type' => 'value',
      '#value' => $def_type,
    );
  }
  else {
    $form['message'] = array(
      '#type' => 'markup',
      '#value' => '<p>'. t("There are currently no !def_type types available that you can create. Try installing and/or enabling modules that provide some.", array('!def_type' => $def_type)). '</p>',
    );
  }

  return $form;
}

function ext_admin_add_type_form_submit($form, &$form_state) {
  $vals = $form_state['values'];
  $form_state['redirect'] = "admin/build/ext/". $vals['def_type']. "s/add/". $vals['type'];
}


function ext_admin_add_form($form, &$form_state, $def_type, $type) {

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t("Enter a name for the new !def_type. The name should be in CamelCase format (no spaces and the first letter of each word capitalised), may contain only alphanumeric characters, and must start with a letter.", array('!def_type' => ucfirst($def_type))),
    '#required' => TRUE,
  );

  $form['def_type'] = array(
    '#type' => 'value',
    '#value' => $def_type,
  );
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );

  $type_info = _ext_get_type($def_type, $type);
  _ext_module_load_include($type_info['module'], 'ext.admin', 'inc', 'includes');
  $func = $type_info['module']. "_ext_{$def_type}_addedit_form";
  if (function_exists($func)) {
    $custom_form = $func($form_state, $type);
    if ($custom_form) {
      $form = array_merge($form, $custom_form);
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save"),
  );

  return $form;
}

function ext_admin_add_form_validate($form, &$form_state) {
  $vals = $form_state['values'];

  if (!preg_match('/^[A-Z][0-9a-zA-Z]*$/', $vals['name'])) {
    form_set_error('name', t('The Model name does not have the correct format.'));
  }
  if (call_user_func("ext_get_". $vals['def_type']. "s", $vals['name'])) {
    form_set_error('name', t("The name is already in use."));
  }

  $type_info = _ext_get_type($vals['def_type'], $vals['type']);
  _ext_module_load_include($type_info['module'], 'ext.admin', 'inc', 'includes');
  $func = $type_info['module']. "_ext_". $vals['def_type']. "_addedit_form_validate";
  if (function_exists($func)) {
    $func($form, $form_state, $vals['type'], $vals['name']);
  }
}

function ext_admin_add_form_submit($form, &$form_state) {
  $vals = $form_state['values'];
  $type_info = _ext_get_type($vals['def_type'], $vals['type']);

  $disabled = variable_get("ext_". $vals['def_type']. "_disabled", array());
  $disabled[$vals['name']] = !$vals['enable'];
  variable_set("ext_". $vals['def_type']. "_disabled", $disabled);

  // Set this here so the module hook below can alter it if required.
  $form_state['redirect'] = "admin/build/ext/". $vals['def_type']. "s";

  _ext_module_load_include($type_info['module'], 'ext.admin', 'inc', 'includes');
  $func = $type_info['module']. "_ext_". $vals['def_type']. "_addedit_form_submit";
  if (function_exists($func)) {
    $func($form, $form_state, $vals['type'], $vals['name']);
  }

  ext_regenerate_js();
}



function ext_admin_edit_form($form, &$form_state, $def_type, $type, $name) {

  $form['enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => call_user_func("ext_{$def_type}_is_enabled", $name),
  );

  $form['def_type'] = array(
    '#type' => 'value',
    '#value' => $def_type,
  );
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $name,
  );

  $form['#redirect'] = "admin/build/ext/{$def_type}s";

  $type_info = _ext_get_type($def_type, $type);
  _ext_module_load_include($type_info['module'], 'ext.admin', 'inc', 'includes');
  $func = $type_info['module']. "_ext_{$def_type}_addedit_form";
  if (function_exists($func)) {
    $custom_form = $func($form_state, $type, $name);
    if ($custom_form) {
      $form = array_merge($form, $custom_form);
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save"),
  );

  return $form;
}

function ext_admin_edit_form_validate($form, &$form_state) {
  $vals = $form_state['values'];
  $type_info = _ext_get_type($vals['def_type'], $vals['type']);
  _ext_module_load_include($type_info['module'], 'ext.admin', 'inc', 'includes');
  $func = $type_info['module']. "_ext_". $vals['def_type']. "_addedit_form_validate";
  if (function_exists($func)) {
    $func($form, $form_state, $vals['type'], $vals['name']);
  }
}

function ext_admin_edit_form_submit($form, &$form_state) {
  $vals = $form_state['values'];
  $type_info = _ext_get_type($vals['def_type'], $vals['type']);

  $disabled = variable_get("ext_". $vals['def_type']. "s_disabled", array());
  $disabled[$vals['name']] = !$vals['enable'];
  variable_set("ext_". $vals['def_type']. "s_disabled", $disabled);

  _ext_module_load_include($type_info['module'], 'ext.admin', 'inc', 'includes');
  $func = $type_info['module']. "_ext_". $vals['def_type']. "_addedit_form_submit";
  if (function_exists($func)) {
    $func($form, $form_state, $vals['type'], $vals['name']);
  }

  ext_regenerate_js();
}




function ext_admin_delete_form(&$form_state, $def_type, $type, $name) {
  $form = array();
  $form['#model_def_type'] = $def_type;
  $form['#model_type'] = $type;
  $form['#model_name'] = $name;
  return confirm_form(
    $form,
    t('Are you sure you want to delete the !def_type %name?', array('!def_type' => ucfirst($def_type), '%name' => $name)),
    "admin/build/ext/{$def_type}s",
    t("This action cannot be undone."),
    t("Delete"),
    t("Cancel")
  );
}

function ext_admin_delete_form_submit($form, &$form_state) {
  $type_info = _ext_get_type($form['#model_def_type'], $form['#model_type']);

  $disabled = variable_get("ext_". $form['#model_def_type']. "_disabled", array());
  unset($disabled[$form['#model_name']]);
  variable_set("ext_". $form['#model_def_type']. "_disabled", $disabled);

  _ext_module_load_include($type_info['module'], 'ext.admin', 'inc', 'includes');
  module_invoke($type_info['module'], "ext_". $form['#model_def_type']. "_delete", $form['#model_type'], $form['#model_name']);

  ext_regenerate_js();

  $form_state['redirect'] = "admin/build/ext/{$form['#model_def_type']}s";
}
