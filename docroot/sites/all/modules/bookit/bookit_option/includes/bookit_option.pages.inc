<?php

/**
 * Menu callback. List of booking options.
 */
function bookit_option_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // If only one type exists then go directly to the form.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  return theme('option_add_list', array('content' => $content));
}

/**
 * Displays the list of booking option types.
 */
function theme_option_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="bookit-option-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer bookit_option types')) {
      $output = '<p>' . t('You have not created any option types yet. Go to the <a href="@create-option-type">option type creation page</a> to add a new option type.', array('@create-option-type' => url('admin/bookit/config/options/types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No option types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Menu callback: display an overview of available types.
 */
function bookit_option_types_overview() {
  $header = array(
    t('Name'),
    t('Operations'),
  );

  $rows = array();

  // Loop through all defined option types.
  foreach (bookit_option_types() as $type => $option_type) {
    // Build the operation links for the current option type.
    $links = menu_contextual_links('bookit-option-type', 'admin/bookit/config/options/types', array(strtr($type, array('_' => '-'))));

    // Add the option type's row to the table's rows array.
    $rows[] = array(
      theme('option_type_admin_overview', array('option_type' => $option_type)),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  // If no option types are defined...
  if (empty($rows)) {
    // Add a standard empty row with a link to add a new option type.
    $rows[] = array(
      array(
        'data' => t('There are no option types yet. <a href="@link">Add booking option type</a>.', array('@link' => url('admin/bookit/config/options/types/add'))),
        'colspan' => 2,
      )
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Builds an overview of an option type for display to an administrator.
 */
function theme_option_type_admin_overview($variables) {
  $option_type = $variables['option_type'];

  $output = check_plain($option_type['name']);
  $output .= ' <small>' . t('(Machine name: @type)', array('@type' => $option_type['type'])) . '</small>';
  $output .= '<div class="description">' . filter_xss_admin($option_type['description']) . '</div>';

  return $output;
}


/**
 * Form callback for creating booking options.
 */
function bookit_option_option_form($form, &$form_state, $bookit_option) {
  // Store bookit_option in form state.
  $form_state['bookit_option'] = $bookit_option;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $bookit_option->title,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  // Get entity language.
  $langcode = entity_language('bookit_option', $bookit_option);
  if (empty($langcode)) {
    $langcode = LANGUAGE_NONE;
  }

  field_attach_form('bookit_option', $bookit_option, $form, $form_state, $langcode);

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 400,
  );

  $form['language'] = array(
    '#type' => 'value',
    '#value' => $langcode,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('bookit_option_option_form_submit'),
  );

  $form['#validate'][] = 'bookit_option_option_form_validate';

  return $form;
}

/**
 * Validation callback for bookit_option_option_form().
 */
function bookit_option_option_form_validate($form, &$form_state) {
  // Validate fields.
  $bookit_option = $form_state['bookit_option'];
  field_attach_form_validate('bookit_option', $bookit_option, $form, $form_state);
}

/**
 * Submit callback for bookit_option_option_form().
 */
function bookit_option_option_form_submit($form, &$form_state) {
  global $user;

  $bookit_option = &$form_state['bookit_option'];

  // Save default parameters back into the $bookit_option object.
  $bookit_option->title = $form_state['values']['title'];
  $bookit_option->language = $form_state['values']['language'];

  // Set the option's uid if it's being created at this time.
  if (empty($bookit_option->option_id)) {
    $bookit_option->uid = $user->uid;
  }

  // Notify field widgets.
  field_attach_submit('bookit_option', $bookit_option, $form, $form_state);

  // Save
  bookit_option_save($bookit_option);

  // Redirect based on the button clicked.
  drupal_set_message(t('Booking option saved.'));
  $form_state['redirect'] = 'admin/bookit/config/options';
}

/**
 * Form callback for creating booking options.
 */
function bookit_option_option_delete_form($form, &$form_state, $bookit_option) {
  $form_state['bookit_option'] = $bookit_option;

  // Return a message if the option is the standard.
  if ($bookit_option->option_id == 0) {
    return array('#prefix' => t('This booking option type cannot be deleted, because it is the standard option.'));
  }

  // TODO: Don't allow delete options that already assigned to a bookable item.

  return confirm_form($form,
    t('Are you sure you want to delete the %name booking option?', array('%name' => $bookit_option->title)),
    'admin/bookit/config/options',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
}

/**
 * Submit callback for bookit_option_option_delete_form().
 */
function bookit_option_option_delete_form_submit($form, &$form_state) {
  $bookit_option = $form_state['bookit_option'];
  $title = $bookit_option->title;
  bookit_option_delete($bookit_option->option_id);
  drupal_set_message(t('Booking option @title has been deleted!', array('@title' => $title)));
  $form_state['redirect'] = 'admin/bookit/config/options';
}

/**
 * Form callback for creating booking options.
 */
function bookit_option_type_form($form, &$form_state, $option_type) {
  // Store the initial option type.
  $option_type = (is_array($option_type)) ? $option_type : bookit_option_type_load($option_type);
  $form_state['option_type'] = $option_type;

  $form['option_type'] = array(
    '#tree' => TRUE,
  );

  $form['option_type']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $option_type['name'],
    '#description' => t('The human-readable name of this option type.'),
    '#required' => TRUE,
    '#size' => 32,
  );

  if (empty($option_type['type'])) {
    $form['option_type']['type'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $option_type['type'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'bookit_option_type_load',
        'source' => array('option_type', 'name'),
      ),
      '#description' => t('Must contain only lowercase letters, numbers, and underscores, it must be unique.'),
    );
  }

  $form['option_type']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this option type. The text will be displayed on the <em>Add new content</em> page.'),
    '#default_value' => $option_type['description'],
    '#rows' => 3,
  );

  $form['option_type']['help'] = array(
    '#type' => 'textarea',
    '#title' => t('Explanation or submission guidelines'),
    '#description' => t('This text will be displayed at the top of the page when creating or editing booking options of this type.'),
    '#default_value' => $option_type['help'],
    '#rows' => 3,
  );

  if (module_exists('entity_translation')) {
    $form['option_type']['multilingual'] = array(
      '#type' => 'radios',
      '#title' => t('Multilingual support'),
      '#description' => t('If <em>Entity translation</em> is enabled it will be possible to provide a different version of the same option for each available language.') . '<br />' . t('You can find more options in the <a href="!url">entity translation settings</a>.', array('!url' => url('admin/config/regional/entity_translation'))) . '<br />' . t('Existing options will not be affected by changing this option.'),
      '#options' => array(
        0 => t('Disabled'),
        ENTITY_TRANSLATION_ENABLED => t('Enabled via <em>Entity translation</em>'),
      ),
      '#default_value' => variable_get('language_bookit_option_type_' . $option_type['type'], 0),
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 40,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save booking option type'),
    '#submit' => array('bookit_option_type_form_submit'),
  );

  if (!empty($form_state['option_type']['type'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete booking option type'),
      '#suffix' => l(t('Cancel'), 'admin/bookit/config/options/types'),
      '#submit' => array('bookit_option_type_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['save_continue'] = array(
      '#type' => 'submit',
      '#value' => t('Save and add fields'),
      '#suffix' => l(t('Cancel'), 'admin/bookit/config/options/types'),
      '#submit' => array('bookit_option_type_form_submit'),
      '#weight' => 45,
    );
  }

  $form['#validate'][] = 'bookit_option_type_form_validate';

  return $form;
}

/**
 * Form validate callback.
 */
function bookit_option_type_form_validate($form, &$form_state) {
  $option_type = $form_state['option_type'];

  if (empty($option_type['type'])) {
    if (db_query('SELECT type FROM {bookit_option_type} WHERE type = :type', array(':type' => $form_state['values']['option_type']['type']))->fetchField()) {
      form_set_error('option_type][type', t('The machine name specified is already in use.'));
    }
  }

  return $form;
}

/**
 * Form submit callback.
 */
function bookit_option_type_form_submit($form, &$form_state) {
  $option_type = $form_state['option_type'];
  $updated = !empty($option_type['type']);

  // If a type is set, we should still check to see if a row for the type exists
  // in the database; this is done to accomodate types defined by Features.
  if ($updated) {
    $updated = db_query('SELECT 1 FROM {bookit_option_type} WHERE type = :type', array(':type' => $option_type['type']))->fetchField();
  }

  foreach ($form_state['values']['option_type'] as $key => $value) {
    $option_type[$key] = $value;
  }

  // Write the option type to the database.
  $option_type['is_new'] = !$updated;
  bookit_option_type_save($option_type);

  // Set the multingual value for the booking option type if entity translation is enabled.
  if (module_exists('entity_translation')) {
    variable_set('language_bookit_option_type_' . $option_type['type'], $option_type['multilingual']);
  }

  // Redirect based on the button clicked.
  drupal_set_message(t('Booking option type saved.'));

  if ($form_state['triggering_element']['#parents'][0] == 'save_continue') {
    $form_state['redirect'] = 'admin/bookit/config/options/types/' . strtr($option_type['type'], '_', '-') . '/fields';
  }
  else {
    $form_state['redirect'] = 'admin/bookit/config/options/types';
  }
}

/**
 * Form delete submit callback.
 */
function bookit_option_type_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/bookit/config/options/types/' . strtr($form_state['option_type']['type'], '_', '-') . '/delete';
}

/**
 * Form callback: confirmation form for deleting a option type.
 */
function bookit_option_type_delete_form($form, &$form_state, $option_type) {
  $option_type = (is_array($option_type)) ? $option_type : bookit_option_type_load($option_type);
  $form_state['option_type'] = $option_type;

  // Return a message if the option type is not governed by Booking option module.
  if ($option_type['module'] != 'bookit_option') {
    return array('#prefix' => t('This booking option type cannot be deleted, because it is not defined by the Booking option module.'));
  }

  // Don't allow deletion of option types that have options already.
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'bookit_option', '=')
    ->entityCondition('bundle', $option_type['type'], '=')
    ->count();

  $count = $query->execute();

  if ($count > 0) {
    drupal_set_title(t('Cannot delete the %name booking option type', array('%name' => $option_type['name'])), PASS_THROUGH);

    return array('#prefix' => format_plural($count,
      'There is 1 booking option of this type. It cannot be deleted.',
      'There are @count booking options of this type. It cannot be deleted.'
    ));
  }

  $form['#submit'][] = 'bookit_option_type_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete the %name booking option type?', array('%name' => $option_type['name'])),
    'admin/bookit/config/options/types',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for bookit_option_type_delete_form().
 */
function bookit_option_type_delete_form_submit($form, &$form_state) {
  $option_type = $form_state['option_type'];

  bookit_option_type_delete($option_type['type']);

  drupal_set_message(t('The booking option type %name has been deleted.', array('%name' => $option_type['name'])));
  watchdog('bookit_option', 'Deleted booking option type %name.', array('%name' => $option_type['name']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/bookit/config/options/types';
}
