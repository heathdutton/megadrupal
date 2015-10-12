<?php
/**
 * @file
 * CTools content for the single ticket selection checkout pane.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Booking accommodation'),
  'content_types' => 'commerce_booking_accom_summary',
  // 'single' means not to be subtyped.
  'single' => TRUE,
  // Name of a function which will render the block.
  'render callback' => 'commerce_booking_accom_summary_render',

  // Icon goes in the directory with the content type.
  'description' => t('Show a list of requested accommodation for a booking.'),
  'required context' => new ctools_context_required(t('Booking'), 'entity:commerce_order'),
  'edit form' => 'commerce_booking_accom_summary_edit_form',
  'admin title' => 'commerce_booking_accom_summary_admin_title',

  // presents a block which is used in the preview of the data.
  // Pn Panels this is the preview pane shown on the panels building page.
  'category' => array(t('Commerce Booking'), 0),
);

/**
 * Render the Party Attached Entity Edit Form
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time
 * @param $args
 * @param $context
 *   Context - in this case we don't have any
 *
 * @return
 *   An object with at least title and content members
 */
function commerce_booking_accom_summary_render($subtype, $conf, $args, $context) {
  $booking_wrapper = entity_metadata_wrapper('commerce_order', $context->data);

  // Check if accommodation is enabled.
  $booking_field = commerce_booking_get_field_name($booking_wrapper->commerce_booking_event->type(), $booking_wrapper->commerce_booking_event->value());
  $booking = $booking_wrapper->commerce_booking_event->{$booking_field}->value();
  if (empty($booking['settings']['accommodation'])) {
    return FALSE;
  }

  // Build our block.
  $block = new stdClass();
  $block->title = t('Accommodation');
  $block->content = array();

  $item_list = array(
    '#theme' => 'item_list',
    '#items' => array(),
    '#type' => 'ul',
    '#attributes' => array(),
  );

  $block->content['types'] = array('#title' => t('Accommodation requirements:')) + $item_list;

  // Build our list.
  $accommodations = array();
  foreach($booking_wrapper->commerce_line_items as $line_item_wrapper) {
    if ($line_item_wrapper->getBundle() != 'commerce_booking_accommodation') {
      continue;
    }

    $type_id = $line_item_wrapper->cba_accommodation_type->getIdentifier();
    if (empty($accommodations[$type_id])) {
      $accommodations[$type_id] = array(
        'accom_type' => $line_item_wrapper->cba_accommodation_type,
        'quantity' => 1,
      );
    }
    else {
      $accommodations[$type_id]['quantity']++;
    }
  }

  foreach ($accommodations as $type_id => $accommodation) {
    $item = array(
      'data' => array(
        'quantity' => array(
          '#prefix' => '<span class="right">',
          '#markup' => 'x' . $accommodation['quantity'],
          '#suffix' => '</span>',
        ),
        'name' => array(
          '#prefix' => '<span>',
          '#markup' => $accommodation['accom_type']->label->value(),
          '#suffix' => '</span>',
        ),
      ),
      'class' => array('accom-type', 'accom-type-' . $accommodation['accom_type']->getIdentifier()),
    );

    // Render the data and add the item.
    $item['data'] = drupal_render($item['data']);
    $block->content['types']['#items'][] = $item;
  }

  // Special requirements
  if (!empty($conf['show_special_requirements'])) {
    $field_name = 'partyopoly_camping_requirements';
    $field = field_info_field($field_name);
    $instance = field_info_instance('commerce_order', $field_name, 'commerce_booking');
    $items = field_get_items('commerce_order', $booking_wrapper->value(), $field_name);

    if ($items) {
      $display = $instance['display']['default'];
      $display['label'] = 'hidden';

      $block->content['special_requirements'] = array('#title' => t('Special Requirements')) + $item_list;

      $output = render(field_default_view('commerce_order', $booking_wrapper->value(), $field, $instance, LANGUAGE_NONE, $items, $display));
      $block->content['special_requirements']['#items'][] = empty($output) ? t('None') : $output;
    }
  }

  return $block;
}

/**
 * Config Form
 */
function commerce_booking_accom_summary_edit_form($form, &$form_state) {
  $form['show_special_requirements'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Special Requirements'),
    '#default_value' => $form_state['conf']['show_special_requirements'],
  );

  return $form;
}

function commerce_booking_accom_summary_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Title Callback
 */
function commerce_booking_accom_summary_admin_title($subtype, $conf, $context = NULL) {
  return t('Booking accommodation');
}
