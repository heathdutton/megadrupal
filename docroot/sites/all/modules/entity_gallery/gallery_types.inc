<?php

/**
 * @file
 * Gallery type editing user interface.
 */

/**
 * Displays the gallery type admin overview page.
 */
function entity_gallery_overview_types() {
  $types = entity_gallery_type_get_types();
  $names = entity_gallery_type_get_names();
  $field_ui = module_exists('field_ui');
  $header = array(t('Name'), array('data' => t('Operations'), 'colspan' => $field_ui ? '4' : '2'));
  $rows = array();

  foreach ($names as $key => $name) {
    $type = $types[$key];
    if (entity_gallery_hook($type->type, 'form')) {
      $type_url_str = str_replace('_', '-', $type->type);
      $row = array(theme('entity_gallery_admin_overview', array('name' => $name, 'type' => $type)));
      // Set the edit column.
      $row[] = array('data' => l(t('edit'), 'admin/structure/gallery-types/manage/' . $type_url_str));

      if ($field_ui) {
        // Manage fields.
        $row[] = array('data' => l(t('manage fields'), 'admin/structure/gallery-types/manage/' . $type_url_str . '/fields'));

        // Display fields.
        $row[] = array('data' => l(t('manage display'), 'admin/structure/gallery-types/manage/' . $type_url_str . '/display'));
      }

      // Set the delete column.
      if ($type->custom) {
        $row[] = array('data' => l(t('delete'), 'admin/structure/gallery-types/manage/' . $type_url_str . '/delete'));
      }
      else {
        $row[] = array('data' => '');
      }

      $rows[] = $row;
    }
  }

  $build['entity_gallery_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No gallery types available. <a href="@link">Add gallery type</a>.', array('@link' => url('admin/structure/gallery-types/add'))),
  );

  return $build;
}

/**
 * Returns HTML for a entity gallery type description for the gallery type admin overview page.
 *
 * @param $variables
 *   An associative array containing:
 *   - name: The human-readable name of the gallery type.
 *   - type: An object containing the 'type' (machine name) and 'description' of
 *     the gallery type.
 *
 * @ingroup themeable
 */
function theme_entity_gallery_admin_overview($variables) {
  $name = $variables['name'];
  $type = $variables['type'];

  $output = check_plain($name);
  $output .= ' <small>' . t('(Machine name: @type)', array('@type' => $type->type)) . '</small>';
  $output .= '<div class="description">' . filter_xss_admin($type->description) . '</div>';
  return $output;
}

/**
 * Form constructor for the entity gallery type editing form.
 *
 * @param $type
 *   (optional) An object representing the entity gallery type, when editing an
 *   existing entity gallery type.
 *
 * @see entity_gallery_type_form_validate()
 * @see entity_gallery_type_form_submit()
 * @ingroup forms
 */
