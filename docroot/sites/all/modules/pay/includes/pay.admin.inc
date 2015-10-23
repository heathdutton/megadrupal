<?php

/**
 * @file
 * Administration form settings and related functions.
 */

/**
 * Payment settings overview.
 */
function pay_admin_overview() {
  $output = array();

  $settings = drupal_get_form('pay_admin_settings');
  $output['settings'] = $settings;
  return $output;
}

/**
 * Overall settings form for global payment options.
 */
function pay_admin_settings($form, &$form_state) {
  $list = array('' => t('Please select'));
  $list += pay_currency_list();

  $form = array();

  // Don't allow this to be changed if it has already been used and we don't
  // support multi-currency.  This would cause some forms and transactions
  // to have a different currency than others without support for conversion.
  $disabled = FALSE;
  if (!variable_get('pay_currency_multiple', FALSE)) {
    $disabled = $disabled || db_query("SELECT count(1)
      FROM {pay_transaction} WHERE currency IS NOT NULL AND currency != ''")->fetchField();
  }

  $form['pay_currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#description' => t('The default currency for all payment activities on this site. This cannot be changed once transactions have been created.'),
    '#options' => $list,
    '#required' => TRUE,
    '#default_value' => variable_get('pay_currency', NULL),
    '#disabled' => $disabled,
  );

  // system_settings_form trounces the default submit handler, so define one.
  $form['#submit'] = array('pay_admin_settings_submit');
  return system_settings_form($form);
}

/**
 * Submit handler for settings form.
 */
function pay_admin_settings_submit($form, &$form_state) {
  // No variable has been set yet, so this is the first time setting this value.
  // Set this default value on all pay_forms and pay_transactions.
  $currency = $form_state['values']['pay_currency'];
  if (!$count = db_query("SELECT count(1)
    FROM {pay_transaction} WHERE currency IS NOT NULL AND currency NOT IN ('', :currency)", array(':currency' => $currency))->fetchField()) {

    db_update('pay_form')
      ->fields(array(
          'currency' => $currency,
        ))
      ->execute();
    db_update('pay_transaction')
      ->fields(array(
          'currency' => $currency,
        ))
      ->execute();
  }
}

/**
 * List payment forms
 */
function pay_admin_pay_form_list($handler = NULL, $uid = NULL) {

  $hdrs = array(
    array(
      'data' => t('Title'),
      'field' => 'Title',
    ),
    array(
      'data' => t('Owner'),
      'field' => 'uid',
    ),
    array(
      'data' => t('Handler'),
      'field' => 'handler',
    ),
    array(
      'data' => t('Status'),
      'field' => 'status',
    ),
    array(
      'data' => t('Total'),
      'field' => 'total',
    ),
    array(
      'data' => t('Total paid'),
      'field' => 'total_paid',
    ),
    array(),
  );

  $query = db_select('pay_form', 'f')
    ->fields('f');
  $query->addExpression('SUM(t.total)', 'total');
  $query->addExpression('SUM(t.total_paid)', 'total_paid');

  $query->leftJoin('pay_transaction', 't', 'f.pfid = t.pfid');

  if ($handler) {
    $subclasses = array();
    foreach (pay_handlers('pay_form') as $name => $info) {
      if ($pay_form = pay_form_load($name)) {
        if (is_subclass_of($pay_form, $handler)) {
          $subclasses[] = $name;
        }
      }
    }
    if ($subclasses) {
      $subclasses[] = $handler;
      $query->condition('f.handler', $subclasses, 'in');
    }
    else {
      $query->condition('f.handler', $handler);
    }
    unset($hdrs[2]);
  }

  $query->groupBy('f.pfid');
  $query->extend('tableSort')->orderByHeader($hdrs);
  $query->extend('PagerDefault')->limit(20);
  $res = $query->execute();

  $path = $_GET['q'];
  $rows = array();
  $pay_forms = array();
  foreach ($res as $data) {
    $pay_form = pay_form_load($data);
    $row = array(
      $pay_form->menu_path() ? l($pay_form->title(), $pay_form->menu_path()) : $pay_form->title(),
      theme('username', array('account' => $pay_form->user())),
      $pay_form->handler_title(),
      $pay_form->status ? t('active') : t('disabled'),
      round($data->total),
      round($data->total_paid),
      l(t('edit'), $path . '/' . $pay_form->pfid . '/edit', array('query' => array('destination' => $path))),
    );
    if ($handler) {
      unset($row[2]);
    }
    $rows[] = $row;
  }
  if (empty($rows)) {
    $rows[] = array();
  }

  return theme('table', array('header' => $hdrs, 'rows' => $rows));
}

/**
 * API function: implement a settings form that may be used by a pay_form module.
 */
function pay_form_settings($form, &$form_state, $pay_form) {
  if (!is_object($pay_form)) {
    $pay_form = pay_form_load($pay_form);
  }
  
  $pay_form->settings_form($form, $form_state);
 
  return $form;
}

/*
 * Displays the payment method admin overview page.
 */
function pay_admin_method_overview() {

  $header = array(t('Name'), t('Type'), array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();
  $info = pay_handlers('pay_method');
  foreach (pay_form_load()->pay_method_list() as $pmid => $pay_method) {
    if (!$pay_method = pay_method_load($pmid)) {
      continue;
    }
    $row = array();
    $count = $pay_method->transactionCount();

    // Name column.
    $row[] = $pay_method->title();

    // Type/handler column.
    $row[] = $info[$pay_method->handler()]['title'];

    // Edit link.
    $row[] = l(t('edit'), 'admin/config/pay/method/' . $pmid . '/edit');

    // Disable/delete link.
    if ($count == 0) {
      $row[] = l(t('delete'), 'admin/config/pay/method/' . $pmid . '/delete');
    }
    else {
      $row[] = l(t('disable'), 'admin/config/pay/method/' . $pmid . '/delete');
    }

    $rows[] = $row;
  }

  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No payment methods available. <a href="@link">Add payment method</a>.', array('@link' => url('admin/config/pay/method/add'))),
  );

  return $build;
}

/**
 * Admin form to create or update payment methods.
 */
function pay_admin_method_form($form, $form_state, $pay_method = NULL) {
  if (isset($form_state['storage']['pay_method'])) {
    $pay_method = $form_state['storage']['pay_method'];
  }
  if ($pay_method) {
    $form = array();
    if (!is_object($pay_method)) {
      $pay_method = pay_method_load($pay_method);
    }
    $pay_method->settings_form($form, $form_state);
    $pmid = isset($pay_method->pmid) ? $pay_method->pmid : get_class($pay_method);
    $form['#pay_method'] = $pmid;
  }
  else {
    $form['pay_method'] = array(
      '#type' => 'radios',
      '#title' => t('Payment handler'),
    );
    $options = array();
    foreach (pay_handlers('pay_method') as $name => $info) {
      if (empty($info['title'])) {
        continue; // Exclude unnamed base classes.
      }
      $options[$name] = $name;
      $form['pay_method'][$name] = array(
        '#type' => 'radio',
        '#name' => 'pay_method',
        '#title' => $info['title'],
        '#description' => $info['description'],
        '#return_value' => $name,
      );
    }
    $form['pay_method']['#options'] = $options;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save values'),
  );

  // @TODO: check this still works.
  // Vertical tab-ify this form if vertical_tabs module is installed.
  $form['#pre_render'][] = 'vertical_tabs_form_pre_render';
  return $form;
}

/**
 * Submit handler for pay_method_settings.
 */
function pay_admin_method_form_submit($form, &$form_state) {
  if (empty($form['#pay_method'])) {
    if ($form_state['values']['pay_method']) {
      if (empty($form_state['storage'])) {
        $form_state['storage'] = $form_state['values'];
        $form_state['rebuild'] = TRUE;
        return;
      }
    }
  }
  else {
    $form_state['redirect'] = 'admin/config/pay/method';
    unset($form_state['rebuild'], $form_state['storage']);
  }
}

/**
 * Menu callback; delete a payment method.
 */
function pay_method_delete_confirm($form, &$form_state, $pay_method) {
  $form['pmid'] = array('#type' => 'value', '#value' => $pay_method->pmid);

  if ($pay_method->transactionCount() == 0) {
    $message = t('Are you sure you want to delete the payment method %name?', array('%name' => $pay_method->title()));
    $caption = '<p>' . t('This action cannot be undone.') . '</p>';
    $button  = t('Delete');
  }
  else {
    $message = t('Are you sure you want to disable the payment method %name?', array('%name' => $pay_method->title()));
    $caption = '<p>' . t('Because there are existing transactions using this payment method it is not possible to delete it. Disabling it will remove it from listings and prevent it from being used for future transactions.') . '</p>';
    $button  = t('Disable');
  }

  return confirm_form($form, $message, 'admin/config/pay/method', $caption, $button);
}

/**
 * Process payment method delete confirm submissions.
 */
function pay_method_delete_confirm_submit($form, &$form_state) {
  $pay_method = pay_method_load($form_state['values']['pmid']);
  
  // The delete method will automatically revert to disable.
  $pay_method->delete();

  $t_args = array('%name' => $pay_method->title());
  drupal_set_message(t('The payment method %name has been deleted.', $t_args));
  watchdog('menu', 'Deleted payment method %name.', $t_args, WATCHDOG_NOTICE);

  menu_rebuild();

  $form_state['redirect'] = 'admin/config/pay/method';
  return;
}
