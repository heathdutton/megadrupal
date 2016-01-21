<?php

/**
 * @file
 * Form callbacks for the "custom" display plugin.
 */

/**
 * Submit callback for the 'Add type' button.
 */
function conditional_text_custom_add_button_submit($form, &$form_state) {
  $form_state['custom']['num_types']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX callback for the 'Add type' button.
 */
function conditional_text_custom_add_button_callback($form) {
  module_load_include('inc', 'conditional_text');
  $part = conditional_text_find_subtree($form, 'custom');
  return $part && !empty($part['types']) ? $part['types'] : NULL;
}

/**
 * AJAX callback for the 'Add option' button.
 */
function conditional_text_custom_add_option_callback($form, $form_state) {
  $i = $form_state['triggering_element']['#custom_key'];
  module_load_include('inc', 'conditional_text');
  $part = conditional_text_find_subtree($form, 'custom');
  return $part && !empty($part['types'][$i]) ? $part['types'][$i] : NULL;
}

/**
 * Submit callback for the 'Add option' button.
 */
function conditional_text_custom_add_option_submit($form, &$form_state) {
  $i = $form_state['triggering_element']['#custom_key'];
  $form_state['custom']['num_type'][$i]++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit callback for the 'Remove option' button.
 */
function conditional_text_custom_remove_option_submit($form, &$form_state) {
  $i = $form_state['triggering_element']['#custom_key'];
  if ($form_state['custom']['num_type'][$i] > 0) {
    $form_state['custom']['num_type'][$i]--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit callback for the 'Remove this type' button.
 */
function conditional_text_custom_remove_button_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  if ($form_state['custom']['num_types'] > 0) {
    module_load_include('inc', 'conditional_text');
    $i = $form_state['triggering_element']['#custom_key'];
    $form_state['custom']['num_types']--;
    $ct = &conditional_text_find_subtree($form_state['input'],
                                                         'conditional_text');
    unset($ct['settings']['custom']['types'][$i]);
    $ct['settings']['custom']['types'] =
      array_values($ct['settings']['custom']['types']);
  }
}

/**
 * Element validation callback for the identifier token.
 */
function conditional_text_custom_plugin_identifier_token_validate($element) {
  if (!empty($element['value']) &&
      str_word_count($element['#value'], 0, '_-') != 1) {
    form_error($element, t('The identifier token must be exactly one word.'));
  }
}

/**
 * Helper function for 'exist' callbacks.
 */
function conditional_text_plugin_custom_machine_name_exists($field, $token, $form_state) {
  module_load_include('inc', 'conditional_text');
  return count(array_filter(conditional_text_collect_tree_values(
        $form_state['input'], $field),
      function($item) use($token) {
        return trim($item) !== '' && $item === $token;
      })) > 1;
}

/**
 * Exist callback for the identifier token machine name.
 */
function conditional_text_plugin_custom_identifier_token_exists($token,
                                                                $form,
                                                                $form_state) {
  return conditional_text_plugin_custom_machine_name_exists('identifier_token_mn', $token, $form_state);
}

/**
 * Exist callback for the options machine name.
 */
function conditional_text_plugin_custom_option_name_exists($token, $form, $form_state) {
  return conditional_text_plugin_custom_machine_name_exists('name_mn', $token, $form_state);
}

/**
 * Determines the source of the machine name.
 */
function conditional_text_plugin_custom_machine_name_source_generator($source_name, $element) {
  // preparing the 'source' element of the machine_name
  $source = $element['#array_parents'];
  // removing the last item (which is the current element)
  array_pop($source);
  // and replacing with the human readable name
  $source[] = $source_name;

  $element['#machine_name']['source'] = $source;

  return $element;
}

/**
 * Process callback for the identifier token machine name.
 */
function conditional_text_plugin_custom_identifier_token_mn_process($element) {
  return conditional_text_plugin_custom_machine_name_source_generator('identifier_token', $element);
}

/**
 * Process callback for the option machine name.
 */
function conditional_text_plugin_custom_option_mn_process($element) {
  return conditional_text_plugin_custom_machine_name_source_generator('name', $element);
}
