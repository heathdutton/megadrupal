<?php

/**
 * Implements hook_token_info().
 */
function webform_document_token_info() {
  $info = array();

  $info['tokens']['submission']['options'] = array(
    'name' => t('Select options in readable way'),
    'description' => t('Webform tokens from submitted data. Replace the "?" with the "field key" of "select" component. You may append ":radio" for render as radio buttons or ":inline" to show options in the same line.'),
    'dynamic' => TRUE,
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function webform_document_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  // Webform tokens (caching globally).
  if ($type == 'submission' && !empty($data['webform-submission'])) {
    // Prepare all the submission data that we will likely need more than once.
    $submission = $data['webform-submission'];
    $node = isset($data['node']) ? $data['node'] : node_load($submission->nid);
    $email = isset($data['webform-email']) ? $data['webform-email'] : NULL;
    $format = $sanitize ? 'html' : 'text';
    $submission_renderable = webform_submission_render($node, $submission, $email, $format);

    // Replace individual tokens that have an exact replacement.
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'options':
          $replacements[$original] = drupal_render($submission_renderable);
          break;
      }
    }

    // Webform submission tokens for individual components.
    if ($value_tokens = token_find_with_prefix($tokens, 'options')) {
      foreach ($value_tokens as $name => $original) {
        foreach ($node->webform['components'] as $component) {
          // Build the list of parents for this component.
          $parents = ($component['pid'] == 0) ? array($component['form_key']) : webform_component_parent_keys($node, $component);
          $parent_token = implode(':', $parents);
          if (strpos($name, $parent_token) === 0) {
            // An exact match.
            if (strcmp($name, $parent_token) === 0) {
              $match = TRUE;
              $modifier = NULL;
            }
            // Check if this matches the key plus a modifier.
            else {
              $modifier = substr($name, strrpos($name, ':') + 1);
              // TODO: Allow components to provide additional modifiers per
              // type, i.e. key, day, hour, minute, etc.
              $available_modifiers = array(
                'radio', 'inline',
              );
              if (strcmp($name, $parent_token . ':' . $modifier) === 0 && in_array($modifier, $available_modifiers)) {
                $match = TRUE;
              }
              else {
                $match = FALSE;
              }
            }

            if ($match) {
              // Drill down into the renderable to find the element.
              $display_element = $submission_renderable;
              foreach ($parents as $parent) {
                $display_element = $display_element[$parent];
              }
              // Individual tokens always have access granted even if they're
              // not displayed when printing the whole renderable.
              $display_element['#access'] = TRUE;
              $display_element['#theme_wrappers'] = array();


              // dpm($display_element, '$display_element');
              // dpm($component, '$component');

              // Selects use "items".
              if (isset($component['extra']['items']) && is_string($component['extra']['items'])) {
                $options = _webform_select_options_from_text($component['extra']['items']);
                $token_value = webform_document_convert_select($options, $display_element['#value'], $modifier);

                $replacements[$original] = $token_value;
              }

              // // Grids use "options".
              // if (isset($component['extra']['options']) && is_string($component['extra']['options'])) {
              //   $options = _webform_select_options_from_text($component['extra']['options']);
              //   $checkboxes = webform_document_convert_to_checkboxes($options);

              //   dpm($options, 'grid $options');
              // }

              break;
            }
          }
        }
      }
    }
  }

  return $replacements;
}

function webform_document_convert_select(array $options, array $default_value = array(), $modifier = NULL) {
  $output = array();
  $no = 1;
  foreach ($options as $key => $option) {
    if (in_array($key, $default_value)) {
      $char = $modifier == 'radio' ? '●' : '☒'; // ☑
    }
    else {
      $char = $modifier == 'radio' ? '○' : '☐';
    }
    $value = $char . '  ' . $option;
    // Replace regular spaces with non-braking spaces (nbsp).
    $output[] = str_replace(' ', '&#xA0;', $value);
    $no++;
  }
  $output = implode(($modifier == 'inline' ? '  ' : '<w:br/>'), $output);

  return $output;
}
