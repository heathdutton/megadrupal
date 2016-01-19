<?php

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Act on Linked Field settings
 *
 * This hook is invoked from linked_field_field_attach_view_alter() when linking
 * a field. It allows you to override all settings.
 *
 * @param $settings
 *   An associative array of Linked Field settings.
 * @param $context
 *   An associative array containing:
 *   - entity_type: The type of $entity; for example, 'node' or 'user'.
 *   - entity: The entity with fields to render.
 *   - view_mode: View mode; for example, 'full' or 'teaser'.
 *   - display: Either a view mode string or an array of display settings. If
 *     this hook is being invoked from field_attach_view(), the 'display'
 *     element is set to the view mode string. If this hook is being invoked
 *     from field_view_field(), this element is set to the $display argument
 *     and the view_mode element is set to '_custom'. See field_view_field()
 *     for more information on what its $display argument contains.
 *   - language: The language code used for rendering.
 */
function hook_linked_field_settings_alter(&$settings, $context) {
  $entity_type = $context['entity_type'];
  $entity = $context['entity'];
  list(, , $bundle) = entity_extract_ids($entity_type, $entity);
  $view_mode = $context['view_mode'];

  // Add custom attribute for the link.
  if ($entity_type == 'node' && $bundle == 'article' && $view_mode == 'default') {
    $settings['options']['attributes']['data-id'] = $entity->nid;
  }
}
