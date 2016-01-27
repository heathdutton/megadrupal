<?php

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function personal_site_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $output .= '<div class="breadcrumb">' . implode(' ', $breadcrumb) . '</div>';
    return $output;
  }
}

/**
 * Add body classes if certain regions have content.
 */
function personal_site_preprocess_html(&$variables) {
  if (!empty($variables['page']['featured'])) {
    $variables['classes_array'][] = 'featured';
  }
  if (!empty($variables['page']['triptych_first'])
          || !empty($variables['page']['triptych_middle'])
          || !empty($variables['page']['triptych_last'])) {
    $variables['classes_array'][] = 'triptych';
  }
  if (!empty($variables['page']['footer_firstcolumn'])
          || !empty($variables['page']['footer_secondcolumn'])
          || !empty($variables['page']['footer_thirdcolumn'])
          || !empty($variables['page']['footer_fourthcolumn'])) {
    $variables['classes_array'][] = 'footer-columns';
  }
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['classes_array'][] = 'sidebar-both';
  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['classes_array'][] = 'sidebar-first';
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['classes_array'][] = 'sidebar-second';
  }

  // Add conditional stylesheets for IE
  drupal_add_css(path_to_theme() . '/css/ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/css/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Override or insert variables into the page template.
 */
function personal_site_process_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name'] = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $variables['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $variables['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function personal_site_preprocess_maintenance_page(&$variables) {
  if (!$variables['db_is_active']) {
    unset($variables['site_name']);
  }
  drupal_add_css(drupal_get_path('theme', 'personal_site') . '/css/maintenance-page.css');
}

/**
 * Override or insert variables into the maintenance page template.
 */
function personal_site_process_maintenance_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name'] = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
}

/**
 * Override or insert variables into the node template.
 */
function personal_site_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
  $variables['submitted_year'] = date('Y', $variables['created']);
  $variables['submitted_month'] = date('M', $variables['created']);
  $variables['submitted_day'] = date('d', $variables['created']);
  $variables['tab_left'] = base_path() . path_to_theme() . '/img/tab-left.png';
}

/**
 * Override or insert variables into the page template.
 */
function personal_site_preprocess_page(&$vars) {
  if (isset($vars['main_menu'])) {
    $vars['primary_nav'] = theme('links__system_main_menu', array(
        'links' => $vars['main_menu'],
        'attributes' => array(
            'class' => array('links', 'inline', 'main-menu'),
        ),
        'heading' => array(
            'text' => t('Main menu 2'),
            'level' => 'h2',
            'class' => array('element-invisible'),
        )
            ));
  } else {
    $vars['primary_nav'] = FALSE;
  }
}

/**
 * Override or insert variables into the block template.
 */
function personal_site_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
}

/**
 * Implements theme_menu_tree().
 */
function personal_site_menu_tree($variables) {
  return '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_field__field_type().
 */
function personal_site_field__taxonomy_term_reference($variables) {
  $output = '';
  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h3 class="field-label">' . $variables['label'] . ': </h3>';
  }
  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';
  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';
  return $output;
}
