<?php

/**
 * @file
 * The theme file, which controls the output of the Email Entity.
 */

/**
 * Process variables for entity_email.tpl.php.
 */
function template_preprocess_entity_email(&$variables) {
  // @todo: Create entity email theme.
  /*$message = $variables['elements']['#entity'];

  $variables['view_mode'] = $variables['elements']['#view_mode'];
  $variables['message'] = $message;

  // Helpful $content variable for templates.
  $variables['content'] = array();
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  // Make the field variables available with the appropriate language.
  field_attach_preprocess('message', $message, $variables['content'], $variables);
  list(, , $bundle) = entity_extract_ids('message', $message);

  // Gather CSS classes.
  $variables['classes_array'][] = drupal_html_class('entity-message');
  $variables['classes_array'][] = drupal_html_class('message-' . $bundle);

  // Add suggestions.
  $variables['theme_hook_suggestions'][] = 'message';
  $variables['theme_hook_suggestions'][] = 'message__' . $bundle;
  $variables['theme_hook_suggestions'][] = 'message__' . $bundle . '__' . $variables['view_mode'];

  if ($id = entity_id('message', $message)) {
    $variables['theme_hook_suggestions'][] = 'message__' . $id;
  }*/
}
