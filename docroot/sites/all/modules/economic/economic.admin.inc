<?php
/**
 * @file
 * In this file all the admin actions is defined.
 */

/**
 * Form constructor for global module settings form.
 *
 * @see economic_config_form_validate()
 */
function economic_config_form($form, &$form_state) {
  $form = array();

  $form['config'] = array(
    '#weight' => 0,
    '#tree' => 0,
    '#title' => t('Configuration'),
    '#collapsible' => 1,
    '#type' => 'fieldset',
    '#collapsed' => 0,
    '#group' => 'economic',
  );

  $tmp_logo_string = '<div class="elogo">' . t('open e-conomic') . '</div>';
  $form['config']['logo'] = array(
    '#type' => 'item',
    '#markup' => l($tmp_logo_string, 'https://login.e-conomic.com/global', array(
      'html' => TRUE,
      'attributes' => array('target' => '_blank'),
    )),
  );
  $form['config']['logo']['#attached']['css'][] = drupal_get_path('module', 'economic') . '/theme/css/economic_settings_form.css';

  $economic_account = array(
    'account' => variable_get('economic_config_account', ''),
    'username' => variable_get('economic_config_username', ''),
    'password' => variable_get('economic_config_password', ''),
  );

  $form['config']['economic_config_account'] = array(
    '#required' => 1,
    '#default_value' => $economic_account['account'],
    '#weight' => 4,
    '#type' => 'textfield',
    '#title' => t('Account'),
    '#description' => t('The account given to you by e-conomic'),
  );

  $form['config']['economic_config_username'] = array(
    '#required' => 1,
    '#default_value' => $economic_account['username'],
    '#weight' => 5,
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('The username given to you by e-conomic with your account'),
  );

  $form['config']['economic_config_password'] = array(
    '#required' => 1,
    '#default_value' => $economic_account['password'],
    '#weight' => 10,
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('The password given to you by e-conomic with your account'),
  );

  if (!empty($economic_account['account']) and !empty($economic_account['username']) and !empty($economic_account['password'])) {
    $connected = FALSE;

    if ($economic = _economic_service_connect($economic_account)) {
      $connected = TRUE;
    }
    else {
      drupal_set_message(t('Error: The e-conomic account is not valid or the connection to webservice is broken'), 'warning');
    }

    if (!empty($connected)) {
      /*
       * Debitor
       */
      $form['config_debitor'] = array(
        '#weight' => 1,
        '#title' => t('Debitor settings'),
        '#collapsible' => 1,
        '#type' => 'fieldset',
        '#collapsed' => 1,
        '#description' => t('Need to create an new debitor account') . '<br/><br/>',
      );

      $db_or = db_or();
      $db_or->condition('fci.bundle', array('billing', 'commerce_product'), 'IN');
      $db_or->condition('fci.entity_type', 'commerce_product', '=');

      $que_fields = db_select('field_config_instance', 'fci')->fields('fci', array('field_name', 'bundle'))->condition($db_or);
      $res_fields = $que_fields->execute()->fetchAll();
      $field_options = array(
        'commerce_product' => array('' => t('Ignore this field')),
        'billing' => array('' => t('Select field from billig profile')),
      );

      if (!empty($res_fields)) {
        foreach ($res_fields as $r_row) {
          switch ($r_row->bundle) {
            case "billing":
              $r_row->field_name = str_replace('field_', '', $r_row->field_name);
              break;

            default:
              $r_row->bundle = 'commerce_product';
              break;

          }

          if (empty($field_options[$r_row->bundle])) {
            $field_options[$r_row->bundle] = array();
          }
          $field_options[$r_row->bundle][$r_row->field_name] = t('Field') . ': ' . $r_row->field_name;
        }
      }

      $form['config_debitor']['economic_config_debitor_field'] = array(
        '#required' => 1,
        '#default_value' => variable_get('economic_config_debitor_field', 'phone_number'),
        '#weight' => 10,
        '#type' => 'select',
        '#title' => t('Phone number field'),
        '#description' => t(
          'The field where the user types in phone number or account id, add the field !field_link',
          array(
            '!field_link' => l(t('here'), 'admin/commerce/customer-profiles/types/billing/fields'),
          )
        ),
        '#options' => (!empty($field_options['billing']) ? $field_options['billing'] : array()),
      );

      $form['config_debitor']['economic_config_debitor_group_id'] = array(
        '#required' => 1,
        '#default_value' => variable_get('economic_config_debitor_group_id', 1),
        '#weight' => 20,
        '#type' => 'radios',
        '#title' => t('Debitor group'),
        '#options' => ($connected ? $economic->getDebitors() : array()),
      );

      $form['config_debitor']['economic_config_debitor_payment_terms'] = array(
        '#required' => 1,
        '#default_value' => variable_get('economic_config_debitor_payment_terms', 'Betalt via webshop'),
        '#weight' => 25,
        '#type' => 'radios',
        '#title' => t('Debitor payment terms'),
        '#options' => ($connected ? $economic->getTerms() : array()),
      );

      /*
       * Products
       */
      $form['config_products'] = array(
        '#weight' => 10,
        '#title' => t('Product settings'),
        '#collapsible' => 1,
        '#type' => 'fieldset',
        '#collapsed' => 1,
      );

      $form['config_products']['economic_config_default_product_group'] = array(
        '#required' => 0,
        '#default_value' => variable_get('economic_config_default_product_group', 1),
        '#weight' => 27,
        '#type' => 'radios',
        '#title' => t('Default product group'),
        '#description' => t('Need to transfer products not already created'),
        '#options' => ($connected ? $economic->getProductGroups() : array()),
      );

      $form['config_products']['economic_config_product_field_cost'] = array(
        '#required' => 0,
        '#default_value' => variable_get('economic_config_product_field_cost', 'field_cost_price'),
        '#weight' => 30,
        '#type' => 'select',
        '#title' => t('Cost price field'),
        '#description' => t('If the product cost price is to be transfered'),
        '#options' => (!empty($field_options['commerce_product']) ? $field_options['commerce_product'] : array()),
      );

      /*
       * Invoice config
       */
      $form['config_invoice'] = array(
        '#weight' => 20,
        '#title' => t('Invoice settings'),
        '#collapsible' => 1,
        '#type' => 'fieldset',
        '#collapsed' => 1,
      );
      $form['config_invoice']['economic_config_template_id'] = array(
        '#required' => 0,
        '#default_value' => variable_get('economic_config_template_id', ''),
        '#weight' => 40,
        '#type' => 'hidden',
        '#title' => t('Invoice template id'),
        '#description' => t('When you design your own or modify the existing template, we need the unique id of the template for the invoice to be created'),
      );
      $form['config_invoice']['economic_config_convert_to_invoice'] = array(
        '#required' => 0,
        '#default_value' => variable_get('economic_config_convert_to_invoice', 1),
        '#weight' => 30,
        '#type' => 'checkbox',
        '#title' => t('Convert to invoice'),
        '#description' => t('If checked the order will be converted to invoice after it is created'),
      );
    }
  }
  else {
    drupal_set_message(t('You need to save the account details before you can set all settings'), 'warning', FALSE);
  }
  $form['#validate'][] = 'economic_config_form_validate';

  return system_settings_form($form);
}

