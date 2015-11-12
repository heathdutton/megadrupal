<?php
/**
 * @file
 * Defines admin functionality for the recipe item entity.
 */

/**
 * The recipe item edit form for use inside other forms.
 */
function drpg_recipe_item_embedded_form($form, &$form_state, $recipe_item, $op = 'edit', $item_count = 0) {

  if ($recipe_item != NULL) {
    $recipe_item_id = $recipe_item->recipe_item_id;
    $item_id = $recipe_item->item_id;
    $quanity = $recipe_item->quantity;
  }
  else {
    $recipe_item_id = isset($form_state['recipe_item_id']) ? $form_state['recipe_item_id'] : '';
    $item_id = isset($form_state['item_id']) ? $form_state['item_id'] : '';
    $quanity = isset($form_state['quantity']) ? $form_state['quantity'] : '';
  }

  $form['recipe_item_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $recipe_item_id,
  );

  $form['item_id'] = array(
    '#title' => t('Item'),
    '#type' => 'select',
    '#options' => drpg_item_get_item_names(),
    '#default_value' => $item_id,
  );

  $form['quantity'] = array(
    '#title' => t('Quantity'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => ($quanity > 0) ? $quanity : 1,
  );

  $form['delete'] = array(
    '#type' => 'button',
    '#value' => t('Delete item'),
    '#name' => 'delete-item-' . $item_count,
  );

  return $form;
}