function entity_gallery_type_form($form, &$form_state, $type = NULL) {
  if (!isset($type->type)) {
    // This is a new type. Entity gallery module managed types are custom and
    // unlocked.
    $type = entity_gallery_type_set_defaults(array('custom' => 1, 'locked' => 0));
  }

  // Without javascript we need an extra "Choose" button, which is hidden with
  // CSS when we have javascript enabled.
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'entity_gallery') . '/entity_gallery.admin.css',
  );

  // Make the type object available to implementations of hook_form_alter.
  $form['#entity_gallery_type'] = $type;

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $type->name,
    '#description' => t('The human-readable name of this gallery type. This text will be displayed as part of the list on the <em>Add new gallery</em> page. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => $type->type,
    '#maxlength' => 32,
    '#disabled' => $type->locked,
    '#machine_name' => array(
      'exists' => 'entity_gallery_type_load',
    ),
    '#description' => t('A unique machine-readable name for this gallery type. It must only contain lowercase letters, numbers, and underscores. This name will be used for constructing the URL of the %entity-gallery-add page, in which underscores will be converted into hyphens.', array(
      '%entity-gallery-add' => t('Add new gallery'),
    )),
  );

  // Determine the currently selected entity type.
  $selected = isset($form_state['values']['entity_type']) ? $form_state['values']['entity_type'] : $type->entity_type;

  $options = array();
  $entity_types = entity_get_info();

  foreach ($entity_types as $entity_type => $info) {
    $options[$entity_type] = $info['label'];
  }

  $has_data = FALSE;

  // Disable entity type selection if the entity reference field already
  // contains data.
  $field = field_read_field('entity_gallery_' . $type->entity_type);
  if ($field) {
    $has_data = field_has_data($field);
  }

  $form['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity type'),
    '#default_value' => $type->entity_type,
    '#options' => $options,
    '#empty_option' => t('- Select an entity type -'),
    '#description' => t('The type of entity allowed to be added to the gallery.'),
    '#disabled' => $has_data,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'entity_gallery_dependent_dropdown_degrades_first_callback',
      'wrapper' => 'dropdown-second-replace',
    ),
  );

  // The user must select an entity type before the entity type bundles can be
  // determined.
  $form['select_entity_type'] = array(
    '#type' => 'submit',
    '#value' => t('Choose'),
    '#attributes' => array(
      'class' => array('next-button'),
    ),
  );

  $bundle_options = array();

  if ($selected) {
    $entity_type = entity_get_info($selected);

    // If the entity has bundles, allow option to restrict to bundle(s).
    if (!empty($entity_type['bundles'])) {
      foreach ($entity_type['bundles'] as $bundle_id => $bundle_info) {
        $bundle_options[$bundle_id] = $bundle_info['label'];
      }
    }
  }

  if (isset($options[$selected])) {
    $title = t('@entity_type bundles', array('@entity_type' => $options[$selected]));
  }
  else {
    $title = t('Bundles');
  }

  $entity_type_bundles = unserialize($type->entity_type_bundles);

  $form['entity_type_bundles'] = array(
    '#type' => 'checkboxes',
    '#title' => $title,
    '#default_value' => !empty($entity_type_bundles) ? $entity_type_bundles : array(),
    '#options' => array(),
    '#description' => t('The selected entity type only contains one bundle which is always allowed.'),
    '#prefix' => '<div id="dropdown-second-replace">',
    '#suffix' => '</div>',
  );

  // Alert the user that they must choose an entity_type before selecting
  // entity_type_bundles.
  if (empty($form_state['values']['entity_type'])) {
    $form['entity_type_bundles']['#description'] = t('You must choose an entity type before selecting its bundles.');
  }

  // The selection is hidden if there's just one option, since that's always
  // going to be allowed.
  if (count($bundle_options) > 1) {
    $form['entity_type_bundles']['#options'] = $bundle_options;
    $form['entity_type_bundles']['#description'] = t('Select one or more bundles to restrict adding to. If none are selected, all are allowed.');
  }

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => $type->description,
    '#description' => t('Describe this gallery type. The text will be displayed on the <em>Add new gallery</em> page.'),
  );

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'entity_gallery') . '/gallery_types.js'),
    ),
  );

  $form['submission'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submission form settings'),
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
  );
  $form['submission']['title_label'] = array(
    '#title' => t('Title field label'),
    '#type' => 'textfield',
    '#default_value' => $type->title_label,
    '#required' => TRUE,
  );
  if (!$type->has_title) {
    // Avoid overwriting a gallery type that intentionally does not have a
    // title field.
    $form['submission']['title_label']['#attributes'] = array('disabled' => 'disabled');
    $form['submission']['title_label']['#description'] = t('This gallery type does not have a title field.');
    $form['submission']['title_label']['#required'] = FALSE;
  }
  $form['submission']['entity_gallery_preview'] = array(
    '#type' => 'radios',
    '#title' => t('Preview before submitting'),
    '#default_value' => variable_get('entity_gallery_preview_' . $type->type, DRUPAL_OPTIONAL),
    '#options' => array(
      DRUPAL_DISABLED => t('Disabled'),
      DRUPAL_OPTIONAL => t('Optional'),
      DRUPAL_REQUIRED => t('Required'),
    ),
  );
  $form['submission']['help']  = array(
    '#type' => 'textarea',
    '#title' => t('Explanation or submission guidelines'),
    '#default_value' => $type->help,
    '#description' => t('This text will be displayed at the top of the page when creating or editing galleries of this type.'),
  );
  $form['workflow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Publishing options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
  );
  $form['workflow']['entity_gallery_options'] = array('#type' => 'checkboxes',
    '#title' => t('Default options'),
    '#default_value' => variable_get('entity_gallery_options_' . $type->type, array('status')),
    '#options' => array(
      'status' => t('Published'),
      'sticky' => t('Sticky at top of lists'),
      'revision' => t('Create new revision'),
    ),
    '#description' => t('Users with the <em>Administer galleries</em> permission will be able to override these options.'),
  );
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
  );
  $form['display']['entity_gallery_submitted'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display author and date information.'),
    '#default_value' => variable_get('entity_gallery_submitted_' . $type->type, TRUE),
    '#description' => t('Author username and publish date will be displayed.'),
  );
  $form['old_type'] = array(
    '#type' => 'value',
    '#value' => $type->type,
  );
  $form['old_entity_type'] = array(
    '#type' => 'value',
    '#value' => $type->entity_type,
  );
  $form['orig_type'] = array(
    '#type' => 'value',
    '#value' => isset($type->orig_type) ? $type->orig_type : '',
  );
  $form['base'] = array(
    '#type' => 'value',
    '#value' => $type->base,
  );
  $form['custom'] = array(
    '#type' => 'value',
    '#value' => $type->custom,
  );
  $form['modified'] = array(
    '#type' => 'value',
    '#value' => $type->modified,
  );
  $form['locked'] = array(
    '#type' => 'value',
    '#value' => $type->locked,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save gallery type'),
    '#weight' => 40,
  );

  if ($type->custom) {
    if (!empty($type->type)) {
      $form['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete gallery type'),
        '#weight' => 45,
      );
    }
  }

  return $form;
}

