<?php

/**
 * @file
 * Administrative UI for the Mailin Emailing settings.
 */

use Mailin\Attribute as MA;

/**
 * Form builder; Mailin lists and folders settings.
 *
 * @see mailin_emailing_admin_lists_form_submit()
 * @ingroup forms
 */
function mailin_emailing_admin_lists_form($form, &$form_state) {
  $form['folders'] = array();
  $form['lists'] = array('#tree' => TRUE);

  $lists = DrupalMailin::get()->getLists();
  $settings = variable_get('mailin_emailing_lists', array());

  foreach ($lists as $id => $list) {
    if (!isset($form['folders'][$list['folder_id']])) {
      $form['folders'][$list['folder_id']] = array(
        '#folder_name' => $list['folder_name'],
      );
    }

    $element = &$form['folders'][$list['folder_id']][$id];

    // This elements is outside of the folder structure.
    $form['lists'][$id] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#title_display' => 'invisible',
      '#return_value' => $id,
      '#default_value' => in_array($id, $settings),
    );

    $element['name'] = array(
      '#markup' => $list['name'],
    );

    $element['statistics'] = array(
      '#theme' => 'item_list',
      '#items' => array(
        t('Total subscribers: @count', array('@count' => $list['total_subscribers'])),
        t('Total blacklisted: @count', array('@count' => $list['total_blacklisted'])),
        t('Open rate: @count', array('@count' => $list['open_rate'])),
        t('Click rate: @count', array('@count' => $list['click_rate'])),
      ),
    );
  }//end foreach

  $form['mailin_emailing_flatten_lists'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the lists flatten'),
    '#description' => t('When enabled, the lists are shown at the same level, without reference to their respective folders. This setting is used for the registration and block forms but may be overriden in block configuration.'),
    '#default_value' => variable_get('mailin_emailing_flatten_lists', FALSE),
  );

  $form['mailin_emailing_user_registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the lists on the registration form'),
    '#default_value' => variable_get('mailin_emailing_user_registration', TRUE),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler; save the lists settings.
 *
 * @see mailin_emailing_admin_lists_form()
 */
function mailin_emailing_admin_lists_form_submit($form, &$form_state) {
  foreach (array('mailin_emailing_flatten_lists', 'mailin_emailing_user_registration') as $key) {
    variable_set($key, !empty($form_state['values'][$key]));
  }//end foreach

  variable_set('mailin_emailing_lists', !empty($form_state['values']['lists']) ? array_filter($form_state['values']['lists']) : array());
  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Returns HTML for the lists table form.
 *
 * @ingroup themeable
 */
function theme_mailin_emailing_admin_lists_form($variables) {
  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form['folders']) as $folder_id) {
    $rows[] = array(
      array(
        'data' => $form['folders'][$folder_id]['#folder_name'],
        'header' => TRUE,
        'colspan' => 3,
      ),
    );

    foreach (element_children($form['folders'][$folder_id]) as $list_id) {
      $row = array();
      $element = &$form['folders'][$folder_id][$list_id];

      $row[] = drupal_render($form['lists'][$list_id]);
      $row[] = drupal_render($element['name']);
      $row[] = drupal_render($element['statistics']);
      $rows[] = $row;
    }//end foreach
  }//end foreach

  $output = theme('table', array(
    'header' => array(t('Enabled'), t('Name'), t('Statistics')),
    'rows' => $rows,
    'attributes' => array('id' => 'mailin-lists-overview'),
    'empty' => t('There is no defined list in your Mailin application yet.'),
  ));

  return $output . drupal_render_children($form);
}

/**
 * Form builder; Map the user fields with Mailin attributes.
 *
 * @see mailin_emailing_admin_attributes_form_validate()
 * @see mailin_emailing_admin_attributes_form_submit()
 * @ingroup forms
 */
