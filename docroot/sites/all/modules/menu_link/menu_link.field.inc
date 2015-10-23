<?php

/**
 * @file
 * Defines a menu link field type.
 */

/**
 * Implements hook_field_info().
 *
 * Field settings:
 *   - link_path_field: Boolean whether or not the link path field is available
 *   to users. If set to FALSE enity_uri() is used to determine the link path.
 * Instance settings:
 *   - menu_options: The menus available to place links in for this field.
 */
function menu_link_field_info() {
  return array(
    'menu_link' => array(
      'label' => t('Menu link'),
      'description' => t('This field stores a reference to a menu link in the database.'),
      'settings' => array(
        'link_path_field' => FALSE,
      ),
      'instance_settings' => array(
        'menu_options' => array('main-menu'),
      ),
      'default_widget' => 'menu_link_default',
      'default_formatter' => 'menu_link_link',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function menu_link_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];

  $form = array();

  /* @todo Allow menu link fields to be used as a reference field? http://drupal.org/node/1028344
  $form['link_path_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable <em>Path</em> field'),
    '#default_value' => $settings['link_path_field'],
    '#description' => t('Allow users to set the path of menu links. When not checked the uri of the entity being edited is used.'),
  );*/

  return $form;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function menu_link_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  $form['menu_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available menus'),
    '#default_value' => $settings['menu_options'],
    '#options' => menu_get_menus(),
    '#description' => t('The menus available to place links in for this field.'),
    '#required' => TRUE,
    '#weight' => -5,
  );

  return $form;
}

/**
 * Implements hook_field_load().
 */
function menu_link_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => &$item) {
      if (!is_array($item['options'])) {
        $item['options'] = !empty($item['options']) ? unserialize($item['options']) : array();
      }

      $item['external'] = (!empty($item['link_path']) && $item['link_path'] != '<front>' && url_is_external($item['link_path'])) ? 1 : 0;
    }
  }
}

/**
 * Implements hook_field_validate().
 *
 * Possible error codes:
 * - 'menu_link_duplicate': A menu link can only be referenced once per entity
 *   per field.
 * - 'menu_link_invalid_parent': Selected parent menu link does not exist or may
 *   not be selected.
 * - 'menu_link_no_entity_uri': No link path is provided while entity being
 *   edited has no URI of its own.
 */
function menu_link_field_validate($entity_type, $entity, $field, $instance, $langcode, &$items, &$errors) {
  if (!empty($entity)) {
    list($id, , ) = entity_extract_ids($entity_type, $entity);
  }

  // Build an array of existing menu link IDs so they can be loaded with
  // menu_link_load_multiple();
  $mlids = array();
  foreach ($items as $delta => &$item) {
    if (menu_link_field_is_empty($item, $field)) {
      continue;
    }
    if (!empty($item['mlid']) && (int)$item['mlid'] > 0) {
      $mlids[] = (int)$item['mlid'];
    }
    if (!empty($item['plid']) && (int)$item['plid'] > 0) {
      $mlids[] = (int)$item['plid'];
    }
  }
  $menu_links = !empty($mlids) ? menu_link_load_multiple($mlids) : array();

  $mlids = array();
  foreach ($items as $delta => &$item) {
    if (menu_link_field_is_empty($item, $field)) {
      continue;
    }

    if (empty($item['menu_name']) || !in_array($item['menu_name'], $instance['settings']['menu_options'])) {
      $errors[$field['field_name']][$langcode][$delta][] = array(
        'error' => 'menu_link_invalid_parent',
        'message' => t('%name: Invalid menu name.', array('%name' => t($instance['label']))),
      );
    }

    if (!empty($item['plid'])) {
      if (!isset($menu_links[$item['plid']]) || !in_array($menu_links[$item['plid']]['menu_name'], $instance['settings']['menu_options'])) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'menu_link_invalid_parent',
          'message' => t('%name: Invalid parent menu link.', array('%name' => t($instance['label']))),
        );
      }
    }

    if (!empty($item['mlid'])) {
      if (isset($mlids[$item['mlid']])) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'menu_link_duplicate',
          'message' => t('%name: A menu link can only be referenced once per entity per field.', array('%name' => t($instance['label']))),
        );
      }
      else {
        $mlids[$item['mlid']] = $item['mlid'];
      }
    }

    if (!empty($field['settings']['link_path_field']) && empty($item['link_path']) && !empty($id)) {
      $uri = entity_uri($entity_type, $entity);
      if ($uri === NULL) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'menu_link_no_entity_uri',
          'message' => t('%name: No link path is provided while entity being edited has no URI of its own.', array('%name' => t($instance['label']))),
        );
      }
    }
  }
}

