<?php
/**
 * @file
 * Defines admin functionality for the recipe entity.
 */

/**
 * The Recipe edit form.
 */
function drpg_recipe_form($form, &$form_state, $recipe, $op = 'edit', $entity_type = NULL) {
  module_load_include('inc', 'drpg_recipe', 'includes/drpg_recipe_item.admin');

  $form['#attached']['css'] = array(drupal_get_path('module', 'drpg') . '/includes/css/drpg.admin.css');

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($recipe->label) ? $recipe->label : '',
  );

  // Items.
  if ($recipe->recipe_id != 0) {

    $form['items'] = array(
      '#title' => 'Requires Item(s)',
      '#type' => 'fieldset',
      '#attributes' => array(
        'id' => array('items'),
      ),
      '#tree' => TRUE,
    );

    $recipe_items = drpg_recipe_get_recipe_items($recipe->recipe_id);

    if (isset($form_state['values']['items'])) {

      $item_count = 0;

      if (isset($form_state['clicked_button']) && ($form_state['clicked_button']['#value'] == 'Delete item')) {
        $parent_element = $form_state['clicked_button']['#parents'][1];

        $recipe_item = drpg_recipe_get_recipe_item($form_state['values']['items'][$parent_element][0]['recipe_item_id']);
        if ($recipe_item) {
          $recipe_item->delete();
        }

        unset($form_state['values']['items'][$parent_element]);
      }

      foreach ($form_state['values']['items'] as $item_element_id => $item_form_state) {
        if (is_array($item_form_state)) {
          $form['items'][$item_element_id] = array(
            '#type' => 'container',
            '#attributes' => array(
              'class' => array('item-fields'),
            ),
          );

          $form['items'][$item_element_id][] = drpg_recipe_item_embedded_form(array(), $item_form_state, NULL, 'edit', $item_count);
          $item_count++;
        }
      }

      // Handle addition of new recipe item.
      if (isset($form_state['clicked_button']) && ($form_state['clicked_button']['#value'] == 'Add item')) {
        $new_recipe_item = new DrpgRecipeItem();
        $new_recipe_item->recipe_id = $recipe->recipe_id;
        //$new_recipe_item->item_id = $item->item_id;

        $form['items']['item-' . $item_count] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('item-fields'),
          ),
        );

        $item_form_state = array();
        $form['items']['item-' . $item_count][] = drpg_recipe_item_embedded_form(array(), $item_form_state, $new_recipe_item, 'edit', $item_count);
      }

    }
    else {

      $item_form = $item_form_state = array();

      $item_count = 0;
      foreach ($recipe_items as $recipe_item) {
        $form['items']['item-' . $item_count] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('item-fields'),
          ),
        );

        $form['items']['item-' . $item_count][] = drpg_recipe_item_embedded_form($item_form, $item_form_state, $recipe_item, 'edit', $item_count);

        $item_count++;
      }

    }

    $form['items']['add'] = array(
      '#type' => 'button',
      '#value' => t('Add item'),
      '#weight' => 1,
      '#ajax' => array(
        'callback' => 'drpg_recipe_form_items_callback',
        'wrapper' => 'items',
        'method' => 'replace',
      ),
    );

  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  field_attach_form('drpg_recipe', $recipe, $form, $form_state);

  return $form;
}

/**
 * AJAX callback for form items element.
 */
function drpg_recipe_form_items_callback(&$form, &$form_state) {
  return $form['items'];
}

/**
 * Form API submit callback for the Item form.
 */
function drpg_recipe_form_submit(&$form, &$form_state) {
  $recipe = entity_ui_form_submit_build_entity($form, $form_state);

  $recipe->save();

  // Save Recipe Items.
  if (isset($form_state['values']['items'])) {
    foreach ($form_state['values']['items'] as $item_data) {
      if (is_array($item_data)) {
        $item_data = $item_data[0];

        if ($item_data['recipe_item_id'] != 0) {
          $recipe_item = drpg_attribute_get_attribute($item_data['recipe_item_id']);

          $recipe_item->item_id = $item_data['item_id'];
          $recipe_item->quantity = $item_data['quantity'];

          $recipe_item->save();
        }
        else {
          $recipe_item = new DrpgRecipeItem();
          $recipe_item->recipe_id = $recipe->recipe_id;
          $recipe_item->item_id = $item_data['item_id'];
          $recipe_item->quantity = $item_data['quantity'];

          $recipe_item->save();
        }
      }
    }
  }

  $form_state['redirect'] = 'admin/drpg/recipes';
}
