<?php
/**
 * @file
 * Defines admin functionality for the item container entity.
 */

/**
 * The Item Container edit form.
 */
function drpg_item_container_form($form, &$form_state, $item_container, $op = 'edit', $entity_type = NULL) {

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($item_container->label) ? $item_container->label : '',
  );

  $form['item_quantity'] = array(
    '#title' => t('Item Quantity'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => isset($item_container->item_quantity) ? $item_container->item_quantity : '',
  );

  $form['rarity'] = array(
    '#type' => 'fieldset',
    '#title' => 'Item Rarity',
  );

  $form['rarity']['item_rarity_operator'] = array(
    '#type' => 'select',
    '#options' => drpg_item_rarity_operators(),
    '#default_value' => isset($item_container->item_rarity_operator) ? $item_container->item_rarity_operator : '',
  );

  $form['rarity']['item_rarity'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => isset($item_container->item_rarity) ? $item_container->item_rarity : '',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form API submit callback for the Item form.
 */
function drpg_item_container_form_submit(&$form, &$form_state) {
  $item_container = entity_ui_form_submit_build_entity($form, $form_state);

  $item_container->save();
  $form_state['redirect'] = 'admin/drpg/item_containers';
}