/**
 * Form validation handler for economic_config_form().
 *
 * @see economic_config_form()
 */
function economic_config_form_validate($form, &$form_state) {
  $values = ((!empty($form_state['values']) and is_array($form_state['values'])) ? $form_state['values'] : array());

  $economic_account = array(
    'account' => (!empty($values['economic_config_account']) ? $values['economic_config_account'] : ''),
    'username' => (!empty($values['economic_config_username']) ? $values['economic_config_username'] : ''),
    'password' => (!empty($values['economic_config_password']) ? $values['economic_config_password'] : ''),
  );

  if (empty($economic_account['account']) OR empty($economic_account['username']) OR empty($economic_account['password'])) {
    form_set_error(
      'economic_config_account',
      t('One or more e-conomic account information is invalid or missing.')
    );
  }
}

/**
 * Form constructor for commerce economic testing form.
 *
 * @see economic_test_form_submit()
 */
function economic_test_form($form, &$form_state) {
  $form['economic_test'] = array(
    '#weight' => 99,
    '#tree' => 0,
    '#title' => t('Test'),
    '#collapsible' => 0,
    '#type' => 'fieldset',
    '#group' => 'economic',
  );

  $form['economic_test']['order_id'] = array(
    '#required' => 0,
    '#default_value' => '',
    '#weight' => 1,
    '#type' => 'textfield',
    '#title' => t('Order id'),
    '#description' => t('Enter the order id to be tested'),
  );

  $form['economic_test']['buttons']['#weight'] = 30;
  $form['economic_test']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Run test'),
  );
  $form['#submit'][] = 'economic_test_form_submit';

  return $form;
}

/**
 * Submit handler for economic_test().
 *
 * @see economic_test_form()
 */
