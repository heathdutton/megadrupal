<?php
/**
 * @file
 * Template definition
 */

/**
 * Override or insert variables into the maintenance page template.
 */
function maintenance_preprocess_maintenance_page(&$vars) {
  // While markup for normal pages is split into page.tpl.php and html.tpl.php,
  // the markup for the maintenance page is all in the single
  // maintenance-page.tpl.php template. So, to have what's done in
  // maintenance_preprocess_html() also happen on the maintenance page, it has
  // to be called here.
  maintenance_preprocess_html($vars);
}

/**
 * Override or insert variables into the html template.
 */
function maintenance_preprocess_html(&$vars) {
  // Add conditional CSS for IE8 and below.
  $options = array(
    'group' => CSS_THEME,
    'browsers' => array(
      'IE' => 'lte IE 8',
      '!IE' => FALSE,
    ),
    'weight' => 999,
    'preprocess' => FALSE,
  );
  drupal_add_css(path_to_theme() . '/assets/styles/ie.css', $options);
  // Add conditional CSS for IE7 and below.
  $options['browsers']['IE'] = 'lte IE 7';
  drupal_add_css(path_to_theme() . '/assets/styles/ie7.css', $options);
  // Add conditional CSS for IE6.
  $options['browsers']['IE'] = 'lte IE 6';
  drupal_add_css(path_to_theme() . '/assets/styles/ie6.css', $options);
}

/**
 * Override or insert variables into the page template.
 */
function maintenance_preprocess_page(&$vars) {
  $vars['primary_local_tasks'] = $vars['tabs'];
  unset($vars['primary_local_tasks']['#secondary']);
  $vars['secondary_local_tasks'] = array(
    '#theme' => 'menu_local_tasks',
    '#secondary' => $vars['tabs']['#secondary'],
  );
}

/**
 * Display the list of available node types for node creation.
 */
function maintenance_node_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<ul class="admin-list">';
    foreach ($content as $item) {
      $output .= '<li class="clearfix">';
      $output .= '<span class="label">' . l($item['title'], $item['href'], $item['localized_options']) . '</span>';
      $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  else {
    $output = '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }
  return $output;
}

/**
 * Overrides theme_admin_block_content().
 *
 * Use unordered list markup in both compact and extended mode.
 */
function maintenance_admin_block_content($variables) {
  $content = $variables['content'];
  $output = '';
  if (!empty($content)) {
    $output = system_admin_compact_mode() ? '<ul class="admin-list compact">' : '<ul class="admin-list">';
    foreach ($content as $item) {
      $output .= '<li class="leaf">';
      $output .= l($item['title'], $item['href'], $item['localized_options']);
      if (isset($item['description']) && !system_admin_compact_mode()) {
        $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      }
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  return $output;
}

/**
 * Override of theme_tablesort_indicator().
 *
 * Use our own image versions, so they show up as black and not gray on gray.
 */
function maintenance_tablesort_indicator($variables) {
  $style = $variables['style'];
  $theme_path = drupal_get_path('theme', 'maintenance');
  if ($style == 'asc') {
    $variables = array(
      'path' => $theme_path . '/assets/images/arrow-asc.png',
      'alt' => t('sort ascending'),
      'width' => 13,
      'height' => 13,
      'title' => t('sort ascending'),
    );
    return theme('image', $variables);
  }
  else {
    $variables = array(
      'path' => $theme_path . '/assets/images/arrow-desc.png',
      'alt' => t('sort descending'),
      'width' => 13,
      'height' => 13,
      'title' => t('sort descending'),
    );
    return theme('image', $variables);
  }
}

/**
 * Implements hook_css_alter().
 */
function maintenance_css_alter(&$css) {
  // Use maintenance's vertical tabs style instead of the default one.
  if (isset($css['misc/vertical-tabs.css'])) {
    $css['misc/vertical-tabs.css']['data'] = drupal_get_path('theme', 'maintenance') . '/assets/styles/vertical-tabs.css';
  }
  if (isset($css['misc/vertical-tabs-rtl.css'])) {
    $css['misc/vertical-tabs-rtl.css']['data'] = drupal_get_path('theme', 'maintenance') . '/assets/styles/vertical-tabs-rtl.css';
  }
  // Use maintenance's jQuery UI theme style instead of the default one.
  if (isset($css['misc/ui/jquery.ui.theme.css'])) {
    $css['misc/ui/jquery.ui.theme.css']['data'] = drupal_get_path('theme', 'maintenance') . '/assets/styles/jquery.ui.theme.css';
  }
}
