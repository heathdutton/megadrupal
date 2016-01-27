<?php
/**
 * @file
 * Provides the administration page for Image Preloader module.
 */

/**
 * Menu callback for admin/config/image_preloader.
 *
 *   To display the image preloader administration page.
 */
function image_preloader_admin_display($theme = NULL) {
  drupal_add_css(drupal_get_path('module', 'image_preloader') . '/css/image_preloader.css');

  global $theme_key;
  drupal_theme_initialize();

  if (!isset($theme)) {
    // If theme is not specifically set, rehash for the current theme.
    $theme = $theme_key;
  }

  // Fetch blocks.
  $blocks = image_preloader_get_blocks_list();

  // Fetch icons.
  $image_preloader_icons = image_preloader_icon_list();

  return drupal_get_form('image_preloader_admin_display_form', $blocks, $image_preloader_icons);
}

/**
 * Form constructor for the content type, views page and block.
 */
function image_preloader_admin_display_form($form, &$form_state, $blocks, $image_preloader_icons) {
  $form['image_preloader']['nodetypes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Content Types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if (module_exists('views')) {
    $form['image_preloader']['viewspage'] = array(
      '#type' => 'fieldset',
      '#title' => t('Select Views Page'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
  }
  $form['image_preloader']['blocklist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Block'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['image_preloader']['iconslist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Loader Icon'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Get the list of all content type.
  $node_types = node_type_get_types();
  $node_names = node_type_get_names();

  if (is_array($node_names) and count($node_names)) {
    foreach ($node_names as $key => $value) {
      $form['image_preloader']['nodetypes']['image_preloader_posts_' . $node_types[$key]->type . '_enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('@value', array('@value' => $value)),
        '#default_value' => variable_get('image_preloader_posts_' . $node_types[$key]->type . '_enabled', FALSE),
      );
    }
  }

  // Get the list of all views page.
  if (module_exists('views')) {
    $views = views_get_enabled_views();

    foreach ($views as $id => $view) {
      foreach (array_keys($view->display) as $id) {
        if ($id == 'page') {
          $form['image_preloader']['viewspage']['image_preloader_viewspage_' . $view->name . '_enabled'] = array(
            '#type' => 'checkbox',
            '#title' => t('@value', array('@value' => $view->name)),
            '#default_value' => variable_get('image_preloader_viewspage_' . $view->name . '_enabled', FALSE),
          );
        }
      }
    }
  }

  // Get each block info from the block array.
  foreach ($blocks as $i => $block) {
    $form['image_preloader']['blocklist']['image_preloader_blockname_' . $block['delta'] . '_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('@block', array('@block' => $block['info'])),
      '#default_value' => variable_get('image_preloader_blockname_' . $block['delta'] . '_enabled', FALSE),
    );
  }

  // Get loader icon from image preloader icons array.
  $form['image_preloader']['iconslist']['image_preloader_icon'] = array(
    '#type'    => 'radios',
    '#options' => $image_preloader_icons,
    '#default_value' => variable_get('image_preloader_icon', 0),
  );

  // Find selected image loader icons.
  $image_preloader_icons_set = $form['image_preloader']['iconslist']['image_preloader_icon']['#options'];
  foreach (array_keys($image_preloader_icons_set) as $i) {
    if ($i == variable_get('image_preloader_icon')) {
      variable_set('image_preloader_icons_selected', $image_preloader_icons_set[$i]);
    }
  }

  return system_settings_form($form);
}

/**
 * Get the block list array.
 */
function image_preloader_get_blocks_list() {

  $blocks = array();
  // These are {block}.bid values to be kept.
  $bids = array();
  $or = db_or();

  foreach (module_implements('block_info') as $module) {
    $module_blocks = module_invoke($module, 'block_info');
    foreach ($module_blocks as $delta => $block) {
      // Compile a condition to retrieve this block from the database.
      $condition = db_and()
        ->condition('module', $module)
        ->condition('delta', $delta);
      $or->condition($condition);
      // Add identifiers.
      $block['module'] = $module;
      $block['delta']  = $delta;
      $blocks[] = $block;
    }
  }

  return $blocks;

}

/**
 * Get the loader icons array.
 */
function image_preloader_icon_list() {
  $image_preloader_icon_dir = drupal_get_path('module', 'image_preloader') . '/images';
  $images = file_scan_directory($image_preloader_icon_dir, '/.*\.gif$/');
  $image_preloader_icons = array();
  foreach ($images as $file) {
    $image_info = array(
      'path' => base_path() . $file->uri,
      'alt' => 'image preloader icon',
      'title' => 'image preloader icon',
    );
    $image_preloader_icons[] = theme('image', $image_info);
  }

  return $image_preloader_icons;
}
