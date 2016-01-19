<?php

/**
 * Helper function to set datex configuration.
 */
function _datex_set_cfg($name, $value) {
  $cfg = variable_get('datex_config', array());
  $cfg[$name] = $value;
  variable_set('datex_config', $cfg);
}

/**
 * Provides administration form for datex module (menu callback).
 */
function datex_admin_form($form, $form_state) {
  $form['datex_config'] = array(
    '#type' => 'vertical_tabs',
  );

  // __________________________________________________________GENERAL CONFIG.
  $form['datex'] = array(
    '#type' => 'fieldset',
    '#title' => t('Datex'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'datex_config',
  );

  $options = array(
    DATEX_PATCHING_MODE => t('Operate in patching mode (Pathcing of file <common.inc> is requied).'),
    DATEX_NONPATCHING_MODE => t('Operate in non-patching mode.'),
  );

  // Use PHP-Intl or not.
  $form['datex']['mode'] = array(
    '#type' => 'radios',
    '#title' => t('Date integration mode'),
    '#default_value' => _datex_cfg('mode'),
    '#description' => t('The mode which datex uses to integrate with Drupal. Patching mode needs patching a core file, But will support more modules and functionalities, Non patching mode (by using theming layer and form alters) has a more limited support, But it is safer. If you can ignore some dates not being localized, Then non-patching mode is recomended.'),
    '#options' => $options,
  );

  $options = array(
    1 => t('Use PHP-INTL'),
    0 => t('Use datex internal methods.'),
  );

  // Use PHP-Intl or not.
  $form['datex']['useintl'] = array(
    '#type' => 'radios',
    '#title' => t('Date conversion method'),
    '#options' => $options,
    '#description' => t('Intl is a PHP library for formatting dates, You can install it using php PECL or through your distrubuotion repositories if available, For further help, consult php.org or your operating system website. If you are on a shared hosting, You should ask your host provider to install it for you.'),
  );


  if (!class_exists('IntlDateFormatter')) {
    $markup = t('PHP-Intl is not available in this environment.');
    $disabled = TRUE;
    $default = 0;
  }
  else {
    $markup = t('PHP-Intl is available and may be used.');
    $disabled = FALSE;
    $default = _datex_cfg('useintl');
  }

  $form['datex']['useintl']['#disabled'] = $disabled;
  $form['datex']['useintl']['#default_value'] = $default;
  $form['datex']['datex_api_intl_status'] = array(
    '#type' => 'markup',
    '#title' => t('PHP-Intl status'),
    '#markup' => $markup,
  );

  $options = array();
  foreach (system_get_date_types() as $type) {
    $options[variable_get('date_format_' . $type['type'])] = $type['title'];
  }

  // ________________________________________________ NONPATHCHING ONLY CONFIG.
  if (_datex_cfg('mode') == DATEX_NONPATCHING_MODE) {
    $form['datex_nonpatching'] = array(
      '#type' => 'fieldset',
      '#title' => t('Non-patching mode'),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
      '#group' => 'datex_config',
    );
   
    $form['datex_nonpatching']['node_fmt'] = array(
      '#type' => 'radios',
      '#title' => t('Date format for node display pages.'),
      '#default_value' => _datex_cfg('node_fmt'),
      '#description' => t('Date in node display page will be formatted with this format. Only applicable when using non-patching mode.'),
      '#options' => $options,
    );
   
    $form['datex_nonpatching']['comment_fmt'] = array(
      '#type' => 'radios',
      '#title' => t('Date format for comment display pages.'),
      '#default_value' => _datex_cfg('comment_fmt'),
      '#description' => t('Date in comment display will be formatted with this format. Only applicable when using non-patching mode.'),
      '#options' => $options,
    );
  }

  // ______________________________________________________________ NEW SCHEMA.
  $form['datex_new_schema'] = array(
    '#type' => 'fieldset',
    '#title' => t('New schema'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'datex_config',
  );

  $calendars = _datex_api_available_calendars();
  $options = drupal_map_assoc($calendars);
  $form['datex_new_schema']['datex_new_schema_machine_name'] = array(
    '#title' => t('Schema machine name'),
    '#type' => 'machine_name',
    '#maxlength' => 31,
    '#description' => t('Schema machine readable name containing only letters, Numbers and underscores.'),
    '#machine_name' => array(
      'exists' => '_datex_machine_name',
    ),
    '#required' => FALSE,
  );

  foreach (language_list() as $code => $lang) {
    $t = array('@name' => $lang->name, '@native' => $lang->native);
    $name = 'new_datex_' . $code;
    $form['datex_new_schema'][$name] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable datex in @name (@native)', $t),
      '#default_value' => 0,
    );

    $form['datex_new_schema'][$name . '_calendar'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Calendar'),
      '#states' => array(
        'invisible' => array(
          ':input[name="' . $name . '"]' => array('checked' => FALSE),
        ),
      ),
    );
  }

  // ________________________________________________________ SCHEMA MANAGEMENT.
  $form['datex_schema_manage'] = array(
    '#type' => 'fieldset',
    '#description' => t('Mark schemas to delete after save.'),
    '#title' => t('Manage schemas'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'datex_config',
  );

  $header = array(
    'title' => t('Title'),
    'operations' => array('data' => t('Operations')),
  );


  $options = array();
  foreach (_datex_schema_list() as $name) {
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'admin/config/regional/date-time/datex/edit/' . $name,
    );

    $options[$name] = array(
      'title' => check_plain($name),
      'operations' => array(
        'data' => array(
          '#theme' => 'links__node_operations',
          '#links' => $operations,
          '#attribiutes' => array('class' => array('links', 'inline')),
        ),
      ),
    );
  }

  $form['datex_schema_manage']['datex_table'] = array(
    '#type' => 'tableselect',
    '#header' => $header, 
    '#options' => $options,
    '#element_validate' => array('_datax_schema_table_validate'),
  );

  // _________________________________________________________________ ACTIONS.
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('datex_admin_form_submit'),
    '#value' => t('Save'),
    '#name' => 'config',
  );

  return $form;
}

