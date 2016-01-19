<?php

/**
 * @file
 * Rules integration for the ERPAL invoice module.
 */

/**
 * Implements hook_rules_action_info().
 */
function erpal_invoice_rules_action_info() {
  return array(
    'erpal_invoice_set_due_date' => array(
      'label' => t('Set due date for invoice'),
      'parameter' => array(
        'order' => array('type' => 'commerce_order', 'label' => t('Order')),
      ),
      'group' => t('ERPAL invoice'),
    ),
    'erpal_invoice_replicate_order' => array(
      'label' => t('Replicate a Commerce Order'),
      'group' => t('Replicate'),
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Order to replicate'),
        ),
      ),
      'provides' => array(
        'new_commerce_order' => array(
          'type' => 'integer',
          'label' => t('New order'),
        ),
      ),
    ),
  );
}

/**
 * Set due date for invoice.
 *
 * Used as Rules action callback.
 *
 * @see erpal_invoice_rules_action_info()
 */
function erpal_invoice_set_due_date($order) {
  $due_date = 0;
  $invoice_date = field_get_items('commerce_order', $order, 'field_commerce_billy_i_date');
  $credit_period = field_get_items('commerce_order', $order, 'field_invoice_credit_period');
  if ($invoice_date && $credit_period) {
    $due_time = $invoice_date[0]['value'] + (60 * 60 * 24 * $credit_period[0]['value']);
    $lang = field_language('commerce_order', $order, 'field_invoice_due_date');
    $order->field_invoice_due_date[$lang][0]['value'] = $due_time;
  }
  commerce_order_save($order);
}

/**
 * The action function for the 'erpal_invoice_replicate_order'.
 */
function erpal_invoice_replicate_order($commerce_order) {
  $new = replicate_entity('commerce_order', $commerce_order);
  if ($new) {
    $order = commerce_order_load($new);

    // Ensure the attached line items are associated with the order
    // if they do not have an order_id set yet.
    foreach (entity_metadata_wrapper('commerce_order', $order)->commerce_line_items as $line_item_wrapper) {
      if ($line_item_wrapper->order_id->value() != $order->order_id) {
        $line_item_wrapper->order_id = $order->order_id;
        $line_item_wrapper->save();
      }
    }
  }
  return array('new_commerce_order' => $new);
}

/**
 * Alter the replica of type commerce_order.
 *
 * Use this function to make specific changes to an entity of a given type.
 * Usefull to "clean" custom entities, ie reset their id to be able to save
 * the new copy for example.
 *
 * @param object $replica
 *   Reference to the fully loaded entity object being saved (the clone) that
 *   can be altered as needed.
 *
 * @see replicate_clone_entity()
 */
function erpal_invoice_replicate_entity_commerce_order(&$replica) {
  global $user;
  $id = $replica->order_id;
  $replica->order_id = NULL;
  $replica->status = 'pending';
  $replica->uid = $user->uid;
  $commerce_line_items = field_get_items('commerce_order', $replica, 'commerce_line_items');
  $language = field_language('commerce_order', $replica, 'commerce_line_items');
  unset($replica->order_number,
        $replica->revision_id,
        $replica->revision_uid,
        $replica->created,
        $replica->changed,
        $replica->hostname,
        $replica->commerce_line_items);

  // Replicate commerce lite items.
  if ($commerce_line_items) {
    foreach ($commerce_line_items as $value) {
      $line_item = commerce_line_item_load($value['line_item_id']);
      $type = 'product';
      $line_item_new = commerce_line_item_new($type);

      $line_item_new =  clone $line_item;
      unset($line_item_new->line_item_id, $line_item_new->order_id);
      $line_item_new->type = $type;
      commerce_line_item_save($line_item_new);
      $replica->commerce_line_items[$language][] = array(
        'line_item_id' => $line_item_new->line_item_id,
      );
    }
  }

  if ($replica->type == 'commerce_order') {
    $replica->type = 'invoice';
    unset($replica->field_order_date,
          $replica->field_order_text,
          $replica->field_order);
    $replica->field_invoice_order[$language][] = array(
      'target_id' => $id,
    );
  }

}