<?php
/**
 * @file
 * panels-pane.preprocess.inc
 */


/**
 * Implements hook_preprocess_panels_pane().
 */
function wetkit_omega_preprocess_panels_pane(&$variables) {
  if ($variables['pane']->type == 'node_updated') {
    // Date Support for different themes.
    if (variable_get('wetkit_wetboew_theme', 0) == 1) {
      $variables['date_mod'] = 'gcwu-date-mod';
    }
    elseif (variable_get('wetkit_wetboew_theme', 0) == 2) {
      $variables['date_mod'] = 'gcwu-date-mod';
    }
    elseif (variable_get('wetkit_wetboew_theme', 0) == 3) {
      $variables['date_mod'] = 'base-date-mod';
    }
    elseif (variable_get('wetkit_wetboew_theme', 0) == 4) {
      $variables['date_mod'] = 'ogpl-date-mod';
    }
    else {
      $variables['date_mod'] = 'wet-date-mod';
    }
  }

  if (isset($variables['pane']->style['settings']['configure_pane_title_backgrounds'])) {
    // if a title background is set
    $style = $variables['pane']->style['settings']['configure_pane_title_backgrounds'];
    if ($variables['pane']->type == 'menu_tree') {
      if (isset($variables['content']['#content'])) {
        $menu_element= element_children($variables['content']['#content']);
        if (isset($variables['content']['#content'][$menu_element[0]]['#attributes'])) {
          // being extra safe
          $variables['content']['#content'][$menu_element[0]]['#attributes']['class'][] = $style;
        }
      }
    } else {
      $class_stack = explode(" ", $style);
      $variables['title_attributes_array']['class'] = array_merge($variables['title_attributes_array']['class'], $class_stack);
    }

  }
}