function economic_test_form_submit($form, &$form_state) {
  $values = ((!empty($form_state['values']) and is_array($form_state['values'])) ? $form_state['values'] : array());

  if (!empty($values['order_id']) and is_numeric($values['order_id'])) {
    $order_obj = commerce_order_load($values['order_id']);
    if (!empty($order_obj)) {
      $order_obj->test_transfer = TRUE;

      economic_order_transfer($order_obj);
    }
    else {
      drupal_set_message(t('The order could not be found'), 'error');
    }
  }
  else {
    drupal_set_message(t('The order id is not valid'), 'error');
  }
}

/**
 * Commerce e-conomic rules overview.
 */
function economic_rules_form($form, &$form_state) {

  /*
   * Uses rulses api to allow the user to change only rules for this module
   */
  RulesPluginUI::$basePath = 'admin/config/workflow/rules/reaction';
  $options = array('show plugin' => FALSE);

  $content['enabled']['title']['#markup'] = '<b>' . t('Enabled e-conomic rules') . '</b>';

  $conditions = array(
    'event' => 'commerce_order_update',
    'plugin' => 'reaction rule',
    'active' => TRUE,
    'tags' => array('e-conomic'),
  );
  $content['enabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['enabled']['rules']['#empty'] = t('There are no active e-conomic rules.');

  $content['disabled']['title']['#markup'] = '<b>' . t('Disabled e-conomic rules') . '</b>';

  $conditions['active'] = FALSE;
  $content['disabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['disabled']['rules']['#empty'] = t('There are no disabled e-conomic rules.');

  $content['#page_callback'] = 'economic_rules';

  return $content;
}

/**
 * Form constructor for log viewing form.
 */
function economic_log_view_form($form, &$form_state, $log_id) {
  $convert_to_invoice = variable_get('economic_config_convert_to_invoice', 0);

  $log_id = ((!empty($log_id) and is_numeric($log_id)) ? $log_id : 0);
  $query = db_query('SELECT el.* FROM {economic_log} el WHERE (log_id = :log_id)', array(':log_id' => $log_id));

  $log_details = $query->fetchAssoc();
  $data = array();
  if (!empty($log_details)) {
    $log_detail_fields = array(
      'log_id' => t('Log id'),
      'order_id' => t('Order id'),
      'status' => t('Status'),
      'last_update' => t('Date'),
      'created' => t('Created on'),
      'economic_order_id' => t('e-conomic order id'),
      'economic_debitor_refere' => t('e-conomic debitor refere'),
    );
    if ($convert_to_invoice == 1) {
      $log_detail_fields['economic_invoice_id'] = t('e-conomic invoice id');
    }
    $log_detail_fields['response_data'] = t('Response data');
    $log_detail_fields['request_data'] = t('Request data');

    foreach ($log_detail_fields as $f_name => $f_title) {
      $f_value = (!empty($log_details[$f_name]) ? $log_details[$f_name] : '');

      switch ($f_name) {
        case "response_data":
          $json_value = (!empty($f_value) ? json_decode($f_value, TRUE) : array());

          $f_value = '';
          if (!empty($json_value) and is_array($json_value)) {
            if (!empty($json_value['faultstring'])) {
              $f_value = check_plain($json_value['faultstring']);
            }
            else {
              foreach ($json_value as $responce_name => $responce_value) {
                $f_value .= '<div><b>' . $responce_name . '</b>: ' . check_plain($responce_value) . '</div>';
              }
            }
          }

          break;

        case "request_data":
          $f_value = '<pre>' . check_plain(print_r(json_decode($f_value), TRUE)) . '</pre>';
          break;

        case "created":
        case "last_update":
          $f_value = format_date($f_value, 'custom', 'd.m.Y H:i');
          break;

        case "status":
          $f_value = _economic_statuses($f_value);
          break;

      }

      $data[] = array(
        'data' => array(
          array(
            'data' => $f_title,
            'class' => array('active'),
            'style' => 'width:200px;vertical-align:top;',
          ),
          array('data' => $f_value),
        ),
      );
    }
  }

  $form['log_view'] = array(
    '#theme' => 'table',
    '#rows' => $data,
    '#attributes' => array(),
    '#empty' => t('Log not found'),
  );

  return $form;
}

/**
 * Commerce e-conomic statuses definitions.
 *
 * @param int $status_nr
 *   The numeric value of one status.
 */
function _economic_statuses($status_nr = 0) {
  $status_array = array(
    1 => t('Pending'),
    2 => t('Transfered'),
    3 => t('Converted to invoice'),
    7 => t('Error in transfer'),
  );

  if (!empty($status_nr)) {
    return (!empty($status_array[$status_nr]) ? $status_array[$status_nr] : $status_nr);
  }
  else {
    return $status_array;
  }
}

/**
 * Form constructor for the transfer log overview form.
 */
function economic_logs_form($form, &$form_state) {
  $data = array();
  $status_array = _economic_statuses();

  $economic_account = array(
    'convert_to_invoice' => variable_get('economic_config_convert_to_invoice', 0),
  );

  $que_log = db_select('economic_log', 'el');
  $que_log->fields('el', array(
    'log_id',
    'order_id',
    'economic_order_id',
    'economic_debitor_refere',
    'economic_invoice_id',
    'last_update',
    'status',
  ));
  $que_log->orderBy('el.last_update', 'DESC');
  $que_log = $que_log->extend('PagerDefault')->limit(25);
  $logs = $que_log->execute()->fetchAll();

  if (!empty($logs) and is_array($logs)) {
    foreach ($logs as $l_row) {
      $tmp_url = 'admin/commerce/config/economic/logs/' . $l_row->log_id;

      $convert_link = '';
      if (empty($l_row->economic_invoice_id) and !empty($economic_account['convert_to_invoice']) and $economic_account['convert_to_invoice'] == 1) {
        $convert_link = l(t('Convert to invoice'), 'admin/commerce/config/economic/invoice/' . $l_row->economic_order_id);
      }

      $tmp_data = array(
        'data' => array(
          array('data' => l($l_row->log_id, $tmp_url)),
          array('data' => $l_row->order_id),
          array(
            'data' => (!empty($status_array[$l_row->status]) ? $status_array[$l_row->status] : $l_row->status),
            'class' => array(
              'active',
            ),
          ),
          array(
            'data' => format_date($l_row->last_update, 'custom', 'd.m.Y H:i'),
          ),
          array(
            'data' => (!empty($l_row->economic_order_id) ? $l_row->economic_order_id : ''),
          ),
          array('data' => (!empty($l_row->economic_invoice_id) ? $l_row->economic_invoice_id : '')),
          array('data' => $l_row->economic_debitor_refere),
          array('data' => l(t('Show log'), $tmp_url)),
          array(
            'data' => l(t('Edit order'), 'admin/commerce/orders/' . $l_row->order_id . '/edit'),
          ),
          $convert_link,
        ),
      );

      if ($l_row->status == 7) {
        $tmp_data['class'][] = 'dblog-error';
      }

      $data[] = $tmp_data;
    }
  }

  $header = array(
    array('data' => '#'),
    array('data' => t('Refere')),
    array('data' => t('Status')),
    array('data' => t('Date')),
    array('data' => t('e-conomic order id')),
    array('data' => t('e-conomic invoice id')),
    array('data' => t('Client refere')),
    array('data' => t('Operations'), 'colspan' => 3),
  );

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $data,
    '#attributes' => array(),
    '#empty' => t('No logs created yet'),
    '#sticky' => TRUE,
  );
  $form['pager'] = array(
    '#theme' => 'pager',
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'dblog') . '/dblog.css';
  $form['#attached']['css'][] = drupal_get_path('module', 'economic') . '/theme/css/economic_settings_form.css';

  return $form;
}

/**
 * Convert order to invoice.
 */
function economic_convert_order_to_invoice($form, &$form_state, $order_id) {
  $economic_account = array(
    'account' => variable_get('economic_config_account', ''),
    'username' => variable_get('economic_config_username', ''),
    'password' => variable_get('economic_config_password', ''),
    'template_id' => variable_get('economic_config_template_id', 0),
    'debitor_group_id' => variable_get('economic_config_debitor_group_id', 1),
    'debitor_payment_terms' => variable_get('economic_config_debitor_payment_terms', ''),
    'convert_to_invoice' => variable_get('economic_config_convert_to_invoice', 0),
    'debitor_anonymous_number' => variable_get('economic_config_debitor_anonymous_number', 0),
    'default_product_group' => variable_get('economic_config_default_product_group', 1),
    'debitor_field' => 'billing_' . variable_get('economic_config_debitor_field', 'phone_number'),
    'product_field_cost' => variable_get('economic_config_product_field_cost', ''),
  );

  $economic = new EconomicHelper($economic_account['account'], $economic_account['username'], $economic_account['password']);
  $economic->setTemplate($economic_account['template_id']);

  if (!empty($economic_account['convert_to_invoice']) and $economic_account['convert_to_invoice'] == 1) {
    try {
      $economic_invoice_id = $economic->upgradeOrderToInvoice($order_id);
      drupal_set_message(t('Order converted to invoice: @nr', array('@nr' => $economic_invoice_id)));
    }
    catch (Exception $e) {
      drupal_set_message(t('The order was not converted, due to an unexpected error'), 'warning');
    }
  }

  drupal_goto('admin/commerce/config/economic/logs');
  drupal_exit();
}
