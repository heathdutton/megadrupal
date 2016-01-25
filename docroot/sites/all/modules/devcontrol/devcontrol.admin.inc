<?php

/**
 * @file
 * Administration screens.
 */

/**
 * Main page form.
 */
function devcontrol_admin_form($form, &$form_state) {

  global $conf;

  $form['#tree'] = true;

  $form['caches'] = array(
    '#type' => 'fieldset',
    '#title' => t("Quick cache clear"),
    '#collapsible' => true,
    '#collapsed' => false,
  );

  foreach (devcontrol_cache_clear_info_get() as $key => $info) {
    $form['caches'][$key] = array(
      '#type' => 'submit',
      '#value' => $info['title'],
      '#submit' => array('devcontrol_admin_form_submit_cache_clear'),
    );
  }

  $form['cache_select'] = array(
    '#type' => 'fieldset',
    '#title' => t("Clear caches"),
    '#collapsible' => true,
    '#collapsed' => false,
  );
  $options = array();
  foreach (devcontrol_cache_clear_info_get() as $key => $info) {
    if ('all' !== $key) {
      $options[$key] = $info['title'];
    }
  }
  $form['cache_select']['bins'] = array(
    '#title'         => t("Caches to drop"),
    '#type'          => 'checkboxes',
    '#options'       => $options,
    '#default_value' => empty($_COOKIE[DEVCONTROL_CACHE_COOKIE]) ? array('menu', 'hook', 'theme') : explode(',', $_COOKIE[DEVCONTROL_CACHE_COOKIE]),
  );
  $form['cache_select']['remember'] = array(
    '#title'         => t("Remember selection"),
    '#type'          => 'checkbox',
    '#default_value' => true,
  );
  $form['cache_select']['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t("Clear selected"),
    '#submit' => array('devcontrol_admin_form_submit_cache_clear_selected'),
  );

  $form['developer'] = array(
    '#type' => 'fieldset',
    '#title' => t("I am a developer..."),
    '#collapsible' => true,
    '#collapsed' => false,
  );
  $form['developer']['help'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t("Obtaining a cookie will allow you to use the developer interface while you're browsing anonymously or with a low privileged user account.")
      . ' ' . t("Once logged out, you will be able to revert at any time your login identity to the one you were while cliking this button, even if you have no access to the administration pages."),
  );
  if (devcontrol_access_has_cookie()) {
    $form['developer']['remove_my_cookie'] = array(
      '#type' => 'submit',
      '#value' => t("Remove my cookie!"),
      '#submit' => array('devcontrol_admin_form_submit_developer_remove_my_cookie'),
    );
  } else {
    $form['developer']['give_me_a_cookie'] = array(
      '#type' => 'submit',
      '#value' => t("Give me a cookie!"),
      '#submit' => array('devcontrol_admin_form_submit_developer_give_me_a_cookie'),
    );
  }

  $bins = devcontrol_cache_bins_list_get();

  $form['bin'] = array(
    '#type' => 'fieldset',
    '#title' => t("Cache bins"),
    '#collapsible' => true,
    '#collapsed' => true,
  );
  $notempty = 0;
  $total    = 0;
  $options  = array();
  foreach ($bins as $bin) {    
    $object = _cache_get_object($bin);
    $options[$bin]['title']['data'] = array('#markup' => check_plain($bin));
    if ($object->isEmpty()) { 
      $label = t("Empty");
    }
    else {
      $label = t("Has data");
      ++$notempty;
    }
    $options[$bin]['empty']['data'] = array('#markup' => $label);
    $options[$bin]['class']['data'] = array('#markup' => get_class($object));
    ++$total;
  }
  $form['bin']['#title'] .= ' (' . $notempty . '/' . $total . ')';
  $form['bin']['bins'] = array(
    '#type' => 'tableselect',
    '#options' => $options,
    '#header' => array(
      'title' => t("Bin"),
      'empty' => t("Empty"),
      'class' => t("Driver"),
    ),
  );
  $form['bin']['actions']['#type'] = 'actions';
  $form['bin']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Empty selected"),
    '#submit' => array('devcontrol_admin_form_submit_bins_empty_selected'),
  );

  $form['missing_modules'] = array(
    '#type' => 'fieldset',
    '#title' => t("Missing modules"),
    '#collapsible' => true,
    '#collapsed' => true,
  );
  $form['missing_modules']['help'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t("Missing modules are modules that are referenced into the <cite>system</cite> database table but which don't exist anymore in database (files have been physically suppressed). They are known to trigger Drupal core bugs, such as excessive file system scans during runtime."),
  );
  $options = array();
  $list = db_query("SELECT name, filename, type FROM {system}")->fetchAll();
  foreach ($list as $data) {
    if (!file_exists($data->filename)) {
      $options[$data->name]['name']['data'] = array('#markup' => $data->name);
      $options[$data->name]['type']['data'] = array('#markup' => $data->type);
      $options[$data->name]['basename']['data'] = array('#markup' => dirname($data->filename));
    }
  }
  if (!empty($options)) {
    $form['missing_modules']['data'] = array(
      '#type' => 'tableselect',
      '#options' => $options,
      '#header' => array(
        'name' => t("Name"),
        'type' => t("Type"),
        'basename' => t("Missing folder"),
      ),
    );
    $form['missing_modules']['#title'] .= ' (' . count($options) . ')';
    $form['missing_modules']['actions']['#type'] = 'actions';
    $form['missing_modules']['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t("Remove selected"),
      '#submit' => array('devcontrol_admin_form_submit_missing_modules_remove'),
    );
  } else {
    $form['missing_modules']['data'] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => t("Your system is clean."),
    );
  }

  return $form;
}