/**
 * Implements hook_field_presave().
 *
 * @see menu_link_menu_link_update()
 */
function menu_link_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => &$item) {
    if (empty($item['plid'])) {
      $item['plid'] = 0;
    }

    $item += array(
      'options' => array(),
      'hidden' => 0,
      'expanded' => 0,
      'weight' => 0,
    );

    // Add a key to indicate we are saving menu links from within a field. This
    // key is not stored in the database and will only be available during the
    // current request.
    // @see menu_link_menu_link_update()
    $item['menu_link_field_save'] = $field['field_name'];

    // TODO This could override the module column!
    $item['module'] = 'menu_link';
  }
}

/**
 * Implements hook_field_insert().
 */
function menu_link_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  $path = _menu_link_path($entity_type, $entity, $langcode);

  foreach ($items as $delta => &$item) {
    if (empty($field['settings']['link_path_field']) || empty($item['link_path'])) {
      $item['link_path'] = $path;
    }

    // If the options array was serialized on a previous insert or update,
    // unserialize it back to an array for menu_link_save().
    if (isset($item['options']) && is_string($item['options'])) {
      $item['options'] = unserialize($item['options']);
    }
    if (!menu_link_save($item)) {
      drupal_set_message(t('There was an error saving the menu link.'), 'error');
    }

    $item['options'] = serialize($item['options']);
  }
}

/**
 * Implements hook_field_update().
 */
function menu_link_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Load the field items as they are stored in the database before update.
  $original = entity_create_stub_entity($entity_type, array($id, $vid, $bundle));
  field_attach_load($entity_type, array($id => $original), FIELD_LOAD_CURRENT, $options = array('field_id' => $field['id']));

  // Initially asume that all links are being deleted; later on in this function,
  // links that are kept are removed from this array.
  $delete_links = array();
  if (!empty($original->{$instance['field_name']}[$langcode])) {
    foreach($original->{$instance['field_name']}[$langcode] as $item) {
      $delete_links[$item['mlid']] = $item['mlid'];
    }
  }

  $path = _menu_link_path($entity_type, $entity, $langcode);

  foreach ($items as $delta => &$item) {
    if (empty($field['settings']['link_path_field']) || empty($item['link_path'])) {
      $item['link_path'] = $path;
    }

    // If the options array was serialized on a previous insert or update,
    // unserialize it back to an array for menu_link_save().
    if (isset($item['options']) && is_string($item['options'])) {
      $item['options'] = unserialize($item['options']);
    }
    
    // If field is not translatable, make menu item same language as entity.
    if (!$field['translatable']) {
      $item['language'] = $entity->language;
      $item['options']['langcode'] = $entity->language;
    }
    
    if (!menu_link_save($item)) {
      drupal_set_message(t('There was an error saving the menu link.'), 'error');
      // TODO what to do?
    }

    $item['options'] = serialize($item['options']);

    // Don't remove menu links that are being kept.
    unset($delete_links[$item['mlid']]);
  }

  // Delete any menu links that are no longer used.
  if (!empty($delete_links)) {
    menu_link_delete_multiple($delete_links);
  }
}

/**
 * Helper function to build a menu link path based on an entity.
 */
function _menu_link_path($entity_type, $entity, $langcode) {
  $uri = entity_uri($entity_type, $entity);
  if (url_is_external($uri['path'])) {
    $path = url($uri['path'], $uri['options']);
  }
  else {
    $path = drupal_get_normal_path($uri['path'], $langcode);
    if (!empty($uri['options']['query'])) {
      $path .= (strpos($path, '?') !== FALSE ? '&' : '?') . drupal_http_build_query($uri['options']['query']);
    }
    if (!empty($uri['options']['fragment'])) {
      $path .= '#' . $uri['options']['fragment'];
    }
  }
  return $path;
}

/**
 * Implements hook_field_delete().
 */
