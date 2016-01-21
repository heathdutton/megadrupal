<?php

/**
 * Menu callback: display all the view modes for the given entity.
 *
 * @param  $entity      Entity to render
 * @param  $entity_type Entity type
 * @return A page array suitable for use by drupal_render().
 */
function view_modes_display_preview_entity($entity, $entity_type) {
  // Get available view modes for this entity type bundle
  $entity_info = entity_get_info($entity_type);
  $view_modes = $entity_info['view modes'];

  // Fetch the bundle name, if no bundle key is set we use the entity_type (e.g. for user)
  if ($entity_info['entity keys']['bundle']) {
    $bundle = $entity->{$entity_info['entity keys']['bundle']};
  }
  else {
    $bundle = $entity_type;
  }

  $view_mode_settings = field_view_mode_settings($entity_type, $bundle);

  // Loop through the view modes
  $build = array();
  foreach ($view_modes as $view_mode_name => $view_mode_info) {
    // Only consider the view mode if we have custom settings configured
    if (!empty($view_mode_settings[$view_mode_name]['custom_settings'])) {
      $build_add = entity_view($entity_type, array($entity), $view_mode_name);
      $build_add['#prefix'] = '<div class="view-mode-list-item view-mode-list-item-' . $view_mode_name . '"><h1>' . $view_mode_info['label'] . '</h1>';
      $build_add['#suffix'] = '</div>';
      // Add it to build mode array
      $build[] = $build_add;
    }
  }

  // We provide a warning if no view modes with custom settings can be found.
  if (module_exists('field_ui') && empty($build)) {
    $admin_path = _field_ui_bundle_admin_path($entity_type, $bundle);
    return array(
      '#prefix' => '<div class="messages warning">',
      '#markup' => t('No custom view modes can be found for this entity. Only view modes that have custom display settings are shown in the preview. !manage', array('!manage' => l(t('Manage display settings.'), $admin_path . '/display'))),
      '#suffix' => '</div>',
    );
  }

  return $build;
}