/**
 * Selects just the second dropdown to be returned for re-rendering.
 *
 * @return array
 *   Renderable array (the second dropdown).
 */
function entity_gallery_dependent_dropdown_degrades_first_callback($form, $form_state) {
  return $form['entity_type_bundles'];
}

/**
 * Helper function for teaser length choices.
 */
function _entity_gallery_characters($length) {
  return ($length == 0) ? t('Unlimited') : format_plural($length, '1 character', '@count characters');
}

/**
 * Form validation handler for entity_gallery_type_form().
 *
 * @see entity_gallery_type_form_submit()
 */
function entity_gallery_type_form_validate($form, &$form_state) {
  $type = new stdClass();
  $type->type = $form_state['values']['type'];
  $type->name = trim($form_state['values']['name']);

  // Work out what the type was before the user submitted this form
  $old_type = $form_state['values']['old_type'];

  $types = entity_gallery_type_get_names();

  if (!$form_state['values']['locked']) {
    // 'theme' conflicts with theme_entity_gallery_form().
    // '0' is invalid, since elsewhere we check it using empty().
    if (in_array($type->type, array('0', 'theme'))) {
      form_set_error('type', t("Invalid machine-readable name. Enter a name other than %invalid.", array('%invalid' => $type->type)));
    }
  }

  $names = array_flip($types);

  if (isset($names[$type->name]) && $names[$type->name] != $old_type) {
    form_set_error('name', t('The human-readable name %name is already taken.', array('%name' => $type->name)));
  }
}

/**
 * Form submission handler for entity_gallery_type_form().
 *
 * @see entity_gallery_type_form_validate()
 */
function entity_gallery_type_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  $type = entity_gallery_type_set_defaults();

  $type->type = $form_state['values']['type'];
  $type->name = trim($form_state['values']['name']);
  $type->orig_type = trim($form_state['values']['orig_type']);
  $type->old_type = isset($form_state['values']['old_type']) ? $form_state['values']['old_type'] : $type->type;

  $type->entity_type = $form_state['values']['entity_type'];
  $type->old_entity_type = isset($form_state['values']['old_entity_type']) ? $form_state['values']['old_entity_type'] : $type->entity_type;

  // Filter out any unselected checkboxes.
  $type->entity_type_bundles = array_filter($form_state['values']['entity_type_bundles']);

  $type->description = $form_state['values']['description'];
  $type->help = $form_state['values']['help'];
  $type->title_label = $form_state['values']['title_label'];
  // title_label is required in core; has_title will always be true, unless a
  // module alters the title field.
  $type->has_title = ($type->title_label != '');

  $type->base = !empty($form_state['values']['base']) ? $form_state['values']['base'] : 'entity_gallery_content';
  $type->custom = $form_state['values']['custom'];
  $type->modified = TRUE;
  $type->locked = $form_state['values']['locked'];
  if (isset($form['#entity_gallery_type']->module)) {
    $type->module = $form['#entity_gallery_type']->module;
  }

  // Selecting 'Choose' will rebuild the form and present it again.
  if ($op == t('Choose')) {
    $form_state['rebuild'] = TRUE;
    return;
  }

  if ($op == t('Delete gallery type')) {
    $form_state['redirect'] = 'admin/structure/gallery-types/manage/' . str_replace('_', '-', $type->old_type) . '/delete';
    return;
  }

  $variables = $form_state['values'];

  // Remove everything that's been saved already - whatever's left is assumed
  // to be a persistent variable.
  foreach ($variables as $key => $value) {
    if (isset($type->$key)) {
      unset($variables[$key]);
    }
  }

  unset($variables['form_token'], $variables['op'], $variables['submit'], $variables['delete'], $variables['reset'], $variables['form_id'], $variables['form_build_id']);

  // Save or reset persistent variable values.
  foreach ($variables as $key => $value) {
    $variable_new = $key . '_' . $type->type;
    $variable_old = $key . '_' . $type->old_type;

    if (is_array($value)) {
      $value = array_keys(array_filter($value));
    }
    variable_set($variable_new, $value);

    if ($variable_new != $variable_old) {
      variable_del($variable_old);
    }
  }

  // Saving the gallery type after saving the variables allows modules to act
  // on those variables via hook_entity_gallery_type_insert().
  $status = entity_gallery_type_save($type);

  entity_gallery_types_rebuild();
  menu_rebuild();
  $t_args = array('%name' => $type->name);

  if ($status == SAVED_UPDATED) {
    // Accommodate changes to the entity reference entity type.
    if ($type->old_entity_type != $type->entity_type) {
      entity_gallery_delete_entity_reference_field($type, $type->old_entity_type);
      entity_gallery_create_entity_reference_field($type, $type->entity_type, $type->entity_type_bundles);
    }
    else {
      entity_gallery_update_entity_reference_field($type, $type->entity_type, $type->entity_type_bundles);
    }
    drupal_set_message(t('The gallery type %name has been updated.', $t_args));
  }
  elseif ($status == SAVED_NEW) {
    entity_gallery_create_entity_reference_field($type, $type->entity_type, $type->entity_type_bundles);
    entity_gallery_add_description_field($type);
    drupal_set_message(t('The gallery type %name has been added.', $t_args));
    watchdog('entity_gallery', 'Added gallery type %name.', $t_args, WATCHDOG_NOTICE, l(t('view'), 'admin/structure/gallery-types'));
  }

  $form_state['redirect'] = 'admin/structure/gallery-types';
  return;
}