/**
 * Datex admin form table select element validate.
 *
 * So the user does not remove default schema.
 */
function _datax_schema_table_validate($element, $form_state) {
  $v = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  if ($v['default'] === 'default') {
    form_set_error($element['#name'], t('You can not remove default schema'));
  }
}

/**
 * Form submit for datex admin form.
 *
 * This admin form could be implemented better.
 */
function datex_admin_form_submit($form, &$form_state) {
  $v = $form_state['values'];
  $cfg = array(
    'useintl',
    'mode',
    'node_fmt',
    'comment_fmt',
  );

  foreach($cfg as $name) {
    _datex_set_cfg($name, $v[$name]);
  }

  $schema = variable_get('datex_schema');
  if (!empty($v['datex_new_schema_machine_name'])) {
    $machine = $v['datex_new_schema_machine_name'];

    foreach (language_list() as $language) {
      $code = $language->language;
      if ($v['new_datex_' . $code] != '0') {
        $schema[$machine][$code] = $v['new_datex_' . $code . '_calendar'];
      }
    }
  }

  foreach ($v['datex_table'] as $select) {
    if ($select !== 'default' && isset($schema[$select])) {
      unset($schema[$select]);
    }
  }
  variable_set('datex_schema', $schema);
}

/**
 * Datex form element validation for schema name.
 */
function _datex_machine_name($value, $element, $form_state) {
  $machine = $form_state['values']['datex_new_schema_machine_name'];

  $datex = array(
    'default',
    'disabled',
  );

  if (in_array($machine, $datex)  || in_array($machine, _datex_schema_list())) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Menu callback, Admin form for editing datex schema.
 */
function datex_schema_edit_form($form, $form_state, $name) {
  $list = _datex_schema_list();
  if (!in_array($name, $list)) {
    $form['notfound'] = array(
      '#markup' => t('Not found'),
    );

    return $form;
  }

  $schema = variable_get('datex_schema');
  $default = $schema[$name];
  $form['schemaname'] = array(
    '#type' => 'markup',
    '#markup' => '<h2> Name: ' . check_plain($name) . '</h2>',
  );

  $form['name'] = array(
    '#type' => 'value',
    '#value' => $name,
  );

  $options = _datex_api_available_calendars();
  foreach (language_list() as $code => $lang) {
    $calendar = isset($default[$code]) ? strval($default[$code]) : '0';
    $enabled = $calendar !== '0';
    $t = array('@name' => $lang->name, '@native' => $lang->native);
    $form['datex_' . $code] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable datex in @name (@native)', $t),
      '#default_value' => $enabled ? '1' : '0',
    );

    $form[$code . '_calendar'] = array(
      '#type' => 'select',
      '#options' => drupal_map_assoc($options),
      '#title' => t('Calendar'),
      '#states' => array(
        'invisible' => array(
          ':input[name="datex_' . $code . '"]' => array('checked' => FALSE),
        ),
      ),

      '#default_value' => $enabled ? $default[$code] : '',
    );
  }
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('datex_schema_edit_form_submit'),
  );

  return $form;
}

/**
 * Form submit callback for: datex_schema_edit_form().
 */
function datex_schema_edit_form_submit($form, $form_state) {
  $v = $form_state['values'];
  $name = $v['name'];
  $schema = variable_get('datex_schema');

  foreach (language_list() as $lang) {
    $code = $lang->language;
    if ($v['datex_' . $code] == '0') {
      if (isset($schema[$name][$code])) {
        unset($schema[$name][$code]);
      }
    }
    else {
      $schema[$name][$code] = $v[$code . '_calendar'];
    }
  }

  variable_set('datex_schema', $schema);

  $message = t('New configuration for @schema schema saved.', array('@schema' => $name));
  drupal_set_message($message);
}
