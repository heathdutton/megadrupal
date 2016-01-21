<?php

// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('rebuild_registry')) {
  drupal_theme_rebuild();
}

/**
 * Implements hook_theme().
 */
function simpliste_theme() {
  return array(
    'search_element' => array('render element' => 'element'),
  );
}

/**
 * Implements hook_preprocess_html().
 */
function simpliste_preprocess_html(&$vars) {
  if ($head_code = theme_get_setting('head_code')) {
    drupal_add_html_head(array('#type' => 'markup', '#markup' => $head_code), 'simpliste_head_code');
  }
  if (theme_get_setting('wireframes')) {
    $vars['classes_array'][] = 'wireframes';
  }
  if (theme_get_setting('timer')) {
    drupal_register_shutdown_function('simpliste_shutdown');
  }
  // Remove default "no-sidebars" class.
  foreach ($vars['classes_array'] as $delta => $class) {
    if ($class == 'no-sidebars') {
      unset($vars['classes_array'][$delta]);
    }
  }
  $vars['classes_array'][] = &drupal_static('sidebar_class', 'right-sidebar');
}


/**
 * Implements hook_preprocess_page().
 */
function simpliste_preprocess_page(&$vars) {

  // Add default skin.
  $skin = theme_get_setting('active_skin');
  drupal_add_css(
    $vars['directory'] . "/skins/$skin/style.css",
    array('weight' => 999, 'group' => CSS_THEME)
  );

  // Add PIE.
  $pie_path = $vars['base_path'] . $vars['directory'] . '/js/PIE.htc';
  $pie_selectors = '#main-menu li a, a.button, .form-submit, .tabs ul.primary li a';
  drupal_add_css($pie_selectors . '{position: relative; behavior: url(' . $pie_path . ');}',
    array(
      'type' => 'inline',
      'browsers' => array('IE' => 'lte IE 9', '!IE' => FALSE),
      'preprocess' => FALSE,
      'weight' => 1000,
      'group' => CSS_THEME,
    )
  );

  //-- Main menu.
  if (!empty($vars['main_menu'])) {
    $vars['main_menu'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['main_menu'] = FALSE;
  }

  //-- Secondary menu.
  if (!empty($vars['secondary_menu'])) {
    $vars['secondary_menu'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['secondary_menu'] = FALSE;
  }

  //-- Footer message.
  $vars['footer_message'] = theme_get_setting('footer_message');

  //-- Simpliste settings.
  if ($vars['is_admin'] && theme_get_setting('settings_form') && $vars['page']['sidebar']) {
    $vars['page']['sidebar']['settings_form'] = drupal_get_form('simpliste_settings_form');
    $vars['page']['sidebar']['#sorted'] = FALSE;
  }

  //-- Custom featured content.
  if (theme_get_setting('featured')) {
    $vars['page']['featured']['message'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="col_100 block">',
      '#markup' => theme_get_setting('featured'),
      '#suffix' => '</div>',
      '#weight' => -100,
    );
    $vars['page']['featured']['#sorted'] = FALSE;
  }

  //-- Sidebar position.
  $vars['sidebar_position'] = theme_get_setting('sidebar_position');
  // Convert path to lowercase. This allows comparison of the same path
  // with different case. Ex: /Page, /page, /PAGE.
  $pages = drupal_strtolower(theme_get_setting('sidebar_pages'));
  // Convert the Drupal path to lowercase.
  $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
  // Compare the lowercase internal and lowercase path alias (if any).
  $page_match = drupal_match_path($path, $pages);
  if ($path != $_GET['q']) {
    $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
  }
  // When $sidebar_visibility has a value of 0, the sidebar is displayed
  // on all pages except those listed in $pages. When set to 1 , it is
  // displayed only on those pages listed in $pages.
  if (theme_get_setting('sidebar_visibility') xor $page_match) {
    $vars['sidebar_position'] = 'none';
  }

  //-- Content width and body sidebar class.
  $sidebar_class = &drupal_static('sidebar_class');
  if ($vars['sidebar_position'] == 'none' || !$vars['page']['sidebar']) {
    $vars['content_classes'] = 'col_100';
    $sidebar_class = 'no-sidebar';
  }
  else {
    $vars['content_classes'] = 'col_66';
    $sidebar_class = $vars['sidebar_position'] . '-sidebar';
  }

}

/**
 * Implements hook_preprocess_comment().
 */
function simpliste_preprocess_comment(&$vars) {
  $vars['id'] = l(
    '#' . $vars['id'],
    'node/' . $vars['node']->nid,
    array(
      'attributes' => array('class' => array('comment-id')),
      'fragment' => 'comment-' . $vars['comment']->cid,
    )
  );
}

/**
 * Implements hook_preprocess_block().
 */
function simpliste_preprocess_block(&$vars) {
  if ($vars['block']->region == 'featured') {
    $vars['classes_array'][] = 'col_33';
  }
}

/**
 * Implements theme_region().
 */
function simpliste_region($vars) {
  return $vars['content'];
}

/**
 * Implements theme_breadcrumb().
 */
function simpliste_breadcrumb($vars) {
  $breadcrumb_settings = theme_get_setting('breadcrumb');
  if ($breadcrumb_settings['display']) {
    // Optionally get rid of the homepage link.
    if (!$breadcrumb_settings['home']) {
      array_shift($vars['breadcrumb']);
    }
    // Append the content title to the end of the breadcrumb.
    if ($breadcrumb_settings['title']) {
      $vars['breadcrumb'][] = drupal_get_title();
    }
    // Return the breadcrumb with separators.
    if ($vars['breadcrumb']) {
      return implode($breadcrumb_settings['separator'], $vars['breadcrumb']);
    }
  }
  return FALSE;
}

/**
 * Implements theme_block__system__main().
 */
function simpliste_block__system__main($vars) {
  $output = '<div id="system-main" class="' . $vars['classes'] . '" ' . $vars['attributes'] . '>';
  $output .= render($title_prefix);
  if ($vars['block']->subject) {
    $output .= '<h2 ' . $vars['title_attributes'] . '>' . $vars['block']->subject . '</h2>';
  }
  $output .= render($vars['title_suffix']);
  $output .= $vars['content'];
  $output .= '</div>';
  return $output;
}

/**
 * Implements hook_form_BASE_FORM_ID_alter().
 */
function simpliste_form_node_form_alter(&$form, $form_state) {
  if (theme_get_setting('hide_format_node_form')) {
    $form['#after_build'][] = 'simpliste_node_hide_format_selection';
  }
}

/**
 * Node form after_build callback.
 */
function simpliste_node_hide_format_selection($form) {
  $language = $form['body']['#language'];
  $form['body'][$language]['0']['format']['#access'] = FALSE;
  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function simpliste_form_comment_form_alter(&$form, $form_state) {
  if (theme_get_setting('hide_format_comment_form')) {
    $form['#after_build'][] = 'simpliste_comment_hide_format_selection';
  }
}

/**
 * Comment form after_build callback.
 */
function simpliste_comment_hide_format_selection($form) {
  $language = $form['comment_body']['#language'];
  $form['comment_body'][$language]['0']['format']['#access'] = FALSE;
  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function simpliste_form_search_block_form_alter(&$form, $form_state) {
  $form['search_block_form']['#theme'] = 'search_element';
  $form['search_block_form']['#attributes']['type'] = 'search';
  $form['search_block_form']['#attributes']['placeholder'] = t('Search this site');
  $form['search_block_form']['#size'] = 25;
}

/**
 * Implements theme_search_element().
 */
function simpliste_search_element($vars) {
  $element = $vars['element'];
  element_set_attributes($element, array('id', 'name', 'value', 'size', 'maxlength'));
  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';
  return $output;
}

/**
 * Implements theme_field__field_type().
 */
function simpliste_field__text_with_summary($vars) {

  $output = '';

  // Render the label, if it's not hidden.
  if (!$vars['label_hidden']) {
    $output .= '<div class="field-label"' . $vars['title_attributes'] . '>' . $vars['label'] . ':&nbsp;</div>';
  }

  // Render the items.
  foreach ($vars['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    $output .= '<div class="body ' . $classes . '"' . $vars['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
  }
  return $output;
}

/**
 * Simpliste settings form.
 */
function simpliste_settings_form($form, &$form_state) {
  $form = array(
    '#prefix' => '<div class="block"><h2>' . t('Simpliste settings') . '</h2>',
    '#suffix' => '</div>',
    'active_skin' => array(
      '#type' => 'select',
      '#title' => t('Active skin:'),
      '#options' => drupal_map_assoc(simpliste_get_skins()),
      '#default_value' => theme_get_setting('active_skin'),
    ),
    'sidebar_position' => array(
      '#type' => 'select',
      '#title' => t('Sidebar position:'),
      '#options' => drupal_map_assoc(array('left', 'right')),
      '#default_value' => theme_get_setting('sidebar_position'),
    ),
    'wireframes' => array(
      '#type' => 'checkbox',
      '#title' => t('Wireframe mode'),
      '#default_value' => theme_get_setting('wireframes'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Send'),
    ),
  );
  return $form;
}

/**
 * Simpliste settings form submit.
 */
function simpliste_settings_form_submit($form, &$form_state) {
  $settings = variable_get('theme_simpliste_settings', array());
  $settings['active_skin'] = $form_state['values']['active_skin'];
  $settings['sidebar_position'] = $form_state['values']['sidebar_position'];
  $settings['wireframes'] = $form_state['values']['wireframes'];
  variable_set('theme_simpliste_settings', $settings);
}

/**
 * Get all skins.
 *
 *  @TODO Make it cacheable.
 */
function simpliste_get_skins() {
  foreach (scandir(drupal_get_path('theme', 'simpliste') . '/skins') as $directory) {
    if ($directory != '.' && $directory != '..') {
      $skins[] = $directory;
    }
  }
  return $skins;
}

/**
 * Shutdown callback.
 */
function simpliste_shutdown() {
  echo '<div id="timer">' . t('Page execution time was <b>!time</b> ms' , array('!time' => round(timer_read('page')))) . '</div>';
}