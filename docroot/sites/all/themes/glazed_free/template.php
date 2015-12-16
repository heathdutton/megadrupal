<?php

/**
 * @file template.php
 */

/**
 * @code makes sure sylesheet is never loaded via @import. @import loading prevents respondjs from doing it's job.
 * This aides in testing your mediaqueries in IE during development, when CSS aggregation is turned off.
 */

$files_path = variable_get('file_public_path', conf_path() . '/files');

if (is_file($files_path . '/glazed-themesettings-glazed_free.css')) {
  drupal_add_css(
    $files_path . '/glazed-themesettings-glazed_free.css', array(
      'preprocess' => variable_get('preprocess_css', '') == 1 ? TRUE : FALSE,
      'group' => CSS_THEME,
      'media' => 'all',
      'every_page' => TRUE,
      'weight' => 100
    )
  );
}

/**
 * Load logic from theme features
 */
foreach (file_scan_directory(drupal_get_path('theme', 'glazed_free') . '/features', '/controller.inc/i') as $file) {
  require_once($file->uri);
}

/**
 * Implements template_preprocess_comment().
 * Using comment patch https://www.drupal.org/files/bootstrap-use-media-object-as-comments-2109369-6.patch
 */
function glazed_free_preprocess_comment(&$vars) {
  $comment = $vars['elements']['#comment'];
  $node = $vars['elements']['#node'];
  if ($vars['elements']['#comment']->depth > 0) {
    $vars['author'] .= ' ' . t('replied') . ':';
  }
  else {
    $vars['author'] .= ' ' . t('said') . ':';
  }
  $vars['classes_array'][] = 'media';
  $vars['title_attributes_array']['class'][] = 'media-heading';
  $vars['time_ago'] = format_interval((time() - $comment->changed) , 2) . t(' ago');
}

/**
 * Implements template_preprocess_node()
 */
function glazed_free_preprocess_node(&$vars) {
  if ($vars['type'] == 'news') {
    $vars['submitted'] = t('Posted on !datetime', array( '!datetime' => format_date($vars['node']->created, 'custom', 'l, F j, Y')));
  }
  if ($vars['teaser']) {
    $vars['display_submitted'] = FALSE;
  }
  if ($vars['type'] == 'event') {
    if (isset($vars['field_event_registration']) && ($vars['field_event_registration']['und'][0]['registration_type']) && function_exists('registration_event_count')) {
      // @see registration.module ~ #646
      $entity_id = $vars['nid'];
      $entity_type = 'node';
      $settings = registration_entity_settings($entity_type, $entity_id);
      $count = registration_event_count($entity_type, $entity_id);
      $capacity = $settings['capacity'];
      if ($capacity == 0) {
        $capacity = t('No limit set');
        $available = t('No limit set');
      }
      else {
        $available = $settings['capacity'] - $count;
      }
        $vars['registration_event_count'] = '<h3 class="field-label">' . t('Spaces') . '</h3><div class="field-event-spaces">' . $capacity . '</div>';
        $vars['registration_event_capacity'] = '<h3 class="field-label">' . t('Available spaces') . '</h3><div class="field-event-spaces-available">' . $available . '</div>';
    }
  }
}

/**
 * Implements template_preprocess_html().
 */
function glazed_free_preprocess_html(&$vars) {
  global $theme;

  if (theme_get_setting('sticky_footer')) {
    $vars['html_attributes_array']['class'] = 'html--glazed-sticky-footer';
  }

  if (module_exists('admin_menu_toolbar') && user_access('access administration menu')) {
    $vars['classes_array'][] = 'admin-menu-toolbar';
  }

  if (theme_get_setting('header_position')) {
    // Side Header
    $vars['classes_array'][] = 'body--glazed-header-side';
    if (theme_get_setting('header_side_overlay')) {
      $vars['classes_array'][] = 'body--glazed-header-side-overlay';
    } else {
      $vars['classes_array'][] = 'body--glazed-header-not-side-overlay';
    }
  }
  else {
    // Top Header
    if (theme_get_setting('header_top_overlay')) {
      $vars['classes_array'][] = 'body--glazed-header-overlay';
    } else {
      $vars['classes_array'][] = 'body--glazed-header-not-overlay';
    }

    if (theme_get_setting('header_top_behavior') == 'fixed') {
      $vars['classes_array'][] = 'body--glazed-header-fixed';
    }
  }
}


/**
 * Implements template_preprocess_page().
 */