function mailin_emailing_admin_attributes_form($form, &$form_state) {
  $attributes = DrupalMailin::get()->getAttributes(array(
    MA\Attribute::ATTRIBUTE_LIST_NORMAL,
    MA\Attribute::ATTRIBUTE_LIST_CATEGORY,
    MA\Attribute::ATTRIBUTE_LIST_TRANSACTIONAL,
  ));

  $properties = entity_get_all_property_info('user');
  $mapped_attributes = array();

  $form['#filter'] = new MA\Filter($attributes);
  $form['#mappings'] = mailin_emailing_get_mappings();
  $form['#tree'] = TRUE;

  foreach ($form['#mappings'] as $mapping) {
    $mapped_attributes[$mapping['attribute_type']][] = $mapping['attribute_name'];

    $id = $mapping['id'];
    $form['mappings'][$id]['id'] = array('#type' => 'value', '#value' => $id);
    $form['mappings'][$id]['#errors'] = array();
    $form['mappings'][$id]['attribute_type'] = array('#type' => 'value', '#value' => $mapping['attribute_type']);

    // Check if the attribute still exists (Mailin configuration may change)
    $form['#filter']
      ->setFilter('type', $mapping['attribute_type'])
      ->setFilter('name', $mapping['attribute_name']);

    // Check if the associated field still exists.
    if (isset($properties[$mapping['property_name']])) {
      $form['mappings'][$id]['property_name'] = array(
        '#type' => 'item',
        '#title' => $properties[$mapping['property_name']]['label'],
      );
    }
    else {
      $form['mappings'][$id]['#errors'][] = 'property';
      $form['mappings'][$id]['property_name'] = array(
        '#theme' => 'html_tag',
        '#tag' => 'span',
        '#attributes' => array('class' => array('error')),
        '#value' => t('The property %property does not exist for the user entity.', array('%property' => $mapping['property']['name'])),
      );
    }

    if ($attribute = $form['#filter']->getOne()) {
      $form['mappings'][$id]['attribute_name'] = array(
        '#type' => 'item',
        '#title' => $attribute->getName(),
        '#description' => t('Type of the attribute: @type', array('@type' => $attribute->getType())),
      );
    }
    else {
      $form['mappings'][$id]['#errors'][] = 'attribute';
      $form['mappings'][$id]['attribute_name'] = array(
        '#theme' => 'html_tag',
        '#tag' => 'span',
        '#value' => t('Attribute with name %name is not recognized by the Mailin server.', array('%name' => $attribute->getName())),
        '#attributes' => array('class' => array('error')),
      );
    }

    $form['mappings'][$id]['required'] = array(
      '#type' => 'checkbox',
      '#title' => t('Required'),
      '#title_display' => 'invisible',
      '#default_value' => $mapping['required'],
    );

    $form['mappings'][$id]['delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete'),
      '#title_display' => 'invisible',
      '#default_value' => 0,
    );

    $form['mappings'][$id]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#delta' => 50,
      '#title_display' => 'invisible',
      '#default_value' => $mapping['weight'],
    );
  }//end foreach

  // Add new mapping
  $options = array();
  $field_types = field_info_field_types();
  $property_group = t('User properties');
  $field_group = t('User fields');

  foreach ($properties as $property_name => $property) {
    if (!empty($property['field'])) {
      $field = field_info_field($property_name);
      $options[$field_group][$property_name] = t('@field_name (@type)', array('@field_name' => $property['label'], '@type' => $field_types[$field['type']]['label']));
    }
    else {
      $options[$property_group][$property_name] = $property['label'];
    }
  }//end foreach


  $form['new']['property_name'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#title_display' => 'invisible',
    '#options' => array('' => t('- Select -')) + $options,
    '#title_display' => 'invisible',
    '#description' => t('Drupal field or property'),
  );

  $iterator = new \RecursiveIteratorIterator(new \RecursiveArrayIterator($attributes), \RecursiveIteratorIterator::SELF_FIRST);
  $options = array();

  foreach ($iterator as $key => $value) {
    if (is_array($value) && $value) {
      $options[$key] = array();
      $current = &$options[$key];
    }
    elseif ($value instanceof Mailin\Attribute\AttributeInterface) {
      // Ignore attributes that are already mapped.
      if (!isset($mapped_attributes[$value->getType()]) || !in_array($value->getName(), $mapped_attributes[$value->getType()])) {
        $current[$value->getType() . ':' . $value->getName()] = $value->getName();
      }
    }
  }

  $form['new']['attribute_name'] = array(
    '#title' => t('Map new attribute'),
    '#type' => 'select',
    '#options' => array('' => t('- Select -')) + $options,
    '#description' => t('Mailin attribute'),
  );

  $form['new']['required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Required'),
    '#title_display' => 'invisible',
  );

  $form['new']['weight'] = array(
    '#type' => 'weight',
    '#attributes' => array('class' => array('field-weight')),
    '#prefix' => '<div class="add-new-mapping">&nbsp;</div>',
    '#description' => t('Weight for new mapping'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Returns HTML for the mapping table form.
 *
 * @ingroup themeable
 */
function theme_mailin_emailing_admin_attributes_form($variables) {
  $form = $variables['form'];
  $rows = array();

  if (isset($form['mappings'])) {
    foreach (element_children($form['mappings']) as $key) {
      $element = &$form['mappings'][$key];
      $element['weight']['#attributes']['class'] = array('mapping-weight');

      $row = array();
      $classes = array('draggable');

      if (!empty($element['#errors'])) {
        $classes[] = 'error';
      }

      $row[] = drupal_render($element['property_name']);
      $row[] = drupal_render($element['attribute_name']);
      $row[] = drupal_render($element['required']);
      $row[] = drupal_render($element['delete']);
      $row[] = drupal_render($element['weight']);

      $rows[] = array(
        'data' => $row,
        'class' => $classes,
      );
    }//end foreach
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('There is no defined mapping yet.'), 'colspan' => 4));
  }

  // New mapping row.
  $form['new']['weight']['#attributes']['class'] = array('mapping-weight');

  $rows[] = array(
    'data' => array(
      drupal_render($form['new']['property_name']),
      drupal_render($form['new']['attribute_name']),
      drupal_render($form['new']['required']),
      '&nbsp;',
      drupal_render($form['new']['weight']),
    ),
    'class' => array('draggable'),
  );

  drupal_add_tabledrag('mailin-mappings-overview', 'order', 'sibling', 'mapping-weight');

  $output = theme('table', array(
    'header' => array(t('Drupal user property'), t('Mailin attribute'), t('Required'), t('Delete'), t('Weight')),
    'rows' => $rows,
    'attributes' => array('id' => 'mailin-mappings-overview'),
  ));

  return $output . drupal_render_children($form);
}

