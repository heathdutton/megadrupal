<?php

function mail_debugger_php_form($default_values = array()) {
  if (!user_access('use mail debugger php')) {
    return FALSE;
  }
  
  // which modules implement hook_mail?
  $modules = module_implements('mail');

  $contributed_modules = array();
  // loop and add to list of candidates
  foreach ($modules as $module) {
    // Find the human readable name
    $filename = drupal_get_path('module', $module) . "/$module.info";
    $info = drupal_parse_info_file($filename);

    // add to the list of candidates
    $contributed_modules[$module] = $info['package'] . ' | ' . $info['name'];
  }

  // sort the values
  asort($contributed_modules);

  // add the module system name... for the geeks among us.
  foreach ($contributed_modules as $module => $name) {
    $contributed_modules[$module] = $name . ' (' . $module . ')';
  }

  // add contrib section of the form.
  return array(
    'info' => array(
      '#markup' => '<p><big style="font-weight: bold;">Expirimental feature!</big></p>',
    ),
    'to' => array(
      '#type' => 'textfield',
      '#title' => t('E-mail address'),
      '#default_value' => user_variable_get('mail_debugger_contrib_to'),
    ),
    'module' => array(
      '#type' => 'select',
      '#title' => t('Module'),
      '#default_value' => user_variable_get('mail_debugger_contrib_module'),
      '#options' => $contributed_modules,
    ),
    'key' => array(
      '#type' => 'textfield',
      '#title' => t('Mail key'),
      '#default_value' => user_variable_get('mail_debugger_contrib_key'),
    ),
    'param' => array(
      '#type' => 'textarea',
      '#title' => t('Param argument'),
      '#description' => "PHP code to populate the \$param argument. do not use a &lt;?php opening tag and return an array().",
      '#default_value' => user_variable_get('mail_debugger_contrib_param'),
    ),
    'language' => array(
      '#type' => 'textfield',
      '#title' => t('Language'),
      '#default_value' => user_variable_get('mail_debugger_contrib_language'),
    ),
    'from' => array(
      '#type' => 'textfield',
      '#title' => t('From'),
      '#default_value' => user_variable_get('mail_debugger_contrib_from'),
    ),
    'send' => array(
      '#type' => 'checkbox',
      '#title' => t('Send mail'),
      '#default_value' => user_variable_get('mail_debugger_contrib_send'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#name' => 'contrib_mail',
      '#value' => t('Carefull please'),
      '#submit' => array(
        'mail_debugger_callback_submit_contrib_mail',
        'mail_debugger_callback_submit',
      ),
      '#validate' => array('mail_debugger_callback_valid_contrib_mail'),
    ),
  );
}

function mail_debugger_php_form_validate($values, $elements, &$form_state) {
  try {
    $php = $values['param'];

    if (!empty($php)) {
      $param = eval($values['param']);
      if (!is_array($param)) {
        form_set_error($elements['param'], t("The PHP code to populate \$param must be an array"));
      }
    }
  } catch (Exception $ex) {
    form_set_error($elements['param'], $ex->getMessage());
  }

  if (!valid_email_address($values['to'])) {
    form_set_error($elements['to'], t('Enter a valid e-mail address'));
  }

  if (!empty($values['from']) && !valid_email_address($values['from'])) {
    form_set_error($elements['from'], t('Enter a valid e-mail address'));
  }

  if (empty($value['key'])) {
    form_set_error($elements['key'], t('Enter a valid key parameter'));
  }
}

function mail_debugger_php_form_submit($values, &$form_state) {
  if ($values['param']) {
    $param = array();
    try {
      $param = $values['param'];
    } catch (Exception $ex) {
      form_set_error(NULL, $ex->getMessage());
    }
  }

  $module = $values['module'];
  $key = $values['key'];
  $to = $values['to'];
  $language = $values['language'];
  $key = $values['key'];
  $from = $values['from'];
  $send = $values['send'];

  $result = drupal_mail($module, $key, $to, $language, $param, $from, $send);

  if ($result) {
    drupal_set_message('E-mail sent! Please check your mail.');
  } else {
    drupal_set_message('Something went wrong. Check the error logs to get more info.', 'warning');
  }
}
