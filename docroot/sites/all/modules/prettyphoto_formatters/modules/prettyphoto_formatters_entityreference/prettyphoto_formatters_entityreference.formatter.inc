<?php

/**
 * @file
 * prettyPhoto Entity reference formatter hooks and callbacks.
 */

/**
 * Theme function for displaying the prettyPhoto Entity reference field.
 *
 * @param array $variables
 *   An array of Entity reference field's attributes, e.g. entity_type.
 *
 * @return string
 *   HTML output for displaying the referenced node.
 */
function theme_prettyphoto_formatters_entityreference($variables) {
  $entity = $variables['entity'];
  $entity_id = $variables['entity_id'];
  $entity_type = $variables['entity_type'];
  $view_mode = $variables['view_mode'];
  $field = $variables['field'];
  $instance = $variables['instance'];

  $handler = entityreference_get_selection_handler($field, $instance, $entity_type, $entity);
  $label = $handler->getLabel($entity);

  $options = array(
    'attributes' => array(
      'rel' => array('prettyPhoto[' . $field['field_name'] . ']'),
    ),
    'query' => array(
      'ajax' => 'true',
    ),
  );

  $url = 'prettyphoto/ajax/' . $entity_type . '/' . $entity_id . '/' . $view_mode;
  $output = l($label, $url, $options);

  return $output;
}