/**
 * Form validation handler; mappings form.
 *
 * @see mailin_emailing_admin_attributes_form()
 * @see mailin_emailing_admin_attributes_form_submit()
 */
function mailin_emailing_admin_attributes_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (empty($values['new']['property_name']) xor empty($values['new']['attribute_name'])) {
    $error_element = empty($values['new']['property_name']) ? 'new][property_name' : 'new][attribute_name';
    form_set_error($error_element, t('All required fields need to be filled in order to add a new mapping!'));
  }
}

/**
 * Form submission handler; save the mappings.
 *
 * @see mailin_emailing_admin_attributes_form()
 * @see mailin_emailing_admin_attributes_form_validate()
 */
function mailin_emailing_admin_attributes_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $delete = array();

  // Update existing mappings if necessary
  foreach ($form['#mappings'] as $mapping) {
    $current = $values['mappings'][$mapping['id']];
    $current['required'] = !empty($current['required']);

    if (!empty($current['delete'])) {
      $delete[$current['id']] = $form['#mappings'][$current['id']]['attribute_name'] . ' (' . $current['attribute_type'] . ')';
    }
    elseif ($current['weight'] != $mapping['weight'] || $current['required'] != $mapping['required']) {
      drupal_write_record('mailin_emailing_mapping', $current, array('id'));
    }
  }//end foreach

  if ($delete) {
    db_delete('mailin_emailing_mapping')
      ->condition('id', array_keys($delete))
      ->execute();
    drupal_set_message(format_plural(count($delete), 'The field %fields has been deleted.', 'The following fields have been deleted: %fields.', array('%fields' => implode(', ', $delete))));
  }

  // Save new mapping
  if (!empty($values['new']['property_name'])) {
    $new = $values['new'];
    list($new['attribute_type'], $new['attribute_name']) = explode(':', $new['attribute_name']);
    drupal_write_record('mailin_emailing_mapping', $new);
    drupal_set_message(t('The new mapping has been created.'));
  }
  else {
    drupal_set_message(t('The changes have been saved.'));
  }
}

