<?php
/**
 * @file
 * Custom functions.
 */

/**
 * Generate additional classes for nodes.
 *
 * @see clean_preprocess_node()
 */
function clean_node_classes(&$variables) {
  $node = $variables['node'];

  // Generate classes for the node container.
  $classes = &$variables['attributes_array']['class'];
  $classes[] = 'node-type-' . $node->type;
  if ($variables['page']) {
    $classes[] = 'node-page' . $node->type;
  }
  elseif ($variables['teaser']) {
    $classes[] = 'node-teaser-' . $node->type;
  }
  if ($variables['sticky']) {
    $classes[] = 'sticky';
  }
  if (!$variables['status']) {
    $classes[] = 'node-unpublished';
  }

  // Generate classes for the content container.
  $variables['content_attributes_array']['class'][] = 'node-content';
  $variables['content_attributes_array']['class'][] = 'clearfix';

  $variables['attributes_array']['id'] = 'node-' . $node->nid;
}

/**
 * Check for stylesheets to be placed at the top of the stack or conditional
 * Internet Explorer styles in the .info file and add them to the $styles
 * variable.
 */
function _clean_process_stylesheets($themes_active) {
  // Prepare the needed variables.
  global $theme_info;
  $framework_styles = array();
  $conditional_styles = array();

  // If there is more than one active theme, check all base themes for
  // stylesheets.
  if (count($themes_active) > 1) {
    global $base_theme_info;
    foreach ($base_theme_info as $name => $info) {
      if (isset($info->info['framework stylesheets'])) {
        $framework_styles[$info->name] = $info->info['framework stylesheets'];
      }
      if (isset($info->info['conditional stylesheets'])) {
        $conditional_styles[$info->name] = $info->info['conditional stylesheets'];
      }
    }
  }

  // Check the current theme for stylesheets.
  if (isset($theme_info->info['framework stylesheets'])) {
    $framework_styles[$theme_info->name] = $theme_info->info['framework stylesheets'];
  }
  if (isset($theme_info->info['conditional stylesheets'])) {
    $conditional_styles[$theme_info->name] = $theme_info->info['conditional stylesheets'];
  }

  // If there is at least one entry in the $framework_styles array, process it.
  if (count($framework_styles) >= 1) {
    // Add all the framework stylesheets to a group so they are loaded first.
    foreach ($framework_styles as $theme => $medias) {
      foreach ($medias as $media => $stylesheets) {
        foreach ($stylesheets as $path) {
          $path = drupal_get_path('theme', $theme) . '/' . $path;
          drupal_add_css($path, array(
            'group' => CSS_SYSTEM,
            'media' => $media,
            'weight' => -1000,
            'every_page' => TRUE,
          ));
        }
      }
    }
  }

  // If there is at least one entry in the $conditional_styles array, process it.
  if (count($conditional_styles) >= 1) {
    // Add all the conditional stylesheets with drupal_add_css().
    foreach ($conditional_styles as $theme => $conditions) {
      foreach ($conditions as $condition => $medias) {
        foreach ($medias as $media => $stylesheets) {
          foreach ($stylesheets as $path) {
            $path = drupal_get_path('theme', $theme) . '/' . $path;
            if ($condition == '!ie') {
              $browsers = array('!IE' => TRUE, 'IE' => FALSE);
            }
            else {
              $browsers = array('!IE' => FALSE, 'IE' => $condition);
            }
            drupal_add_css($path, array(
              'media' => $media,
              'every_page' => TRUE,
              'browsers' => $browsers,
              'group' => CSS_THEME,
              'weight' => 1000,
            ));
          }
        }
      }
    }
  }
}

/**
 * Collect all information for the active theme.
 */
function _clean_theme_collector() {
  $themes = list_themes();
  $themes_active = array();
  global $theme_info;

  // If there is a base theme, collect the names of all themes that may have
  // data files to load.
  if (isset($theme_info->base_theme)) {
    global $base_theme_info;
    foreach ($base_theme_info as $base) {
      $themes_active[$base->name] = $themes[$base->name];
      $themes_active[$base->name]->path = drupal_get_path('theme', $base->name);
    }
  }

  // Add the active theme to the list of themes that may have data files.
  $themes_active[$theme_info->name] = $themes[$theme_info->name];
  $themes_active[$theme_info->name]->path = drupal_get_path('theme', $theme_info->name);

  return $themes_active;
}
