<?php
/**
 * @file
 * Defines admin functionality for the inventory item entity.
 */

/**
 * The inventory item edit form for use inside other forms.
 */
function drpg_inventory_embedded_form($form, &$form_state, $inventory_item, $op = 'edit', $entity_type = NULL, $item_count = 0) {

  if ($inventory_item != NULL) {
    $inventory_item_id = $inventory_item->inventory_item_id;
    $entity_id = $inventory_item->entity_id;
  }
  else {
    $inventory_item_id = isset($form_state['inventory_item_id']) ? $form_state['inventory_item_id'] : '';
    $entity_id = isset($form_state['entity_id']) ? $form_state['entity_id'] : '';
  }

  $form['inventory_item_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $inventory_item_id,
  );

  $form['entity_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $entity_id,
  );

  $form['delete'] = array(
    '#type' => 'button',
    '#value' => t('Delete item'),
    '#name' => 'delete-item-' . $item_count,
  );

  return $form;
}