/**
 * Clear caches.
 */
function devcontrol_admin_form_submit_cache_clear($form, &$form_state) {

  // Find the triggering button.
  if (!empty($form_state['triggering_element']['#array_parents'])) {
    $key = $form_state['triggering_element']['#array_parents'];
    $key = array_pop($key);

    $info = devcontrol_cache_clear_info_get();
    if (isset($info[$key])) {
      $message = call_user_func($info[$key]['callback']);
      if (!empty($message)) {
        drupal_set_message($message);
      }
    }
  }
}

/**
 * Clear selected caches.
 */
function devcontrol_admin_form_submit_cache_clear_selected($form, &$form_state) {
  $bins     = $form_state['values']['cache_select']['bins'];
  $remember = $form_state['values']['cache_select']['remember'];

  if (empty($bins)) {
    drupal_set_message(t("Cannot empty nothing"), 'error');
    return;
  }

  if ($remember) {
    devcontrol_cookie_set(DEVCONTROL_CACHE_COOKIE, implode(',', $bins));
  }

  $info = devcontrol_cache_clear_info_get();
  foreach ($bins as $key) {
    if (isset($info[$key])) {
      $message = call_user_func($info[$key]['callback']);
      if (!empty($message)) {
        drupal_set_message($message);
      }
    }
  }
}

function devcontrol_admin_form_submit_developer_give_me_a_cookie($form, &$form_state) {
  devcontrol_access_give_cookie();
  drupal_set_message(t("You have been given a cookie!"));
}

function devcontrol_admin_form_submit_developer_remove_my_cookie($form, &$form_state) {
  devcontrol_cookie_drop(DEVCONTROL_COOKIE_NAME);
}

function devcontrol_admin_form_submit_bins_empty_selected($form, &$form_state) {
  $deleted = array();
  foreach ($form_state['values']['bin']['bins'] as $bin => $selected) {
    if ($bin === $selected) {
      $deleted[] = $bin;
      $object = _cache_get_object($bin);
      // Also clear the table in case.
      if (!$object instanceof DrupalDatabaseCache) {
        db_truncate($bin);
      }
      $object->clear('*', true);
    }
  }
  drupal_set_message(t("Cleared %bins cache bins", array('%bins' => implode(', ', $deleted))));
}

function devcontrol_admin_form_submit_missing_modules_remove($form, &$form_state) {
  $deleted = array();
  foreach ($form_state['values']['missing_modules']['data'] as $name => $selected) {
    if ($name === $selected) {
      $deleted[] = $name;
    }
  }
  db_delete('system')->condition('name', $deleted)->execute();
  drupal_set_message(t("Removed %names from system table", array('%names' => implode(', ', $deleted))));
}


/**
 * Caught mail container form.
 */
