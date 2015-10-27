<?php

/**
 * @file
 * 2gis Maps module field definition file.
 */

/**
 * Implements hook_field_info().
 */
function dgis_maps_field_info() {
  return array(
    'dgis_maps' => array(
      'label' => t('2GIS maps'),
      'description' => t('Demonstrates a 2GIS maps.'),
      'default_widget' => 'dgis_maps_widget',
      'default_formatter' => 'dgis_maps_simple_view',
    ),
  );
}

/**
 * Implements hook_field_validate().
 * @see dgis_maps_field_widget_error()
 */
function dgis_maps_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (!empty($item['settings'])) {
      list($lat, $lng) = explode(',', $item['settings']['center']);
      if (!preg_match('@^[0-9]{2}.[0-9]@', $lat) && !preg_match('@^[0-9]{2}.[0-9]@', $lng)) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'dgis_maps_invalid',
          'message' => t('Invalid coordinates. Please retry.'),
        );
      }
      if (!is_numeric($item['settings']['zoom'])) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'dgis_maps_invalid',
          'message' => t('Invalid zoom. Please retry.'),
        );
      }
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function dgis_maps_field_is_empty($item, $field) {
  return empty($item['settings']);
}

/**
 * Implements hook_field_presave().
 */
function dgis_maps_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($field['type'] == 'dgis_maps') {
    foreach ($items as $delta => $item) {
      if (isset($item['settings'])) {
        $items[$delta]['settings'] = json_encode($item['settings']);
      }
    }
  }
}

/**
 * Implements hook_field_formatter_info().
 * @see dgis_maps_field_formatter_view()
 */