/**
 * Form builder; add block form.
 *
 * @see mailin_emailing_add_block_form_submit()
 * @ingroup forms
 */
function mailin_emailing_add_block_form($form, &$form_state, $theme = NULL) {
  module_load_include('inc', 'block', 'block.admin');
  $form = block_admin_configure($form, $form_state, 'mailin_emailing', NULL);
  $form['#theme_key'] = $theme;

  // Other modules should be able to use hook_form_block_add_block_form_alter()
  // to modify this form, so add a base form ID.
  $form_state['build_info']['base_form_id'] = 'block_add_block_form';
  $form['#validate'] = array();
  $form['#submit'] = array('mailin_emailing_add_block_form_submit');

  return $form;
}

/**
 * Form submission handler; save the block.
 *
 * @see mailin_emailing_add_block_form()
 */
function mailin_emailing_add_block_form_submit($form, &$form_state) {
  $blocks = variable_get('mailin_emailing_blocks', array());
  $form_state['values']['delta'] = $delta = empty($blocks) ? 1 : max(array_keys($blocks)) + 1;

  module_invoke('mailin_emailing', 'block_save', $delta, $form_state['values']);

  // Now duplicate the block_add_block_form_submit) function.
  $query = db_insert('block')->fields(array('visibility', 'pages', 'custom', 'title', 'module', 'theme', 'region', 'status', 'weight', 'delta', 'cache'));
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      $region = !empty($form_state['values']['regions'][$theme->name]) ? $form_state['values']['regions'][$theme->name] : BLOCK_REGION_NONE;
      $query->values(array(
        'visibility' => (int) $form_state['values']['visibility'],
        'pages' => trim($form_state['values']['pages']),
        'custom' => (int) $form_state['values']['custom'],
        'title' => $form_state['values']['title'],
        'module' => $form_state['values']['module'],
        'theme' => $theme->name,
        'region' => ($region == BLOCK_REGION_NONE ? '' : $region),
        'status' => 0,
        'status' => (int) ($region != BLOCK_REGION_NONE),
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_NO_CACHE,
      ));
    }
  }
  $query->execute();

  $query = db_insert('block_role')->fields(array('rid', 'module', 'delta'));
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    $query->values(array(
      'rid' => $rid,
      'module' => $form_state['values']['module'],
      'delta' => $delta,
    ));
  }
  $query->execute();

  drupal_set_message(t('The block has been created.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block' . isset($form['#theme_key']) ? '/' . $form['#theme_key'] : '';
}

/**
 * Form builder; deletion confirm form for a given block.
 *
 * @see mailin_emailing_block_delete_form_submit()
 * @ingroup forms
 */
function mailin_emailing_block_delete_form($form, &$form_state, $delta) {
  $blocks = variable_get('mailin_emailing_blocks', array());

  if (!isset($blocks[$delta])) {
    drupal_not_found();
    drupal_exit();
  }

  return confirm_form(
    array(
      'delta' => array('#type' => 'value', '#value' => $delta),
    ),
    t('Are you sure you want to delete the emailing block %name?', array('%name' => $blocks[$delta]['info'])),
    'admin/structure/block',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

/**
 * Form submission handler; delete an emailing block.
 *
 * @see mailin_emailing_block_delete_form()
 */
function mailin_emailing_block_delete_form_submit($form, &$form_state) {
  $delta = $form_state['values']['delta'];
  $blocks = variable_get('mailin_emailing_blocks', array());
  unset($blocks[$delta]);
  variable_set('mailin_emailing_blocks', $blocks);

  db_delete('block')
    ->condition('module', 'mailin_emailing')
    ->condition('delta', $delta)
    ->execute();

  db_delete('block_role')
    ->condition('module', 'mailin_emailing')
    ->condition('delta', $delta)
    ->execute();

  cache_clear_all();
  drupal_set_message(t('The emailing block has been removed.'));
  $form_state['redirect'] = 'admin/structure/block';
}
