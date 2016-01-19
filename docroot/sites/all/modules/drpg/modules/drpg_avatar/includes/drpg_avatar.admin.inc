<?php
/**
 * @file
 * Defines admin functionality for the avatar entity.
 */

/**
 * The Avatar edit form.
 */
function drpg_avatar_form($form, &$form_state, $avatar, $op = 'edit', $entity_type = NULL) {
  module_load_include('inc', 'drpg_attribute', 'includes/drpg_attribute.admin');
  module_load_include('inc', 'drpg_inventory_item', 'includes/drpg_inventory_item.admin');

  $form['#attached']['css'] = array(drupal_get_path('module', 'drpg') . '/includes/css/drpg.admin.css');

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($avatar->label) ? $avatar->label : '',
  );

  $form['owner_uid'] = array(
    '#title' => t('Owner UID'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $avatar->owner_uid,
  );

  $form['currency'] = array(
    '#title' => t('Currency'),
    '#type' => 'textfield',
    '#size' => 10,
    '#description' => t('Generic currency; the game client decides what this is (gold, dollars, bottle caps, etc.)'),
    '#default_value' => $avatar->currency,
  );

  if ($avatar->avatar_id != 0) {

    // Avatar attributes.
    $form['attributes'] = array(
      '#title' => 'Attributes',
      '#type' => 'fieldset',
      '#attributes' => array(
        'id' => array('attributes'),
      ),
      '#tree' => TRUE,
    );

    $attributes = drpg_attribute_get_entity_attributes('drpg_avatar', $avatar->avatar_id);

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
        $new_attribute->type = 'drpg_avatar';
        $new_attribute->entity_id = $avatar->avatar_id;

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
        'callback' => 'drpg_avatar_form_attributes_callback',
        'wrapper' => 'attributes',
        'method' => 'replace',
      ),
    );

    // Avatar inventory.
    $form['inventory'] = array(
      '#title' => 'Inventory Items',
      '#type' => 'fieldset',
      '#attributes' => array(
        'id' => array('inventory'),
      ),
      '#tree' => TRUE,
    );

    $inventory = drpg_inventory_item_get_inventory('drpg_avatar', $avatar->avatar_id);

    if (isset($form_state['values']['inventory'])) {

      $item_count = 0;

      if (isset($form_state['clicked_button']) && ($form_state['clicked_button']['#value'] == 'Delete item')) {
        $parent_element = $form_state['clicked_button']['#parents'][1];

        $inventory_item = drpg_inventory_item_get_item($form_state['values']['inventory'][$parent_element][0]['inventory_item_id']);
        if ($inventory_item) {
          $inventory_item->delete();
        }

        unset($form_state['values']['inventory'][$parent_element]);
      }

      foreach ($form_state['values']['inventory'] as $item_element_id => $item_form_state) {
        if (is_array($item_form_state)) {
          $form['inventory'][$item_element_id] = array(
            '#type' => 'container',
            '#attributes' => array(
              'class' => array('item-fields'),
            ),
          );

          $form['inventory'][$item_element_id][] = drpg_inventory_embedded_form(array(), $item_form_state, NULL, 'edit', NULL, $item_count);
          field_attach_form('drpg_inventory_item', NULL, $form['inventory'][$item_element_id], $item_form_state);
          unset($form['inventory'][$item_element_id]['#parents']);
          $item_count++;
        }
      }

      // Handle addition of new item.
      if (isset($form_state['clicked_button']) && ($form_state['clicked_button']['#value'] == 'Add item')) {
        $new_item = new DrpgInventoryItem();
        $new_item->type = 'drpg_avatar';
        $new_item->entity_id = $avatar->avatar_id;

        $form['inventory']['item-' . $item_count] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('item-fields'),
          ),
        );

        $item_form_state = array();
        $form['inventory']['item-' . $item_count][] = drpg_inventory_embedded_form(array(), $item_form_state, $new_item, 'edit', NULL, $item_count);
        field_attach_form('drpg_inventory_item', $new_item, $form['inventory']['item-' . $item_count], $item_form_state);
        unset($form['inventory']['item-' . $item_count]['#parents']);
      }

    }
    else {

      $item_form = $item_form_state = array();

      $item_count = 0;
      foreach ($inventory['item_instances'] as $item) {
        $form['inventory']['item-' . $item_count] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('item-fields'),
          ),
        );

        $item = drpg_inventory_item_get_item($item->inventory_item_id);

        $form['inventory']['item-' . $item_count][] = drpg_inventory_embedded_form($item_form, $item_form_state, $item, 'edit', NULL, $item_count);
        field_attach_form('drpg_inventory_item', $item, $form['inventory']['item-' . $item_count], $item_form_state);
        unset($form['inventory']['item-' . $item_count]['#parents']);
        $item_count++;
      }

    }

    $form['inventory']['add'] = array(
      '#type' => 'button',
      '#value' => t('Add item'),
      '#weight' => 1,
      '#ajax' => array(
        'callback' => 'drpg_avatar_form_inventory_callback',
        'wrapper' => 'inventory',
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
function drpg_avatar_form_attributes_callback(&$form, &$form_state) {
  return $form['attributes'];
}

/**
 * AJAX callback for form inventory element.
 */
function drpg_avatar_form_inventory_callback(&$form, &$form_state) {
  return $form['inventory'];
}

/**
 * Form API submit callback for the Avatar form.
 */
function drpg_avatar_form_submit(&$form, &$form_state) {
  $avatar = entity_ui_form_submit_build_entity($form, $form_state);

  $avatar->save();

  // Save Avatar Attributes.
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
          $attribute->entity_id = $avatar->avatar_id;
          $attribute->type = 'drpg_avatar';
          $attribute->attribute_name = $attribute_data['attribute_name'];
          $attribute->value = $attribute_data['value'];

          $attribute->save();
        }
      }
    }
  }

  // Save Avatar Inventory.
  if (isset($form_state['values']['inventory'])) {
    foreach ($form_state['values']['inventory'] as $inventory_data) {
      if (is_array($inventory_data)) {
        $item_data = $inventory_data[0];
        $item_data['drpg_item_ref'] = $inventory_data['drpg_item_ref'][LANGUAGE_NONE][0];

        if ($item_data['drpg_item_ref']['target_id'] == NULL) {
          // No item selected.
          continue;
        }

        if ($item_data['inventory_item_id'] != 0) {
          $item = drpg_inventory_item_get_item($item_data['inventory_item_id']);
        }
        else {
          $item = new DrpgInventoryItem();
          $item->entity_id = $avatar->avatar_id;
          $item->type = 'drpg_avatar';
        }

        $item_wrapper = entity_metadata_wrapper('drpg_inventory_item', $item);
        $item_wrapper->drpg_item_ref->set($item_data['drpg_item_ref']['target_id']);

        $item_wrapper->save();
      }
    }
  }

  $form_state['redirect'] = 'admin/drpg/avatars';
}