function devcontrol_admin_mail_ouput_form($form, &$form_state) {

  $mailer = new DevControl_FileMailSystem();
  $path = $mailer->getOutputDirectory();

  $iterator = new FilesystemIterator($path,
    FilesystemIterator::KEY_AS_FILENAME |
    FilesystemIterator::CURRENT_AS_FILEINFO |
    FilesystemIterator::SKIP_DOTS);

  $iterator = new RegexIterator($iterator, '/^[^\.].*/',
    RegexIterator::MATCH, RegexIterator::USE_KEY);

  $options = array();
  foreach ($iterator as $filename => $fileinfo) {
    // Always true, but enable autocompletion
    if ($fileinfo instanceof SplFileInfo) {

      // See mail ajax link.
      /*
      $link = array(
        '#type' => 'link',
        '#title' => $title,
        '#href' => 'profil/commandes/' . $orderId . '/' . $lineItemId . '/comment/ajax',
        '#attributes' => array(
          'class' => array('use-ajax'),
        ),
        '#ajax' => array(
          'progress' => 'throbber',
          'effect' => 'slide',
        ),
      );
       */

      $options[$filename] = array(
        'name'  => check_plain($filename),
        /* 'link'  => drupal_render($link), */
        'size'  => $fileinfo->getSize(),
        'ctime' => format_date($fileinfo->getCTime(), 'short'),
      );
    }
  }

  $form['mailing']['data'] = array(
    '#type' => 'tableselect',
    '#options' => $options,
    '#header' => array(
      'name' => t("filename"),
      /* 'link' => '', */
      'size' => t("size"),
      'ctime' => t("ctime"),
    ),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t("Delete selected"),
    '#submit' => array('devcontrol_admin_mail_ouput_form_mailing_delete_submit'),
  );
  $form['actions']['test_mail'] = array(
    '#type' => 'submit',
    '#value' => t("Send test mail"),
    '#submit' => array('devcontrol_admin_mail_ouput_form_test_mail_submit'),
  );
  $form['actions']['test_mail_attachment'] = array(
    '#type' => 'submit',
    '#value' => t("Send test mail with attachment"),
    '#submit' => array('devcontrol_admin_mail_ouput_form_test_mail_attachment_submit'),
  );

  $form['dialog']['#markup'] = '<div id="ui-devcontrol-dialog"></div>';

  return $form;
}

/**
 * Delete selected mail submit handler.
 */
function devcontrol_admin_mail_ouput_form_mailing_delete_submit($form, &$form_state) {

  $mailer = new DevControl_FileMailSystem();
  $path = $mailer->getOutputDirectory();

  foreach ($form_state['values']['data'] as $filename => $enabled) {
    if (!empty($filename) && $filename === $enabled) {
      $realpath = $path . '/' . $filename;
      if (is_writable($realpath)) {
        if (unlink($realpath)) {
          drupal_set_message(t("%filename has been removed", array('%filename' => $filename)));
        } else {
          drupal_set_message(t("%filename cannot be deleted for an unknown reason", array('%filename' => $filename)), 'error');
        }
      } else {
        drupal_set_message(t("%filename cannot be deleted because file is readonly", array('%filename' => $filename)), 'error');
      }
    }
  }
}

/**
 * Test mail submit handler.
 */
function devcontrol_admin_mail_ouput_form_test_mail_submit($form, &$form_state) {
  drupal_mail('devcontrol', 'test', variable_get('site_mail'), 'en');
  drupal_set_message(t("Test mail sent"));
}

/**
 * Test mail submit handler.
 */
function devcontrol_admin_mail_ouput_form_test_mail_attachment_submit($form, &$form_state) {
  $params = array();

  $uri = 'temporary://decontrol-attachment.txt';
  if (!file_exists($uri)) {
    file_put_contents($uri, 'Testing mail attachment');
  }

  $params['attachments'][] = array(
    'uri'      => $uri,
    'filename' => basename($uri),
    'filesize' => filesize($uri),
    'filemime' => file_get_mimetype($uri),
  );

  $to = variable_get('site_mail');

  drupal_mail('devcontrol', 'test', $to, 'en', $params);
  drupal_set_message(t("Test mail with attachment sent to !to", array(
    '!to' => $to,
  )));
}

/**
 * Export variables form.
 */
