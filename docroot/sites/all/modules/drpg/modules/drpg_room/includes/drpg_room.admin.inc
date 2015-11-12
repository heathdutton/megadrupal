<?php
/**
 * @file
 * Defines admin functionality for the room entity.
 */

/**
 * The Room edit form.
 */
function drpg_room_form($form, &$form_state, $room, $op = 'edit', $entity_type = NULL) {

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#weight' => -2,
    '#default_value' => isset($room->label) ? $room->label : '',
  );

  $form['item_container_quantity'] = array(
    '#title' => t('Number of item containers'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $room->item_container_quantity,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  field_attach_form('drpg_room', $room, $form, $form_state);

  $form['drpg_multi_npc_ref']['#weight'] = -1;

  return $form;
}

/**
 * Form API submit callback for the Room form.
 */
function drpg_room_form_submit(&$form, &$form_state) {
  $room = entity_ui_form_submit_build_entity($form, $form_state);

  $room->save();
  $form_state['redirect'] = 'admin/drpg/rooms';
}
