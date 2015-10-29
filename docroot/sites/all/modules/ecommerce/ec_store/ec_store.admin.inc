<?php

/**
 * @file
 * All store admin functions.
 */

/**
 * Main store settings page, at 'admin/config/store/store'.
 */
function ec_store_ec_settings($form, &$form_state) {
  $site_mail = variable_get('site_mail', ini_get('sendmail_from'));
  if (empty($site_mail)) {
    $site_mail = 'no e-mail address found';
  }

  $form['ec_display_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display email address for non-anonymous customers'),
    '#default_value' => variable_get('ec_display_email', 1),
  );

  // Links settings.
  $form['ec_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['ec_links']['ec_goto_cart_empty'] = array(
    '#type' => 'textfield',
    '#title' => t('Empty cart link'),
    '#default_value' => variable_get('ec_goto_cart_empty', ''),
    '#description' => t('If the user cart is empty, you can send him/her to a given page.'),
  );

  return system_settings_form($form);
}

/**
 * Configure all workflow statuses.
 */
function ec_store_workflow_settings($form, &$form_state) {
  $form = array();

  $workflow = ec_store_transaction_workflow('types');
  $form['ec_store_workflow'] = array(
    '#tree' => TRUE,
  );

  foreach ($workflow as $key => $item) {
    $form['ec_store_workflow'][$key]['workflow'] = array(
      '#type' => 'value',
      '#value' => $item['workflow'],
    );
    $form['ec_store_workflow'][$key]['description'] = array(
      '#markup' => $item['description'],
    );
    $options = array(
      EC_WORKFLOW_TYPE_IN_PROGRESS => t('In progress'),
      EC_WORKFLOW_TYPE_COMPLETE => t('Complete'),
      EC_WORKFLOW_TYPE_CANCEL => t('Cancel'),
    );
    $form['ec_store_workflow'][$key]['type'] = array(
      '#markup' => $options[$item['type']],
    );
    $form['ec_store_workflow'][$key]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $item['weight'],
      '#delta' => count($workflow) + 1,
    );
    $form['ec_store_workflow'][$key]['operations'] = array(
      '#markup' => l(t('edit'), 'admin/config/store/store/workflow/' . $item['workflow'] . '/edit') . ' ' . l(t('delete'), 'admin/config/store/store/workflow/' . $item['workflow'] . '/delete'),
    );
  }

  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_workflow_settings_submit(&$form, &$form_state) {
  foreach ($form_state['values']['ec_store_workflow'] as $record) {
    drupal_write_record('ec_workflow_statuses', $record, 'workflow');
  }
  drupal_set_message(t('Workflow settings updated'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_workflow_settings_edit($form, $form_state, $workflowid = NULL) {
  $form = array();

  if ($workflowid) {
    $workflow = ec_store_transaction_workflow('types');
    if (isset($workflow[$workflowid])) {
      $item = $workflow[$workflowid];

      $form['workflow'] = array(
        '#type' => 'value',
        '#value' => $item['workflow'],
      );
    }
    else {
      drupal_not_found();
    }
  }
  else {
    $item = array();
  }

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => isset($item['description']) ? $item['description'] : NULL,
    '#description' => t('Provide a short descriptions which will be used to identify which workflow state the transaction is in'),
  );

  $form['help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help'),
    '#default_value' => isset($item['help']) ? $item['help'] : NULL,
    '#description' => t('Provide information on what the stage of the transaction workflow should be done.'),
  );

  $options = array(
    EC_WORKFLOW_TYPE_IN_PROGRESS => t('In progress'),
    EC_WORKFLOW_TYPE_COMPLETE => t('Complete'),
    EC_WORKFLOW_TYPE_CANCEL => t('Cancel'),
  );
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type of workflow'),
    '#default_value' => isset($item['type']) ? $item['type'] : NULL,
    '#options' => $options,
    '#description' => t('Provide type of workflow which will allow the system to idenitify the general purpose of the workflow.'),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  if (!empty($item)) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('ec_store_workflow_settings_edit_submit_delete'),
    );
  }
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_workflow_settings_edit_submit(&$form, &$form_state) {
  if (isset($form_state['values']['workflow'])) {
    drupal_write_record('ec_workflow_statuses', $form_state['values'], 'workflow');
  }
  else {
    drupal_write_record('ec_workflow_statuses', $form_state['values']);
  }
  drupal_set_message(t('Workflow %name has been updated', array('%name' => $form_state['values']['description'])));
  $form_state['redirect'] = 'admin/config/store/store/workflow';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_workflow_settings_edit_submit_delete(&$form, &$form_state) {
  drupal_goto('admin/config/store/store/workflow/' . $form_state['values']['workflow'] . '/delete');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_workflow_settings_delete($form, $form_state, $workflowid) {
  $form = array();

  if ($workflowid) {
    $workflow = ec_store_transaction_workflow('types');
    if (isset($workflow[$workflowid])) {
      $item = $workflow[$workflowid];

      $form['workflow'] = array(
        '#type' => 'value',
        '#value' => $item['workflow'],
      );
      $form['name'] = array(
        '#type' => 'value',
        '#value' => $item['description'],
      );
    }
    else {
      drupal_not_found();
    }
  }
  else {
    drupal_not_found();
  }

  return confirm_form($form, t('Do you really want to delete the %name workflow', array('%name' => $item['description'])), 'admin/config/store/store/workflow', t('This action is permanent and cannot be undone'), t('Delete'), t('Cancel'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_workflow_settings_delete_submit(&$form, &$form_state) {
  db_delete('ec_workflow_statuses')
    ->condition('workflow', $form_state['values']['workflow'])
    ->execute();
  drupal_set_message(t('Workflow %description has been deleted', array('%description' => $form_state['values']['name'])));
  $form_state['redirect'] = 'admin/config/store/store/workflow';
}

/**
 * Called by theme_store_invoice().
 */
function _theme_ec_store_invoice($txn, $print_mode = TRUE, $trial = FALSE) {
  global $base_url;

  $header = array();
  $row    = array();

  if (empty($txn->mail) && $txn->uid > 0) {
    $txn->mail = db_query('SELECT mail FROM {users} WHERE uid = :uid', array(':uid' => $txn->uid))->fetchField();
  }

  if (!empty($txn->items)) {
    $header = array(t('Quantity'), t('Item'), t('Price'));

    $shippable = FALSE;
    foreach ($txn->items as $p) {
      $prod = ec_product_load($p);
      if (product_is_shippable($p->vid)) {
        $shippable = TRUE;
      }
      $price = ec_store_adjust_misc($txn, $p);

      $subtotal += (product_has_quantity($p) ? $p->qty * $price : $price);
      $details = '';
      if (0 && is_array($p->data)) {
        foreach ($p->data as $key => $value) {
          if (!empty($value)) {
            $items[] = '<strong>' . check_plain($key) . ': </strong>' . check_plain($value);
          }
        }
        if (!empty($items)) {
          $details = theme('item_list', array('items' => $items));
        }
      }

      $row[] = array(
        array(
          'data' => $p->qty,
          'align' => 'center',
          'valign' => 'top',
        ),
        '<em>' . check_plain($p->title) . '</em> ' . (($prod->sku != '') ?
        '[' . check_plain($prod->sku) . ']' : '') . '<br />' . $details,
        array(
          'data' => format_currency($price),
          'valign' => 'top',
          'align' => 'right',
        ),
      );
    }

    if (is_array($txn->misc)) {
      foreach ($txn->misc as $misc) {
        if (empty($misc->seen)) {
          $price = isset($misc->qty) ? $misc->price * $misc->qty : $misc->price;
          $row[] = array(array(
              'data' => "<strong>{$misc->description}</strong>: " .
            format_currency($price),
              'colspan' => 3,
              'align' => 'right',
            ));
        }
      }
    }

    $row[] = array(array(
        'data' => '<hr size="1" noshade="noshade" />',
        'colspan' => 3,
        'align' => 'right',
      ));
    $row[] = array(array(
        'data' => '<strong>' . t('Total:') . '</strong> ' .
      format_currency(ec_store_transaction_calc_gross($txn)),
        'colspan' => 3,
        'align' => 'right',
      ));
  }

  $payment_info  = '<div><strong>' . t('Ordered On:') . '</strong> ' .
    format_date($txn->created) . '</div>';
  if (!empty($txn->duedate)) {
    $payment_info .= '<div><strong>' . t('Due Date:') . '</strong> ' .
      format_date($txn->duedate) . '</div>';
  }
  $payment_info .= '<div><strong>' . t('Transaction ID:') . '</strong> ' .
    ($trial ? t('Trial Invoice - Not Yet Posted') : $txn->txnid) . '</div>';

  $css        = base_path() . drupal_get_path('module', 'store') . '/css/invoice.css';
  $site_name  = t('%site-name Invoice', array('%site-name' => variable_get('site_name', 'drupal')));

  module_load_include('inc', 'ec_store', 'ec_store');
  if ($shipping_to = theme('ec_store_format_address', array('txn' => $txn, 'type' => 'shipping', 'html' => 'html'))) {
    $shipping_label = t('Shipping to');
  }
  if ($billing_to = theme('ec_store_format_address', array('txn' => $txn, 'type' => 'billing', 'html' => 'html'))) {
    $billing_label = t('Billing to');
  }

  if (!empty($txn->ship)) {
    $shipping_method_label = t('Shipping method:');
    $shipping_method = ec_store_format_shipping_method($txn);
  }
  $email_label = t('E-mail:');
  $items_label = t('Items ordered');
  $items_view = theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('cellpadding' => 3, 'cellspacing' => 3)));

  $payment_label = t('Payment Info');

  if (!empty($print_mode)) {
    $output .= <<<EOD
<html>
  <head>
    <style type="text/css" media="all">@import url('$css');</style>
  </head>
  <body>
EOD;
  }

  $output .= <<<EOD
    <h1>$site_name</h1>

    <table cellspacing="5">
      <tr>
        <th align="left">$shipping_label</th>
        <th align="left">$billing_label</th>
      </tr>
      <tr>
        <td>$shipping_to</td>
        <td>$billing_to</td>
      </tr>
    </table>

    <p><strong>$shipping_method_label</strong> $shipping_method</p>
    <p><strong>$email_label</strong> $txn->mail</p>

    <h2>$items_label</h2>
    $items_view

    <h2>$payment_label</h2>
    $payment_info
EOD;

  if (!empty($print_mode)) {
    $output .= <<<EOD
    </body>
  </html>
EOD;
  }

  if (empty($print_mode)) {
    return $output;
  }
  echo $output;
}
