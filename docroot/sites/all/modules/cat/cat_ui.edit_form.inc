<?php

/**
 * @file The "Edit form" of cat_items.
 *
 * @author Oleksiy Kalinichenko <aexchecker@gmail.com>
 */

/**
 * Form for editing CAT item.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function cat_ui_edit_form($form, &$form_state) {
  // Add form inc file, also see module_load_include()
  form_load_include($form_state, 'inc', 'cat_ui');
  form_load_include($form_state, 'inc', 'cat_ui', 'cat_ui.add_form');
  form_load_include($form_state, 'inc', 'cat_ui', 'cat_ui.edit_form');

  // Load form for creation of CAT item but with existing item in cache.
  $form = cat_ui_add_form(array(), $form_state, FALSE, 'edit-form');

  // Disable machine name edition.
  if (isset($form['machine_name'])) {
    $form['machine_name']['#disabled'] = TRUE;
  }

  // Set two submit buttons for every form.
  $form['actions'] = array(
    '#type' => 'action',
    '#weight' => 100,
  );

  $base = array(
    '#type' => 'submit',
    '#ajax' => array(
      'callback' => 'cat_ui_edit_form_ajax_callback',
      'wrapper' => 'cat-ui-form-wrapper',
    ),
  );

  $form['actions']['update'] = array('#value' => t('Update')) + $base;
  $form['actions']['update_save'] = array('#value' => t('Update and Save')) + $base;

  $form['#submit'][] = 'cat_ui_edit_form_submit';

  return $form;
}

/**
 * Ajax callback for CAT item EDIT form.
 * Redirect user to general CAT item list when item is updated.
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function cat_ui_edit_form_ajax_callback($form, $form_state) {
  if (!form_get_errors()) {
    $values = $form_state['values'];
    if (!empty($values['update_save']) && $values['op'] == $values['update_save']) {
      // Redirect user to page with CAT item list.
      ctools_include('ajax');
      // Ajax responder is not required since CTools 7.x-1.0 but this line
      // should be added to keep module works properly for all versions.
      ctools_add_js('ajax-responder');
      $commands = array();
      $commands[] = ctools_ajax_command_redirect('admin/structure/cat');
      return array(
        '#type' => 'ajax',
        '#commands' => $commands,
      );
    }
  }

  return $form;
}

/**
 * Ajax callback for CAT_item EDIT links.
 */
function cat_ui_edit_form_ajax_links($cat_item, $step) {
  $commands = array();

  $form_state = array(
    'storage' => array(
      'machine_name' => $cat_item->machine_name,
      'step' => $step,
    ),
  );

  $form = drupal_build_form('cat_ui_edit_form', $form_state);

  $commands[] = ajax_command_replace('#cat-ui-form-wrapper', render($form));
  $commands[] = ajax_command_invoke('#cat-ui-menu li a', 'removeClass', array('active'));
  $commands[] = ajax_command_invoke('#cat-ui-menu li a:eq(' . --$step . ')', 'addClass', array('active'));
  $commands[] = ajax_command_prepend('#cat-ui-form-wrapper', theme('status_messages'));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Submit callback for path CAT item EDIT form.
 *
 * @param array $form
 * @param array $form_state
 */
function cat_ui_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if (!empty($values['update']) && $values['op'] == $values['update']) {
    // If user clicks UPDATE button CAT item will be automatically moved to cache.
    // We just need to show user some notifications.
    drupal_set_message(t('The CAT item has been updated.'));
    drupal_set_message(t('Changes will not be permanent until you save it.'), 'warning');
  }
  elseif (!empty($values['update_save']) && $values['op'] == $values['update_save']) {
    $cat_item = cat_object_cache_get($form_state['storage']['machine_name']);

    cat_save($cat_item);
    // We just need to show user some notifications.
    drupal_set_message(t('The CAT item "%name" was updated.', array('%name' => $cat_item->name)));
  }
}