function glazed_free_preprocess_page(&$vars) {
  global $theme;

  if (theme_get_setting('page_title_home_hide') && $vars['is_front']) {
    $vars['title'] = '';
  }

  if ($vars['is_front'] && theme_get_setting('stlink')) {
    $vars['sooperthemes_attribution_link'] = glazed_free_attribution_link();
  } else {
    $vars['sooperthemes_attribution_link'] = '';
  }


  $vars['content_row_class'] = ' class="row"';
  $vars['content_container_class'] = ' class="container main-container"';
  if ((isset($vars['node'])
    && ($vars['node']->type == 'drag_drop_page'))
    && (arg(2) != 'edit')
    && empty($vars['page']['sidebar_first'])
    && empty($vars['page']['sidebar_second'])) {
    $vars['content_column_class'] = '';
    $vars['content_row_class'] = '';
    $vars['content_container_class'] = ' class="main-container"';
  }

  // Glazed Header
  $vars['glazed_header_classes'] = array('navbar', 'glazed-header');
  if (theme_get_setting('header_position')) {
    // Side Header
      $vars['glazed_header_classes'][] = 'glazed-header--side';
      $vars['glazed_header_classes'][] = theme_get_setting('header_side_align');
      if (theme_get_setting('header_side_overlay')) {
        $vars['glazed_header_classes'][] = 'glazed-header--overlay';
      }

  }
  else {
    // Top Header
    $vars['glazed_header_classes'][] = 'glazed-header--top';

    if (theme_get_setting('header_top_overlay')) {
      $vars['glazed_header_classes'][] = 'glazed-header--overlay';
    }

    if (theme_get_setting('header_top_layout')) {
      $header_top_layout_class = drupal_html_class('glazed-header--' . theme_get_setting('header_top_layout'));
      $vars['glazed_header_classes'][] = $header_top_layout_class;
    }

    if (theme_get_setting('header_top_behavior') == 'fixed') {
      $vars['glazed_header_classes'][] = 'glazed-header--fixed';
    }

    if (theme_get_setting('header_top_behavior') == 'sticky') {
      $vars['glazed_header_classes'][] = 'glazed-header--sticky';
      $scroll = theme_get_setting('header_top_height_sticky_offset');
      $vars['header_affix'] = ' data-spy="affix" data-offset-top="' . $scroll . '"';
    }

    if (theme_get_setting('header_top_layout')) {
      $vars['glazed_header_classes'][] = drupal_html_class('glazed-header--' . theme_get_setting('header_top_layout'));
    }
  }
  $vars['glazed_header_classes'] = implode(' ', $vars['glazed_header_classes']);

  // Glazed Secondary Header
  $vars['glazed_secondary_header_classes'] = array('glazed-secondary-header');
  switch (theme_get_setting('secondary_header_hide')) {
    case 'hidden_xs':
      $vars['glazed_secondary_header_classes'][] = 'hidden-xs';
      break;
    case 'hidden_sm':
     $vars['glazed_secondary_header_classes'] += array('hidden-xs', 'hidden-sm');
      break;
    case 'hidden_md':
      $vars['glazed_secondary_header_classes'] += array('hidden-xs', 'hidden-sm', 'hidden-md');
      break;

    default:
      $vars['glazed_secondary_header_classes'][] = 'hidden-xs';
      break;
  }
  if (theme_get_setting('secondary_header_hide')) {
    $vars['glazed_secondary_header_classes'][] = drupal_html_class(theme_get_setting('secondary_header_hide'));
  }
  else {
    $vars['glazed_secondary_header_classes'][] = 'hidden-xs';
  }
  $vars['glazed_secondary_header_classes'] = implode(' ', $vars['glazed_secondary_header_classes']);

  // Page Title
  $vars['glazed_title_classes'] = array('page-title');
  if (theme_get_setting('page_title_animate')) {
    $vars['glazed_title_classes'][] = 'wow';
    $vars['glazed_title_classes'][] = theme_get_setting('page_title_animate');
  }
  $vars['glazed_title_classes'] = implode(' ', $vars['glazed_title_classes']);
}

/**
 * Override or insert variables into the page template for HTML output.
 */
function glazed_free_process_html(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
  if (!isset($vars['cond_scripts_bottom'])) $vars['cond_scripts_bottom'] = "";
  if (drupal_is_front_page()) {
    $vars['page_bottom'] .= '<div style="display:none">sfy39587stp16</div>';
  }
}

/**
 * Override or insert variables into the page template.
 */
function glazed_free_process_page(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}


/**
 * Implements hook_js_alter().
 */
function glazed_free_js_alter(&$js) {
  $theme_path = drupal_get_path('theme', 'glazed_free') . '/';
  // Add Bootstrap settings.
  $js['settings']['data'][]['glazed'] = array(
    'glazedPath' => $theme_path,
  );
}

 /**
 * Optional Footer Link to SooperThemes
 */
function glazed_free_attribution_link() {
  $key = ord($_SERVER["SERVER_NAME"])%10;
  $links = array(
  '<a href="http://www.sooperthemes.com/">Drupal Themes by SooperThemes</a>',
  '<a href="http://www.sooperthemes.com">Drupal templates</a> by SooperThemes',
  '<a href="http://www.sooperthemes.com/">Drupal Premium Themes</a>',
  'Bootstrap <a href="http://www.sooperthemes.com/">Premium Drupal theme</a> by SooperThemes',
  '<a href="http://www.sooperthemes.com">Drupal templates</a> by SooperThemes',
  'SooperThemes <a href="http://www.sooperthemes.com">Premium Drupal themes</a>',
  'Premium <a href="http://www.sooperthemes.com">Drupal themes</a> by SooperThemes.com',
  '<a href="http://www.sooperthemes.com/">Drupal theme</a> by SooperThemes',
  '<a href="http://www.sooperthemes.com">Drupal Premium Themes</a> by SooperThemes',
  '<a href="http://www.sooperthemes.com/">Premium Drupal themes</a>',
  );
  return '<p class="clear-both sooperthemes-attribution-link glazed-util-text-muted text-right">' . $links[$key] . '</p>';
}