/**
 * Implements hook_entity_gallery_type_insert().
 */
function entity_gallery_entity_gallery_type_insert($info) {
  if (!empty($info->old_type) && $info->old_type != $info->type) {
    $update_count = entity_gallery_type_update_entity_galleries($info->old_type, $info->type);

    if ($update_count) {
      drupal_set_message(format_plural($update_count, 'Changed the gallery type of 1 gallery from %old-type to %type.', 'Changed the gallery type of @count galleries from %old-type to %type.', array('%old-type' => $info->old_type, '%type' => $info->type)));
    }
  }
}

/**
 * Implements hook_entity_gallery_type_update().
 */
function entity_gallery_entity_gallery_type_update($info) {
  if (!empty($info->old_type) && $info->old_type != $info->type) {
    $update_count = entity_gallery_type_update_entity_galleries($info->old_type, $info->type);

    if ($update_count) {
      drupal_set_message(format_plural($update_count, 'Changed the gallery type of 1 gallery from %old-type to %type.', 'Changed the gallery type of @count galleries from %old-type to %type.', array('%old-type' => $info->old_type, '%type' => $info->type)));
    }
  }
}

/**
 * Resets relevant fields of a module-defined entity gallery type to their default values.
 *
 * @param $type
 *   The entity gallery type to reset. The entity gallery type is passed back by
 *   reference with its resetted values. If there is no module-defined info for
 *   this entity gallery type, then nothing happens.
 */
function entity_gallery_type_reset($type) {
  $info_array = module_invoke_all('entity_gallery_info');
  if (isset($info_array[$type->orig_type])) {
    $info_array[$type->orig_type]['type'] = $type->orig_type;
    $info = entity_gallery_type_set_defaults($info_array[$type->orig_type]);

    foreach ($info as $field => $value) {
      $type->$field = $value;
    }
  }
}

/**
 * Menu callback; delete a single gallery type.
 *
 * @ingroup forms
 */
function entity_gallery_type_delete_confirm($form, &$form_state, $type) {
  $form['type'] = array('#type' => 'value', '#value' => $type->type);
  $form['name'] = array('#type' => 'value', '#value' => $type->name);

  $message = t('Are you sure you want to delete the gallery type %type?', array('%type' => $type->name));
  $caption = '';

  $num_galleries = db_query("SELECT COUNT(*) FROM {entity_gallery} WHERE type = :type", array(':type' => $type->type))->fetchField();
  if ($num_galleries) {
    $caption .= '<p>' . format_plural($num_galleries, '%type is used by 1 gallery on your site. If you remove this gallery type, you will not be able to edit the %type gallery and it may not display correctly.', '%type is used by @count galleries on your site. If you remove %type, you will not be able to edit the %type galleries and they may not display correctly.', array('%type' => $type->name)) . '</p>';
  }

  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/structure/gallery-types', $caption, t('Delete'));
}

/**
 * Process gallery type delete confirm submissions.
 *
 * @see entity_gallery_type_delete_confirm()
 */
function entity_gallery_type_delete_confirm_submit($form, &$form_state) {
  entity_gallery_type_delete($form_state['values']['type']);

  variable_del('entity_gallery_preview_' . $form_state['values']['type']);
  $t_args = array('%name' => $form_state['values']['name']);
  drupal_set_message(t('The gallery type %name has been deleted.', $t_args));
  watchdog('entity_gallery', 'Deleted gallery type %name.', $t_args, WATCHDOG_NOTICE);

  entity_gallery_types_rebuild();
  menu_rebuild();

  $form_state['redirect'] = 'admin/structure/gallery-types';
  return;
}
