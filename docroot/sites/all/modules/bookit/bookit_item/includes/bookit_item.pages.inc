<?php

/**
 * Menu callback. List of bookable items.
 */
function bookit_item_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // If only one booking type exists then go directly to the form.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  return theme('item_add_list', array('content' => $content));
}

/**
 * Displays the list of booking types.
 */
function theme_item_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="bookit-item-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer bookit_item types')) {
      $output = '<p>' . t('You have not created any booking types yet. Go to the <a href="@create-item-type">booking type creation page</a> to add a new type.', array('@create-item-type' => url('admin/bookit/config/types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No booking types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Menu callback: display an overview of available booking types.
 */
function bookit_item_types_overview() {
  $header = array(
    t('Name'),
    t('Operations'),
  );

  $rows = array();

  // Loop through all defined booking types.
  foreach (bookit_item_types() as $type => $item_type) {

    // Build the operation links for the current booking type.
    $links = menu_contextual_links('bookit-item-type', 'admin/bookit/config/types', array(strtr($type, array('_' => '-'))));

    // Add the booking type's row to the table's rows array.
    $rows[] = array(
      theme('item_type_admin_overview', array('item_type' => $item_type)),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  // If no booking types are defined.
  if (empty($rows)) {

    // Add a standard empty row with a link to add a new booking type.
    $rows[] = array(
      array(
        'data' => t('There are no booking types yet. <a href="@link">Add a booking type</a>.', array('@link' => url('admin/bookit/config/types/add'))),
        'colspan' => 2,
      )
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Builds an overview of a booking type for display to an administrator.
 */
function theme_item_type_admin_overview($variables) {
  $item_type = $variables['item_type'];

  $output = check_plain($item_type['name']);
  $output .= ' <small>' . t('(Machine name: @type)', array('@type' => $item_type['type'])) . '</small>';
  $output .= '<div class="description">' . filter_xss_admin($item_type['description']) . '</div>';

  return $output;
}

/**
 * Form callback for creating bookable items.
 */
function bookit_item_item_form($form, &$form_state, $bookit_item) {
  // Store bookable item in form state.
  $form_state['bookit_item'] = $bookit_item;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $bookit_item->title,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  // Get entity language.
  $langcode = entity_language('bookit_item', $bookit_item);
  if (empty($langcode)) {
    $langcode = LANGUAGE_NONE;
  }

  field_attach_form('bookit_item', $bookit_item, $form, $form_state, $langcode);

  $form['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Disabled bookable items are not displayed in frontend results.'),
    '#options' => array(t('Disabled'), t('Active')),
    '#default_value' => $bookit_item->status,
    '#required' => TRUE,
    '#weight' => 35,
  );

  $item_type = bookit_item_type_load($bookit_item->type);

  $form['change_history'] = array(
    '#type' => 'fieldset',
    '#title' => t('Change history'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($bookit_item->item_id) || empty($item_type['revision']),
    '#weight' => 350,
  );

  if (!empty($bookit_item->item_id)) {
    $form['change_history']['revision'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create new revision on update'),
      '#description' => t('If an update log message is entered, a revision will be created even if this is unchecked.'),
      '#default_value' => $item_type['revision'],
      '#access' => user_access('administer bookit_item entities'),
    );
  }

  $form['change_history']['log'] = array(
    '#type' => 'textarea',
    '#title' => !empty($bookit_item->item_id) ? t('Update log message') : t('Creation log message'),
    '#rows' => 4,
    '#description' => t('Provide an explanation of the changes you are making. This will provide a meaningful history of changes to this bookable item.'),
  );

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
    '#submit' => array('bookit_item_item_form_submit'),
  );

  $form['#validate'][] = 'bookit_item_item_form_validate';

  return $form;
}

/**
 * Validation callback for bookit_item_item_form().
 */
function bookit_item_item_form_validate($form, &$form_state) {
  // Validate fields.
  $bookit_item = $form_state['bookit_item'];
  field_attach_form_validate('bookit_item', $bookit_item, $form, $form_state);
}

/**
 * Submit callback for bookit_item_item_form().
 */
function bookit_item_item_form_submit($form, &$form_state) {
  global $user;

  $bookit_item = &$form_state['bookit_item'];

  // Save default parameters back into the $bookit_item object.
  $bookit_item->title = $form_state['values']['title'];
  $bookit_item->status = $form_state['values']['status'];
  $bookit_item->language = $form_state['values']['language'];

  // Set the bookable items's uid if it's being created at this time.
  if (empty($bookit_item->item_id)) {
    $bookit_item->uid = $user->uid;
  }

  // Trigger a new revision if the checkbox was enabled or a log message supplied.
  if ((user_access('administer bookit_item entities') && !empty($form_state['values']['revision'])) ||
    (!user_access('administer bookit_item entities') && !empty($form['change_history']['revision']['#default_value'])) ||
    !empty($form_state['values']['log'])) {
    $bookit_item->revision = TRUE;
    $bookit_item->log = $form_state['values']['log'];
  }

  // Notify field widgets.
  field_attach_submit('bookit_item', $bookit_item, $form, $form_state);

  // Save
  bookit_item_save($bookit_item);

  // Redirect based on the button clicked.
  drupal_set_message(t('Item saved.'));

  // Redirect to the bookit_item edit page.
  $form_state['redirect'] = 'admin/bookit/items/' . $bookit_item->item_id;
}

/**
 * Form callback for creating bookable items.
 */
function bookit_item_item_delete_form($form, &$form_state, $bookit_item) {
  $form_state['bookit_item'] = $bookit_item;

  // TODO: Don't allow delete booked items.

  return confirm_form($form,
    t('Are you sure you want to delete the %name item?', array('%name' => $bookit_item->title)),
    'admin/bookit/items',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
}

/**
 * Submit callback for bookit_item_item_delete_form().
 */
function bookit_item_item_delete_form_submit($form, &$form_state) {
  $bookit_item = $form_state['bookit_item'];
  $title = $bookit_item->title;
  bookit_item_delete($bookit_item->item_id);
  drupal_set_message(t('Bookable item @title has been deleted!', array('@title' => $title)));
  $form_state['redirect'] = 'admin/bookit/items';
}

/**
 * Form callback for creating bookable items.
 */
function bookit_item_type_form($form, &$form_state, $item_type) {
  // Store the initial booking type.
  $item_type = (is_array($item_type)) ? $item_type : bookit_item_type_load($item_type);
  $form_state['item_type'] = $item_type;

  $form['item_type'] = array(
    '#tree' => TRUE,
  );

  $form['item_type']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $item_type['name'],
    '#description' => t('The human-readable name of this booking type.'),
    '#required' => TRUE,
    '#size' => 32,
  );

  if (empty($item_type['type'])) {
    $form['item_type']['type'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $item_type['type'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'bookit_item_type_load',
        'source' => array('item_type', 'name'),
      ),
      '#description' => t('Must contain only lowercase letters, numbers, and underscores, it must be unique.'),
    );
  }

  $form['item_type']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this booking type. The text will be displayed on the <em>Add an item</em> page.'),
    '#default_value' => $item_type['description'],
    '#rows' => 3,
  );

  $form['item_type']['help'] = array(
    '#type' => 'textarea',
    '#title' => t('Explanation or submission guidelines'),
    '#description' => t('This text will be displayed at the top of the page when creating or editing bookable items of this type.'),
    '#default_value' => $item_type['help'],
    '#rows' => 3,
  );

  $form['item_type']['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Default items of this booking type to be saved as new revisions when edited.'),
    '#default_value' => $item_type['revision'],
  );

  if (module_exists('entity_translation')) {
    $form['item_type']['multilingual'] = array(
      '#type' => 'radios',
      '#title' => t('Multilingual support'),
      '#description' => t('If <em>Entity translation</em> is enabled it will be possible to provide a different version of the same bookable item for each available language.') . '<br />' . t('You can find more options in the <a href="!url">entity translation settings</a>.', array('!url' => url('admin/config/regional/entity_translation'))) . '<br />' . t('Existing bookable items will not be affected by changing this option.'),
      '#options' => array(
        0 => t('Disabled'),
        ENTITY_TRANSLATION_ENABLED => t('Enabled via <em>Entity translation</em>'),
      ),
      '#default_value' => variable_get('language_bookit_item_type_' . $item_type['type'], 0),
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 40,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save booking type'),
    '#submit' => array('bookit_item_type_form_submit'),
  );

  if (!empty($form_state['item_type']['type'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete booking type'),
      '#suffix' => l(t('Cancel'), 'admin/bookit/config/types'),
      '#submit' => array('bookit_item_type_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['save_continue'] = array(
      '#type' => 'submit',
      '#value' => t('Save and add fields'),
      '#suffix' => l(t('Cancel'), 'admin/bookit/config/types'),
      '#submit' => array('bookit_item_type_form_submit'),
      '#weight' => 45,
    );
  }

  $form['#validate'][] = 'bookit_item_type_form_validate';

  return $form;
}

/**
 * Form validate callback.
 */
function bookit_item_type_form_validate($form, &$form_state) {
  $item_type = $form_state['item_type'];

  if (empty($item_type['type'])) {
    if (db_query('SELECT type FROM {bookit_item_type} WHERE type = :type', array(':type' => $form_state['values']['item_type']['type']))->fetchField()) {
      form_set_error('item_type][type', t('The machine name specified is already in use.'));
    }
  }

  return $form;
}

/**
 * Form submit callback.
 */
function bookit_item_type_form_submit($form, &$form_state) {
  $item_type = $form_state['item_type'];
  $updated = !empty($item_type['type']);

  // If a booking type is set, we should still check to see if a row for the booking type exists
  // in the database; this is done to accomodate booking types defined by Features.
  if ($updated) {
    $updated = db_query('SELECT 1 FROM {bookit_item_type} WHERE type = :type', array(':type' => $item_type['type']))->fetchField();
  }

  foreach ($form_state['values']['item_type'] as $key => $value) {
    $item_type[$key] = $value;
  }

  // Write the booking type to the database.
  $item_type['is_new'] = !$updated;
  bookit_item_type_save($item_type);

  // Set the multingual value for the booking type if entity translation is enabled.
  if (module_exists('entity_translation')) {
    variable_set('language_bookit_item_type_' . $item_type['type'], $item_type['multilingual']);
  }

  // Redirect based on the button clicked.
  drupal_set_message(t('Item type saved.'));

  if ($form_state['triggering_element']['#parents'][0] == 'save_continue') {
    $form_state['redirect'] = 'admin/bookit/config/types/' . strtr($item_type['type'], '_', '-') . '/fields';
  }
  else {
    $form_state['redirect'] = 'admin/bookit/config/types';
  }
}

/**
 * Form delete submit callback.
 */
function bookit_item_type_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/bookit/config/types/' . strtr($form_state['item_type']['type'], '_', '-') . '/delete';
}

/**
 * Form callback: confirmation form for deleting a booking type.
 */
function bookit_item_type_delete_form($form, &$form_state, $item_type) {
  $item_type = (is_array($item_type)) ? $item_type : bookit_item_type_load($item_type);
  $form_state['item_type'] = $item_type;

  // Return a message if the booking type is not governed by Bookit Item module.
  if ($item_type['module'] != 'bookit_item') {
    return array('#prefix' => t('This booking type cannot be deleted, because it is not defined by the Bookit Item module.'));
  }

  // Don't allow deletion of booking types that have items already.
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'bookit_item', '=')
    ->entityCondition('bundle', $item_type['type'], '=')
    ->count();

  $count = $query->execute();

  if ($count > 0) {
    drupal_set_title(t('Cannot delete the %name booking type', array('%name' => $item_type['name'])), PASS_THROUGH);

    return array('#prefix' => format_plural($count,
      'There is 1 item of this type. It cannot be deleted.',
      'There are @count items of this type. It cannot be deleted.'
    ));
  }

  $form['#submit'][] = 'bookit_item_type_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete the %name booking type?', array('%name' => $item_type['name'])),
    'admin/bookit/config/types',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for bookit_item_type_delete_form().
 */
function bookit_item_type_delete_form_submit($form, &$form_state) {
  $item_type = $form_state['item_type'];

  bookit_item_type_delete($item_type['type']);

  drupal_set_message(t('The booking type %name has been deleted.', array('%name' => $item_type['name'])));
  watchdog('bookit_item', 'Deleted booking type %name.', array('%name' => $item_type['name']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/bookit/config/types';
}
