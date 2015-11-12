<?php

/**
 * @file
 * Plugin to provide an relationship handler for ticket from event.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Ticket from Booking'),
  'keyword' => 'ticket_from_booking',
  'description' => t('Adds a the Ticket from a Booking.'),
  'required context' => array(
    new ctools_context_required(t('Booking'), 'commerce_order'),
    new ctools_context_required(t('User'), 'user'),
  ),
  'context' => 'party_commerce_booking_commerce_booking_ticket_from_commerce_order_context',
  'edit form' => 'party_commerce_booking_commerce_booking_ticket_from_commerce_order_settings_form',
  'defaults' => array(),
);

/**
 * Return a new context based on an existing context.
 */
function party_commerce_booking_commerce_booking_ticket_from_commerce_order_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context[0]->data) || empty($context[1]->data)) {
    return ctools_context_create_empty('entity:commerce_booking_ticket', NULL);
  }

  $party = party_user_get_party($context[1]->data);
  $order = $context[0]->data;

  // Find this party's ticket for the order.
  $query = db_select('commerce_order', 'o');
  $query->join('field_data_commerce_line_items', 'o_li',
    'o_li.entity_type = :o_li_entity_type AND o_li.entity_id = o.order_id',
    array(':o_li_entity_type' => 'commerce_order'));
  $query->join('field_data_commerce_booking_ticket', 'li_t',
    'li_t.entity_type = :li_t_entity_type AND li_t.entity_id = o_li.commerce_line_items_line_item_id',
    array(':li_t_entity_type' => 'commerce_line_item'));
  $query->join('commerce_booking_ticket', 't',
    't.ticket_id = li_t.commerce_booking_ticket_target_id');
  $query->join('field_data_party', 't_p',
    't_p.entity_type = :t_p_entity_type AND t_p.entity_id = t.ticket_id',
    array(':t_p_entity_type' => 'commerce_booking_ticket'));
  $query->condition('o.order_id', $order->order_id);
  $query->condition('t_p.party_target_id', $party->pid);
  $query->addField('t', 'ticket_id');
  $ticket_id = $query->execute()->fetchField();

  if ($ticket_id) {
    $ticket = entity_load_single('commerce_booking_ticket', $ticket_id);
    return ctools_context_create('entity:commerce_booking_ticket', $ticket);
  }
  else {
    return FALSE;
  }
}

/**
 * Settings form for the relationship.
 */
function party_commerce_booking_commerce_booking_ticket_from_commerce_order_settings_form($form, &$form_state) {
  return $form;
}