function dgis_maps_field_formatter_info() {
  return array(
    'dgis_maps_simple_view' => array(
      'label' => t('Simple 2GIS maps formatter'),
      'field types' => array('dgis_maps'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 * @see dgis_maps_field_formatter_info()
 */
function dgis_maps_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'dgis_maps_simple_view':
      if (!empty($items)) {
        drupal_add_library('dgis_maps', '2gis_maps');
        foreach ($items as $delta => $item) {
          // We don't use drupal_html_id, to store true delta.
          $map_id = "map-$delta";

          $map_data = array(
            '#type' => 'html_tag',
            '#tag' => 'span',
            '#attributes' => array(
              'id' => "$map_id-data",
              'class' => array('element-hidden'),
            ),
            '#value' => $item['settings'],
          );

          $field_output['map'] = array(
            '#type' => 'html_tag',
            '#tag' => 'div',
            '#attributes' => array(
              'id' => $map_id,
              'class' => array('dgis-maps-container-view'),
              'style' => 'width:' . 500 . 'px; height:' . 400 . 'px;',
            ),
            '#value' => '',
            '#suffix' => render($map_data),
          );

          $element[$delta] = $field_output;
        }
      }
      break;
    }

  return $element;
}

/**
 * Implements hook_field_widget_info().
 * @see dgis_maps_field_widget_form()
 */
function dgis_maps_field_widget_info() {
  return array(
    'dgis_maps_widget' => array(
      'label' => t('2GIS maps'),
      'field types' => array('dgis_maps'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function dgis_maps_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $field_state = field_form_get_state($element['#field_parents'], $field['field_name'], $langcode, $form_state);
  if (isset($field_state['items'])) {
    $items = $field_state['items'];
  }

  $removed = FALSE;
  // Prepare remove button for elements with unlimited cardinality.
  if ($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED) {
    // Get parent which will we use into Remove Button Element.
    $parents = array_merge($element['#field_parents'], array(
      $field['field_name'],
      $langcode,
      $delta
    ));
    $remove_button_name = implode('_', $parents) . '_remove_button';
    $remove_button = array(
      '#delta' => $delta,
      '#name' => $remove_button_name,
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => array('dgis_maps_widget_remove_button_submit_handler'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'path' => 'dgis-maps/ajax',
        'effect' => 'fade',
      ),
      '#weight' => 10,
    );
    $removed = !empty($form_state['triggering_element']['#name']) && $form_state['triggering_element']['#name'] == $remove_button_name;
  }


  // We can take values if item wasn't removed.
  if (!$removed) {
    // Saved settings.
    if (isset($items[$delta]['settings'])) {
      $settings = json_decode($items[$delta]['settings']);
    }
    // Settings from $_POST.
    else if (!empty($form_state['values'][$field['field_name']])) {
      // If we have data in $_POST for example when we want to add one more item.
      // Then we should take these settings and put it into Drupal.settings to re init map with these values on JS side.
      $settings = $form_state['values'][$field['field_name']][$langcode][$delta]['settings'];
    }
  }

  // Default settings.
  if (empty($settings)) {
    $settings = json_decode(dgis_maps_get_default_map_data());
  }

  $widget = $element;
  $widget['#delta'] = $delta;

  switch ($instance['widget']['type']) {
    case 'dgis_maps_widget':
      $widget['center'] = array(
        '#attributes' => array(
          'class' => array('edit-dgis-map-center')
        ),
        '#title' => t('Center'),
        '#type' => 'textfield',
        '#default_value' => $settings->center,
      );

      $widget['zoom'] = array(
        '#attributes' => array(
          'class' => array('edit-dgis-map-zoom')
        ),
        '#title' => t('Zoom'),
        '#type' => 'textfield',
        '#default_value' => $settings->zoom,
      );

      // Add remove button for elements with unlimited cardinality.
      if ($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED) {
        $widget['remove_button'] = $remove_button;
      }

      $widget['markers'] = array(
        '#type' => 'hidden',
        '#attributes' => array(
          'class' => array('edit-dgis-map-markers')
        ),
        '#default_value' => $settings->markers,
      );

      // Load library.
      $widget['#attached']['library'][] = array(
        'dgis_maps',
        '2gis_maps_block_admin'
      );

      // We don't use drupal_html_id, to store true delta.
      $map_id = "map-$delta";
      $widget['map'] = array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#value' => ' ',
        '#attributes' => array(
          'id' => $map_id,
          'class' => array('dgis-maps-container-edit'),
          'style' => 'width:' . 500 . 'px; height:' . 400 . 'px; clear: both;',
        ),
        '#weight' => -20,
      );

      if ($instance['required'] == 1) {
        $widget['#required'] = 1;
      }
      break;
  }

  $element['settings'] = $widget;

  return $element;
}

/**
 * Implements hook_field_widget_error().
 * @see dgis_maps_field_validate()
 * @see form_error()
 */
function dgis_maps_field_widget_error($element, $error, $form, &$form_state) {
  switch ($error['error']) {
    case 'dgis_maps_invalid':
      form_error($element, $error['message']);
      break;
  }
}

/**
 * Ajax callback remove field when remove click is trigger.
 */
function dgis_maps_field_remove_item() {
  // We are don't update html and we don't need have ajax_html_ids.
  // Otherwise remove buttons wouldn't be bound after AJAX.
  if (isset($_POST['ajax_html_ids'])) {
    unset($_POST['ajax_html_ids']);
  }

  list($form, $form_state) = ajax_get_form();
  drupal_process_form($form['#form_id'], $form, $form_state);
  // Get the information on what we're removing.
  $button = $form_state['triggering_element'];
  // Go three levels up in the form, to the whole widget.
  $element = drupal_array_get_nested_value($form, array_slice($button['#array_parents'], 0, -4));
  // Now send back the proper AJAX command to replace it.
  $return = array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace('#' . $element['#id'], render($element)),
    ),
  );

  // Because we're doing this ourselves, messages aren't automatic. We have
  // to add them.
  $messages = theme('status_messages');
  if ($messages) {
    $return['#commands'][] = ajax_command_prepend('#' . $element['#id'], $messages);
  }

  return $return;
}

/**
 * Submit callback to remove an item from the field UI multiple wrapper.
 *
 * When a remove button is submitted, we need to find the item that it
 * referenced and delete it. Since field UI has the deltas as a straight
 * unbroken array key, we have to renumber everything down. Since we do this
 * we *also* need to move all the deltas around in the $form_state['values']
 * and $form_state['input'] so that user changed values follow. This is a bit
 * of a complicated process.
 */
function dgis_maps_widget_remove_button_submit_handler($form, &$form_state) {
  $button = $form_state['triggering_element'];
  $delta = $button['#delta'];
  // Where in the form we'll find the parent element.
  $address = array_slice($button['#array_parents'], 0, -3);

  // Go one level up in the form, to the widgets container.
  $parent_element = drupal_array_get_nested_value($form, $address);
  $field_name = $parent_element['#field_name'];
  $langcode = $parent_element['#language'];
  $parents = $parent_element['#field_parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);

  // Go ahead and renumber everything from our delta to the last
  // item down one. This will overwrite the item being removed.
  for ($i = $delta; $i <= $field_state['items_count']; $i++) {
    $old_element_address = array_merge($address, array($i + 1));
    $new_element_address = array_merge($address, array($i));

    $moving_element = drupal_array_get_nested_value($form, $old_element_address);
    $moving_element_value = drupal_array_get_nested_value($form_state['values'], $old_element_address);
    $moving_element_input = drupal_array_get_nested_value($form_state['input'], $old_element_address);

    // Tell the element where it's being moved to.
    $moving_element['#parents'] = $new_element_address;

    // Move the element around.
    form_set_value($moving_element, $moving_element_value, $form_state);
    drupal_array_set_nested_value($form_state['input'], $moving_element['#parents'], $moving_element_input);
  }

  // Replace the deleted entity with an empty one. This helps to ensure that
  // trying to add a new entity won't ressurect a deleted entity
  // from the trash bin.
  // $count = count($field_state['entity']);
  // Then remove the last item. But we must not go negative.
  if ($field_state['items_count'] > 0) {
    $field_state['items_count']--;
  }

  // Fix the weights. Field UI lets the weights be in a range of
  // (-1 * item_count) to (item_count). This means that when we remove one,
  // the range shrinks; weights outside of that range then get set to
  // the first item in the select by the browser, floating them to the top.
  // We use a brute force method because we lost weights on both ends
  // and if the user has moved things around, we have to cascade because
  // if I have items weight weights 3 and 4, and I change 4 to 3 but leave
  // the 3, the order of the two 3s now is undefined and may not match what
  // the user had selected.
  $input = drupal_array_get_nested_value($form_state['input'], $address);
  // Clear NULL values.
  $input = array_filter($input);
  // Sort by weight.
  uasort($input, '_field_sort_items_helper');

  // Reweight everything in the correct order.
  $weight = -1 * $field_state['items_count'];
  foreach ($input as $key => $item) {
    if ($item) {
      $input[$key]['_weight'] = $weight++;
    }
  }
  drupal_array_set_nested_value($form_state['input'], $address, $input);
  field_form_set_state($parents, $field_name, $langcode, $form_state, $field_state);

  $form_state['rebuild'] = TRUE;
}