function devcontrol_admin_export_variables_form($form, &$form_state) {

  $variables = db_select('variable', 'v')
    ->fields('v', array('name', 'value'))
    ->orderBy('v.name', 'ASC')
    ->execute()
    ->fetchAllKeyed();

  foreach ($variables as $name => $value) {

    $uvalue = unserialize($value);

    if (null === $uvalue) {
      $valueStr = '<em>null</em>';
    } else if (is_numeric($uvalue)) {
      $valueStr = (string)$uvalue;
    } else if (is_bool($uvalue)) {
      $valueStr = '<em>' . ($uvalue ? 'true' : 'false') . '</em>';
    } else if (is_string($uvalue)) {
      if (false !== ($pos = strpos($uvalue, "\n"))) {
        $uvalue = substr($uvalue, 0, $pos - 1);
      }
      if (strlen($uvalue) > 75) {
        $valueStr = '<pre>' . drupal_substr($uvalue, 0, 70) . ' [...]</pre>';
      }
      $valueStr = '<pre>' . $uvalue . '</pre>';
    } else {
      $valueStr = '<em>' . gettype($uvalue) . '</em>';
    }

    $options[$name] = array(
      'name'  => check_plain($name),
      'type'  => gettype($uvalue), 
      'value' => $valueStr,
      'ssize' => strlen($value),
    );
  }

  $form['variables']['variables'] = array(
    '#type' => 'tableselect',
    '#options' => $options,
    '#header' => array(
      'name' => t("name"),
      'type' => t("type"),
      'value' => t("value"),
      'ssize' => t("size"),
    ),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['export'] = array(
    '#type' => 'submit',
    '#value' => t("Export selected"),
    '#submit' => array('devcontrol_admin_variable_export_form_export_submit'),
    '#ajax' => array(
      // FIXME: Working in the js callback creates a nojs downgrade problem.
      'callback' => 'devcontrol_admin_variable_export_form_export_js',
      'wrapper' => 'devcontrol-export',
    ),
  );

  $form['export']['#prefix'] = '<div id="devcontrol-export">';
  $form['export']['#suffix'] = '</div>';

  return $form;
}

/**
 * Export variables JS callback.
 */
function devcontrol_admin_variable_export_form_export_js($form, &$form_state) {
  $export = array_filter($form_state['values']['variables']);

  $variables = db_select('variable', 'v')
    ->fields('v', array('name', 'value'))
    ->condition('v.name', $export, 'IN')
    ->orderBy('v.name', 'ASC')
    ->execute()
    ->fetchAllKeyed();

  foreach ($variables as $name => $value) {
    $variables[$name] = unserialize($value);
  }

  $form['export']['#markup'] = '<pre>// Generated by devcontrol' . "\n"
      . '$variables = ' . var_export($variables, true) . ';</pre>';

  return $form['export'];
}

/**
 * Export variables submit handler.
 */
function devcontrol_admin_variable_export_form_export_submit($form, &$form_state) {}

/**
 * Field export form.
 */
function devcontrol_admin_export_fields_form($form, &$form_state) {

  // Fetch all fields list.
return $form;
  foreach ($variables as $name => $value) {

    $uvalue = unserialize($value);

    if (null === $uvalue) {
      $valueStr = '<em>null</em>';
    } else if (is_numeric($uvalue)) {
      $valueStr = (string)$uvalue;
    } else if (is_bool($uvalue)) {
      $valueStr = '<em>' . ($uvalue ? 'true' : 'false') . '</em>';
    } else if (is_string($uvalue)) {
      if (false !== ($pos = strpos($uvalue, "\n"))) {
        $uvalue = substr($uvalue, 0, $pos - 1);
      }
      if (strlen($uvalue) > 75) {
        $valueStr = '<pre>' . drupal_substr($uvalue, 0, 70) . ' [...]</pre>';
      }
      $valueStr = '<pre>' . $uvalue . '</pre>';
    } else {
      $valueStr = '<em>' . gettype($uvalue) . '</em>';
    }

    $options[$name] = array(
      'name'  => check_plain($name),
      'type'  => gettype($uvalue), 
      'value' => $valueStr,
      'ssize' => strlen($value),
    );
  }

  $form['variables']['variables'] = array(
    '#type' => 'tableselect',
    '#options' => $options,
    '#header' => array(
      'name' => t("name"),
      'type' => t("type"),
      'value' => t("value"),
      'ssize' => t("size"),
    ),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['export'] = array(
    '#type' => 'submit',
    '#value' => t("Export selected"),
    '#submit' => array('devcontrol_admin_variable_export_form_export_submit'),
    '#ajax' => array(
      // FIXME: Working in the js callback creates a nojs downgrade problem.
      'callback' => 'devcontrol_admin_variable_export_form_export_js',
      'wrapper' => 'devcontrol-export',
    ),
  );

  $form['export']['#prefix'] = '<div id="devcontrol-export">';
  $form['export']['#suffix'] = '</div>';

  return $form;
}

/**
 * Settings form.
 */
function devcontrol_admin_settings_form($form, &$form_state) {

  $form[DEVCONTROL_VAR_ENV] = array(
    '#title'         => t("Environment name"),
    '#type'          => 'textfield',
    '#default_value' => variable_get(DEVCONTROL_VAR_ENV),
    '#description'   => t("This is the name you can see in the developer fixed indicator. Per default it will display the machine hostname."),
  );

  $form[DEVCONTROL_VAR_BAR_THEME] = array(
    '#title'         => t("Bar theme"),
    '#type'          => 'select',
    '#options'       => devcontrol_bar_theme_list(),
    '#default_value' => variable_get(DEVCONTROL_VAR_BAR_THEME, 'nice'),
    '#description'   => t("Theme to use for the sidebar."),
  );

  return system_settings_form($form);
}
