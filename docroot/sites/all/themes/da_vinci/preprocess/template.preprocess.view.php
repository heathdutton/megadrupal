<?php
/**
 * @file
 * Preproccess functions for Views.
 */

/**
 * Implements da_vinci_preprocess_views_view().
 */
function da_vinci_preprocess_views_view(&$vars) {
  if ($vars['view']->name == theme_get_setting('masonry')) {
    $theme_path = drupal_get_path('theme', 'da_vinci');
    $lib_dv_dir = libraries_get_path('da-vinci-plugins');
    $lib_masonry_dir = libraries_get_path('masonry');
    drupal_add_js($lib_masonry_dir . '/masonry.pkgd.min.js');
    drupal_add_js($lib_dv_dir . '/classie.js');
    drupal_add_js($lib_dv_dir . '/imageload.js');
    if ($vars['view']->editing == FALSE) {
      drupal_add_js($theme_path . '/js/masonry-view.js');
    }
    else {
      drupal_set_message(t('Masonry Desactivated to Edit Mode.'), 'info');
    }
  }
}
