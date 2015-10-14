<?php

/**
 * @file Helpers for CAT UI module.
 *
 * @author Oleksiy Kalinichenko <aexchecker@gmail.com>
 */


/**
 * Add default values to form if presented.
 *
 * @param array $form
 *          Form to which values will be applied from cache.
 * @param string $machine_name
 *          Machine name of CAT item.
 */
function _cat_ui_form_apply_default_values(&$form, $machine_name, $skipped_elements = array()) {
  if ($cat_item = cat_object_cache_get($machine_name)) {
    // Try to apply values from cache to form.
    foreach ($form as $key => $form_value) {
      if (!empty($cat_item->{$key}) && !in_array($key, $skipped_elements)) {
        if (is_array($cat_item->{$key})) {
          foreach ($cat_item->{$key} as $array_key => $value) {
            $form[$key][$array_key]['#default_value'] = $value;
          }
        }
        else {
          $form[$key]['#default_value'] = $cat_item->{$key};
        }
      }
    }
  }
  
}

/**
 * Attach ajax-processed buttons to the form.
 *
 * @param array $form
 *          Form which will be added to the button.
 * @param array $buttons
 *          Array with buttons that should be added ('prev', 'next', 'finish').
 * @param string $form_name
 *          Name of form that addes buttons.
 *
 * @see cat_ui_add_form_ajax_callback()
 * @see cat_ui_add_form_submit()
 * @see cat_ui_edit_form_ajax_callback()
 * @see cat_ui_edit_form_submit()
 */
function _cat_ui_form_attach_buttons(&$form, $buttons = array(), $form_name) {
  if ($form_name == 'cat_ui_add_form' || $form_name == 'cat_ui_edit_form') {
    $form_wrapper = 'cat-ui-form';
  }
  else {
    $form_wrapper = str_replace('_', '-', $form_name);
  }

  // @see cat_ui_add_form_ajax_callback() and cat_ui_edit_form_ajax_callback()
  $base = array(
    '#type' => 'submit',
    '#ajax' => array(
      'callback' => "{$form_name}_ajax_callback",
      'wrapper' => "{$form_wrapper}-wrapper",
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  // @see cat_ui_add_form_submit() and cat_ui_edit_form_submit()
  if (in_array('prev', $buttons)) {
    $form['actions']['prev'] = array(
      '#value' => t('Back'),
      // Do not validate form when BACK button clicked.
      '#limit_validation_errors' => array(),
      '#submit' => array("{$form_name}_submit"),
    ) + $base;
  }

  if (in_array('next', $buttons)) {
    $form['actions']['next'] = array('#value' => t('Continue')) + $base;
  }

  if (in_array('finish', $buttons)) {
    $form['actions']['finish'] = array('#value' => t('Finish')) + $base;
    if ($form_name == 'cat_ui_add_form') {
      module_load_include('inc', 'cat_ui', 'cat_ui.add_form');
      $form['actions']['finish']['#ajax']['callback'] = '_cat_ui_add_form_finish_ajax_callback';
    }
  }
}