function menu_link_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  $mlids = array();
  foreach ($items as $delta => $item) {
    $mlids[] = $item['mlid'];
  }

  if (!empty($mlids)) {
    // Only delete menu links that are (still) owned by the Menu link module.
    $mlids = db_select('menu_links')
      ->fields('menu_links', array('mlid'))
      ->condition('module', 'menu_link')
      ->condition('mlid', $mlids)
      ->execute()
      ->fetchCol();
    if (!empty($mlids)) {
      menu_link_delete_multiple($mlids);
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function menu_link_field_is_empty($item, $field) {
  if (!empty($item['delete']) || !trim($item['link_title']) || empty($item['menu_name']) || (empty($item['plid']) && $item['plid'] != '0')) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_field_formatter_info().
 */
function menu_link_field_formatter_info() {
  return array(
    'menu_link_link' => array(
      'label' => t('Link'),
      'field types' => array('menu_link'),
    ),
  );
}

/**
 * Implements hook_field_formatter_prepare_view().
 */
function menu_link_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  $mlids = array();
  // Collect every possible menu link attached to any of the fieldable entities.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      // Prevent notices in previews of unsaved entities.
      if (!empty($id)) {
        // Prevent doing things twice.
        if (!empty($item['mlid']) && empty($item['title'])) {
          // Force the array key to prevent duplicates.
          $mlids[$item['mlid']] = $item['mlid'];
        }
      }
    }
  }

  if (!empty($mlids)) {
    $menu_links = menu_link_load_multiple($mlids);

    // Iterate through the fieldable entities again to attach the loaded menu
    // link data.
    foreach ($entities as $id => $entity) {
      $rekey = FALSE;

      foreach ($items[$id] as $delta => $item) {
        // Prevent notices in previews.
        if (!empty($id) && !empty($item['mlid'])) {
          // Check whether the menu link field instance value could be loaded.
          if (isset($menu_links[$item['mlid']])) {
            // Replace the instance value with the menu link data.
            $items[$id][$delta] = $menu_links[$item['mlid']];
          }
          // Otherwise, unset the instance value, since the menu link does not exist.
          else {
            unset($items[$id][$delta]);
            $rekey = TRUE;
          }
        }

        if ($rekey) {
          // Rekey the items array.
          $items[$id] = array_values($items[$id]);
        }
      }
    }
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function menu_link_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  // If the default formatter is set to "hidden" but the field is being
  // displayed using this formatter, some module don't properly invoke
  // hook_field_formatter_prepare_view() which is essential for this formatter.
  // Let's make sure it has been invoked.
  // TODO remove if resolved.
  if ($instance['display']['default']['type'] == 'hidden') {
    list($id, , ) = entity_extract_ids($entity_type, $entity);
  	$items = array($id => &$items);
    menu_link_field_formatter_prepare_view(
      $entity_type,
      array($id => $entity),
      $field,
      array($id => $instance),
      $langcode,
      $items,
      array($id => $display)
    );
    $items = $items[$id];
  }

  switch ($display['type']) {
    case 'menu_link_link':
      foreach ($items as $delta => $item) {
        // Don't display a link if the entity wasn't saved yet, i.e. in preview.
        if (empty($item['href'])) {
          $menus = menu_get_menus();
          $element[$delta] = array(
            '#markup' => '<span class="menu-link-preview-title">' .
              $item['link_title'] . '</span>&nbsp;' . t('in') . '&nbsp;' .
             '<span class="menu-link-preview-menu">' .$menus[$item['menu_name']] .
             '</span>',
          );
        }
        else {
          $element[$delta] = array(
            '#type' => 'link',
            '#title' => $item['title'],
            '#href' => $item['href'],
            '#options' => $item['localized_options'],
          );
        }
      }
    break;
  }

  return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function menu_link_field_widget_info() {
  return array(
    'menu_link_default' => array(
      'label' => t('Select list'),
      'field types' => array('menu_link'),
      'settings' => array(
        'description_field' => TRUE,
        'expanded_field' => FALSE
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function menu_link_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $form['description_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable <em>Description</em> field'),
    '#default_value' => $settings['description_field'],
    '#description' => t('The description field is used as a tooltip when the mouse hovers over the menu link.'),
    '#weight' => 5,
  );
  $form['expanded_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable <em>Expanded</em> field'),
    '#default_value' => $settings['expanded_field'],
    '#description' => t('The expanded field is a checkbox which, if selected and the menu link has children, makes the menu link always appear expanded.'),
    '#weight' => 5,
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function menu_link_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $module_path = drupal_get_path('module', 'menu_link');

  $element += array(
    '#input' => TRUE,
    '#type' => ($field['cardinality'] == 1) ? 'fieldset' : 'container',
    '#element_validate' => array('menu_link_field_widget_validate'),
    '#attached' => array(
      'js' => array($module_path . '/menu_link.field.js'),
      'css' => array($module_path . '/menu_link.field.css'),
    ),
    '#attributes' => array(
      'class' => array('menu-link-item', 'menu-link-auto-title', 'menu-link-auto-fieldset-summary'),
    )
  );

  // Populate the element with the link data.
  foreach (array('mlid', 'link_path', 'options', 'hidden', 'expanded') as $key) {
    if (isset($items[$delta][$key])) {
      $element[$key] = array('#type' => 'value', '#value' => $items[$delta][$key]);
    }
  }

  $menus = menu_get_menus();
  $available_menus = array_combine($instance['settings']['menu_options'], $instance['settings']['menu_options']);
  $menu_item = array(
    'mlid' => !empty($items[0]['mlid']) ? $items[0]['mlid'] : 0,
    'has_children' => FALSE,
  );
  $options = _menu_get_options($menus, $available_menus, $menu_item);

  $element['parent'] = array(
    '#type' => 'select',
    '#title' => t('Parent menu item'),
    '#options' => $options,
    '#empty_value' => '_none',
    '#attributes' => array(
      'class' => array('menu-link-item-parent'),
      'title' => t('Choose the menu item to be the parent for this link.'),
    ),
    '#required' => !empty($element['#required']),
  );
  if (isset($items[$delta]['menu_name'], $items[$delta]['plid'])) {
    $element['parent']['#default_value'] = $items[$delta]['menu_name'] . ':' . $items[$delta]['plid'];
  }

  $element['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 50,
    '#default_value' => isset($items[$delta]['weight']) ? $items[$delta]['weight'] : 0,
    '#attributes' => array(
      'class' => array('menu-link-item-weight'),
      'title' => t('Menu links with smaller weights are displayed before links with larger weights.'),
    ),
  );

  $element['link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($items[$delta]['link_title']) ? $items[$delta]['link_title'] : '',
    '#size' => 50,
    '#maxlength' => 255,
    '#attributes' => array(
      'class' => array('menu-link-item-title'),
    ),
    '#description' => t('The text to be used for this link in the menu.'),
    '#required' => !empty($element['#required']),
  );

  if (!empty($field['settings']['link_path_field'])) {
    $element['link_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path'),
      '#default_value' => isset($items[$delta]['link_path']) ? $items[$delta]['link_path'] : '',
      '#size' => 50,
      '#maxlength' => 255,
      '#attributes' => array(
        'class' => array('menu-link-item-path'),
      ),
      '#description' => t('The path for this menu link.'),
    );
  }

  if (!empty($instance['widget']['settings']['fragment_field'])) {
    $element['fragment'] = array(
      '#type' => 'textfield',
      '#title' => t('URL Fragment'),
      '#field_prefix' => '#',
      '#default_value' => isset($items[$delta]['options']['attributes']['fragment']) ? $items[$delta]['options']['attributes']['fragment'] : '',
      '#size' => 10,
      '#maxlength' => 255,
      '#attributes' => array(
        'class' => array('menu-link-item-fragment'),
      ),
    );
  }

  if (!empty($instance['widget']['settings']['expanded_field'])) {
    $element['expanded'] = array(
      '#type' => 'checkbox',
      '#title' => t('Expanded'),
      '#title_display' => 'before',
      '#default_value' => isset($items[$delta]['expanded']) ? $items[$delta]['expanded'] : 0,
      '#attributes' => array(
        'class' => array('menu-link-item-expanded'),
        'title' => t('If selected and this menu link has children, the menu will always appear expanded.'),
      ),
      '#weight' => 5,
    );
  }

  if (!empty($instance['widget']['settings']['description_field'])) {
    $element['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => isset($items[$delta]['options']['attributes']['title']) ? $items[$delta]['options']['attributes']['title'] : '',
      '#rows' => 1,
      '#description' => t('Shown when hovering over the menu link.'),
      '#attributes' => array('class' => array('menu-link-item-description')),
      '#weight' => 10,
    );
  }

  return $element;
}

/**
 * Form element validate handler for menu link field widget.
 */
function menu_link_field_widget_validate($element, &$form_state) {
  $item = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  if (!empty($item['parent']) && $item['parent'] != '_none') {
    list($item['menu_name'], $item['plid']) = explode(':', $item['parent']);
  }

  $item['link_title'] = trim($item['link_title']);

  if (!empty($item['description']) && trim($item['description'])) {
    $item['options']['attributes']['title'] = trim($item['description']);
  }
  else {
    // If the description field was left empty, remove the title attribute
    // from the menu link.
    unset($item['options']['attributes']['title']);
  }

  if (!empty($item['fragment']) && trim($item['fragment'])) {
    $item['options']['fragment'] = trim($item['fragment']);
  }
  else {
    unset($item['options']['fragment']);
  }

  form_set_value($element, $item, $form_state);
}

/**
 * Implements hook_field_widget_error().
 */
function menu_link_field_widget_error($element, $error, $form, &$form_state) {
  switch ($error['error']) {
    case 'menu_link_invalid_parent':
      $error_element = $element['parent'];
      break;

    default:
      $error_element = $element['link_title'];
      break;
  }

  form_error($error_element, $error['message']);
}
