<?php
/**
 * @file
 * Theme functions and preprocess function for trello_api.
 */

/**
 * Preprocess function for trello_api_label.
 */
function template_preprocess_trello_api_label(&$vars) {
  $label = $vars['elements']['#trello_api_label'];
  $vars['name'] = check_plain($label->name);
  $vars['color_class'] = 'trello-color-' . check_plain($label->color);
}
