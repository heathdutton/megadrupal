<?php

/**
 * @file
 * Form callbacks for the "enabled" condition plugin.
 */

/**
 * Form validation callback.
 */
function conditional_text_enabled_module_name_validate($element) {
  if (!empty($element['#value'])) {
    $validating_regexes = array(
      '#^[a-z_]+-[\d]\.([\d]+|x)-[\d]+.([\d]+|x)$#i',
      '#^[a-z_]+-[\d]\.([\d]+|x)-[\d]+.([\d]+|x)-(alpha|beta|rc)[\d]+$#i',
    );

    foreach ($validating_regexes as $regex) {
      if (preg_match($regex, $element['#value'])) {
        return;
      }
    }
    form_error($element, t('Invalid module name or version.'));
  }
}

/**
 * Submit handler for the remove button.
 */
function conditional_text_enabled_remove_button_submit($form, &$form_state) {
  if ($form_state['enabled']['num_modules'] > 1) {
    $form_state['enabled']['num_modules']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the add button.
 */
function conditional_text_enabled_add_button_submit($form, &$form_state) {
  $form_state['enabled']['num_modules']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX callback for the add button.
 */
function conditional_text_enabled_add_button_callback($form) {
  module_load_include('inc', 'conditional_text');
  $part = conditional_text_find_subtree($form, 'enabled');
  return $part && !empty($part['modules']) ? $part['modules'] : NULL;
}
