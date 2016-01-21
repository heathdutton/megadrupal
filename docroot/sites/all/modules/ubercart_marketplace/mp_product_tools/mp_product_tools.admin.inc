<?php

/**
 * @file
 * Settings and config for mp_product_tools module.
 */
function mp_product_tools_admin_settings($form, &$form_state) {
  // Get a list of all product classes.
  //$result = db_query('SELECT pcid, name from {uc_product_classes}')->fetchAllAssoc('pcid', PDO::FETCH_ASSOC);
  //foreach ($result as $classes) {
  //  $productclasses[$classes['pcid']] = $classes['name'];
  //}
  $productclasses = db_query('SELECT pcid, name from {uc_product_classes}')->fetchAllKeyed(0, 1);

  // Add entry for Default Products to end of the list:
  $productclasses['product'] = 'Default Ubercart Product Content Type';

  // Get a list of enabled classes.
  //$result = db_query('SELECT pcid, enabled from {mp_product_tools}')->fetchAllAssoc('pcid', PDO::FETCH_ASSOC);
  //foreach ($result as $classes) {
  //  $enabledclasses[$classes['pcid']] = $classes['enabled'];
  //}
  $enabledclasses = db_query('SELECT pcid, enabled from {mp_product_tools}')->fetchAllKeyed(0, 1);

  $form = array();
  $form['content'] = array(
    '#markup' => t('<p>"Marketplace Product Tools" provides an interface to tailor Marketplace products on a class by class basis.  It also provides the ability to enable or disable Marketplace functionality.</p>'),
  );
  $form['classes'] = array(
    '#type' => 'fieldset',
    '#title' => 'Product Classes',
  );

  $header = array(
    t('Product Class'),
    t('Marketplace Status'),
    t('Actions'),
  );

  foreach ($productclasses as $pcid => $classname) {
    $enabled = !empty($enabledclasses[$pcid]);
    $radio1 = '<input type=radio name=radio[' . $pcid . '] value=1';
    if ($enabled) {
      $radio1 .= ' checked="checked"';
    }
    $radio1 .= '> Enabled';
    $radio2 = '<input type=radio name=radio[' . $pcid . '] value=0';
    if (!$enabled) {
      $radio2 .= ' checked="checked"';
    }
    $radio2 .= '> Disabled';
    $tablerow[] = array(
      l($classname, 'admin/store/products/marketplace-tools/' . $pcid),
      $radio1 . '&nbsp;&nbsp;&nbsp;&nbsp;' . $radio2,
      l(t('Edit'), 'admin/store/products/marketplace-tools/' . $pcid),
    );
  }

  $form['classes']['table'] = array(
    '#markup' => theme('table', array('header' => $header, 'rows' => $tablerow)),
  );
  $form['classes']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  return $form;
}

/**
 * Submit handler for the mp_products_admin_settings form.
 */
function mp_product_tools_admin_settings_submit($form, &$form_state) {
  $enabled_classes = array();
  foreach ($form_state['input']['radio'] as $key => $value) {
    // If class is disabled, check to see if it's in the db, if not add it as disabled, otherwise update it's status to disabled.
    if ($value == 0) {
      $result = db_query("SELECT * FROM {mp_product_tools} WHERE pcid = :pcid", array(':pcid' => $key))->fetchField();
      if (!$result) {
        db_insert('mp_product_tools')
          ->fields(array(
            'pcid' => $key,
            'enabled' => 0,
         ))
         ->execute();
      }
      else {
        db_update('mp_product_tools')
          ->fields(array(
            'enabled' => 0,
          ))
          ->condition('pcid', $key)
          ->execute();
      }
    }
    // If class is enabled...
    if ($value == 1) {
      // Get the enable classes so we can add them to our persistent variable, mp_product_tools_enabled_classes
      //$result = db_query("SELECT name FROM {uc_product_classes} WHERE pcid='%s'", $key);
      //$class = db_fetch_array($result);
      $enabled_classes[$key] = $key; // == 'product' ? 'Default Ubercart Product Content Type' : $class['name'];

      // ...insert into database if it's not already there.
      $result = db_query("SELECT * FROM {mp_product_tools} WHERE pcid = :pcid", array(':pcid' => $key))->fetchField();
      if (!$result) {
        db_insert('mp_product_tools')
          ->fields(array(
            'pcid' => $key,
            'enabled' => 1,
          ))
          ->execute();
      }
      else {
        db_update('mp_product_tools')
          ->fields(array(
            'enabled' => 1,
          ))
          ->condition('pcid', $key)
          ->execute();
      }
    }
  }

  variable_set('mp_product_tools_enabled_classes', $enabled_classes);

  drupal_set_message('Marketplace product class settings updated.');
}

/**
 * Product class settings form.
 */
function mp_product_tools_class_settings($form, &$form_state) {
  $tmp = explode("/", $_GET['q']);
  $prodclass = $tmp[4];
  $class = ($prodclass != 'product') ? db_query("SELECT name from {uc_product_classes} WHERE pcid = :pcid", array(':pcid' => $prodclass))->fetchField() : 'Default Ubercart Product Content Type';
  $sign_flag = variable_get('uc_sign_after_amount', FALSE);
  $currency_sign = variable_get('uc_currency_sign', '$');

  $form = array();
  $form['#tree'] = TRUE;
  $form['product_class'] = array(
    '#type' => 'hidden',
    '#value' => $prodclass,
  );
  $form['marketplace_tools'] = array(
    '#type' => 'fieldset',
    '#title' => 'Product Settings for "' . $class . '"',
    '#collapsible' => FALSE,
  );

  $form['marketplace_tools']['commission'] = array(
    '#type' => 'fieldset',
    '#title' => 'Commission Settings',
    '#collapsible' => TRUE,
    '#description' => t('Commission by role'),
  );

  $roles_array = user_roles(true, 'act as seller');
  foreach ($roles_array as $role) {
    $form['marketplace_tools']['commission'][$role] = array(
      '#type' => 'textfield',
      '#title' => $role,
      '#size' => 10,
      '#default_value' => db_query("SELECT rate FROM {mp_seller_rates} WHERE rid = :rid AND class = :class", array(':rid' => array_search($role, $roles_array), ':class' => $prodclass))->fetchField(),
    );
  }

  if (module_exists('mp_file')) {
    $form['marketplace_tools']['enable_downloads'] = array(
      '#type' => 'checkbox',
      '#title' => 'Enable downloadable product features',
      //'#description' => 'Enable dowloadable products',
      '#default_value' => db_query("SELECT enabled FROM {mp_file_class_settings} WHERE pcid = :pcid", array(':pcid' => $prodclass))->fetchField(),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 10,
  );
  return $form;
}

/**
 * Submit handler for product class settings form.
 */
function mp_product_tools_class_settings_submit($form, &$form_state) {
  $roles_array = user_roles(true, 'act as seller');
  foreach ($roles_array as $role) {
    db_merge('mp_seller_rates')
      ->key(array(
        'rid' => array_search($role, $roles_array),
        'class' => $form_state['values']['product_class'],
      ))
      ->fields(array(
        'rate' => (float)$form_state['values']['marketplace_tools']['commission'][$role],
      ))
      ->execute();
  }

  if (module_exists('mp_file')) {
    db_merge('mp_file_class_settings')
      ->key(array(
        'pcid' => $form_state['values']['product_class'],
      ))
      ->fields(array(
        'enabled' => $form_state['values']['marketplace_tools']['enable_downloads'],
      ))
      ->execute();
  }

  drupal_set_message('Marketplace product settings saved.');
  drupal_goto('admin/store/products/marketplace-tools');
}
