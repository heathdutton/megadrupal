<?php
/**
 * @file
 * Defines admin functionality for the item entity.
 */

/**
 * The Item edit form.
 */
function drpg_item_form($form, &$form_state, $item, $op = 'edit', $entity_type = NULL) {
  module_load_include('inc', 'drpg_attribute', 'includes/drpg_attribute.admin');

  $form['#attached']['css'] = array(drupal_get_path('module', 'drpg') . '/includes/css/drpg.admin.css');

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($item->label) ? $item->label : '',
  );

  $form['rarity'] = array(
    '#title' => t('Rarity'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $item->rarity,
  );

  $form['equippable'] = array(
    '#title' => t('Equippable'),
    '#type' => 'checkbox',
    '#default_value' => $item->equippable,
  );

  $form['currency_cost'] = array(
    '#title' => t('Currency Cost'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $item->currency_cost,
  );

  $form['currency_value'] = array(
    '#title' => t('Currency Value'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $item->currency_value,
  );

  $form['ui_asset_path'] = array(
    '#title' => t('UI Asset Path'),
    '#type' => 'textfield',
    '#default_value' => isset($item->ui_asset_path) ? $item->ui_asset_path : '',
    '#description' => 'The path to this item\'s UI asset, relative to your game\'s assets path.',
  );

  $form['game_asset_path'] = array(
    '#title' => t('In-Game Asset Path'),
    '#type' => 'textfield',
    '#default_value' => isset($item->game_asset_path) ? $item->game_asset_path : '',
    '#description' => 'The path to this item\'s in-game asset, relative to your game\'s assets path.',
  );

  // Item attributes.
  if ($item->item_id != 0) {

    $form['attributes'] = array(
      '#title' => 'Attributes',
      '#type' => 'fieldset',
      '#attributes' => array(
        'id' => array('attributes'),
      ),
      '#tree' => TRUE,
    );

    $attributes = drpg_attribute_get_entity_attributes('drpg_item', $item->item_id);

    if (isset($form_state['values']['attributes'])) {

      $attribute_count = 0;

      if (isset($form_state['clicked_button']) && ($form_state['clicked_button']['#value'] == 'Delete attribute')) {
        $parent_element = $form_state['clicked_button']['#parents'][1];

        $attribute = drpg_attribute_get_attribute($form_state['values']['attributes'][$parent_element][0]['attribute_id']);
        if ($attribute) {
          $attribute->delete();
        }

        unset($form_state['values']['attributes'][$parent_element]);
      }

      foreach ($form_state['values']['attributes'] as $attribute_element_id => $attribute_form_state) {
        if (is_array($attribute_form_state)) {
          $form['attributes'][$attribute_element_id] = array(
            '#type' => 'container',
            '#attributes' => array(
              'class' => array('attribute-fields'),
            ),
          );

          $form['attributes'][$attribute_element_id][] = drpg_attribute_embedded_form(array(), $attribute_form_state, NULL, 'edit', NULL, $attribute_count);
          $attribute_count++;
        }
      }

      // Handle addition of new attribute.
      if (isset($form_state['clicked_button']) && ($form_state['clicked_button']['#value'] == 'Add attribute')) {
        $new_attribute = new DrpgAttribute();
        $new_attribute->type = 'drpg_item';
        $new_attribute->entity_id = $item->item_id;

        $form['attributes']['attribute-' . $attribute_count] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('attribute-fields'),
          ),
        );

        $attribute_form_state = array();
        $form['attributes']['attribute-' . $attribute_count][] = drpg_attribute_embedded_form(array(), $attribute_form_state, $new_attribute, 'edit', NULL, $attribute_count);
      }

    }
    else {

      $attribute_form = $attribute_form_state = array();

      $attribute_count = 0;
      foreach ($attributes as $attribute) {
        $form['attributes']['attribute-' . $attribute_count] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('attribute-fields'),
          ),
        );

        $form['attributes']['attribute-' . $attribute_count][] = drpg_attribute_embedded_form($attribute_form, $attribute_form_state, $attribute, 'edit', NULL, $attribute_count);

        $attribute_count++;
      }

    }

    $form['attributes']['add'] = array(
      '#type' => 'button',
      '#value' => t('Add attribute'),
      '#weight' => 1,
      '#ajax' => array(
        'callback' => 'drpg_item_form_attributes_callback',
        'wrapper' => 'attributes',
        'method' => 'replace',
      ),
    );

  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * AJAX callback for form attributes element.
 */
function drpg_item_form_attributes_callback(&$form, &$form_state) {
  return $form['attributes'];
}

/**
 * Form API submit callback for the Item form.
 */
function drpg_item_form_submit(&$form, &$form_state) {
  $item = entity_ui_form_submit_build_entity($form, $form_state);

  $item->save();

  // Save Item Attributes.
  if (isset($form_state['values']['attributes'])) {
    foreach ($form_state['values']['attributes'] as $attribute_data) {
      if (is_array($attribute_data)) {
        $attribute_data = $attribute_data[0];

        if ($attribute_data['attribute_id'] != 0) {
          $attribute = drpg_attribute_get_attribute($attribute_data['attribute_id']);

          $attribute->attribute_name = $attribute_data['attribute_name'];
          $attribute->value = $attribute_data['value'];

          $attribute->save();
        }
        else {
          $attribute = new DrpgAttribute();
          $attribute->entity_id = $item->item_id;
          $attribute->type = 'drpg_item';
          $attribute->attribute_name = $attribute_data['attribute_name'];
          $attribute->value = $attribute_data['value'];

          $attribute->save();
        }
      }
    }
  }

  $form_state['redirect'] = 'admin/drpg/items';
}
