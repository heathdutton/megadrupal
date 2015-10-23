<?php
/**
 * @file
 * Declare Views integration.
 */

/**
 * Implements hook_views_plugins().
 */
function g2migrate_views_plugins() {
  $plugins = array(
    'pager' => array(
      'g2migrate_mini' => array(
        'title' => t('Paged output, custom mini pager'),
        'short title' => t('Mini'),
        'help' => t('Use the mini pager output.'),
        'handler' => 'G2MigrateViewsPagerPlugin',
        'help topic' => '',
        'uses options' => TRUE,
        'parent' => 'mini',
      ),
    ),
  );

  return $plugins;
}

/**
 * Settings for the photo gallery feature.
 */
function g2migrate_settings($key = NULL, $view = NULL) {
  $settings = array(
    'content_type' => 'gallery_photo',
    'base_path' => 'pictures/gallery',
    'album_path' => 'pictures/album',
    'suffix' => 'list#content',
    'items_per_page' => 16,
  );

  if ($view) {
    $display = $view->display[$view->current_display];
    $default_display = $view->display['default'];
    $display_options = $display->display_options + $default_display->display_options;
    $settings['content_type'] = current($display_options['filters']['type']['value']);
    $settings['album_path'] = preg_replace('#/%.*#', '', $display_options['path']);
    $settings['items_per_page'] = $display_options['pager']['options']['items_per_page'];
    $settings['album'] = current($view->args);
  }
  else {
    $settings['album'] = arg(2);
  }

  return $key ? $settings[$key] : $settings;
}

/**
 * Implements hook_views_pre_render().
 *
 * Modify the breadcrumb on the photo gallery view. Add CSS.
 */
function g2migrate_views_pre_render(&$view) {
  $album_view = variable_get('g2migrate_album_view', 'g2migrate_albums');
  if ($view->name == $album_view && $view->display[$view->current_display]->display_plugin == 'page') {
    $g2migrate = drupal_get_path('module', 'g2migrate');
    drupal_add_css("$g2migrate/g2migrate.css");
    $display = $view->display[$view->current_display];
    $default_display = $view->display['default'];
    $display_options = $display->display_options + $default_display->display_options;
    $album_path = g2migrate_settings('album_path', $view);

    $trail = drupal_get_breadcrumb();
    // Keep the Home link.
    $mytrail = array($trail[0]);

    $tid = g2migrate_settings('album', $view);
    // Add parent terms to the breadcrumb.
    $parents = taxonomy_get_parents_all($tid);
    $term = array_shift($parents);
    while ($parent = array_pop($parents)) {
      $mytrail[] = l($parent->name, "$album_path/{$parent->tid}/list", array(
        'fragment' => 'content',
      ));
    }
    // Add a select list for child terms to the breadcrumb.
    $children = taxonomy_get_tree($term->vid, $tid, 1, FALSE);
    if ($children) {
      $form = drupal_get_form('g2migrate_breadcrumb_term_form',
        $term, $view);
      $mytrail[] = drupal_render($form);
    }
    else {
      $mytrail[] = l($term->name, "$album_path/{$term->tid}/list", array(
        'fragment' => 'content',
      ));
    }

    drupal_set_breadcrumb($mytrail);
  }
}

function g2migrate_breadcrumb_term_form($form, &$form_state, $term, $view) {
  $tid = $term->tid;
  $settings = g2migrate_settings(NULL, $view);
  $prefix = '/' . $settings['album_path'];
  $suffix = $settings['suffix'];
  $default = "$prefix/$tid/$suffix";
  $options = array();
  foreach (taxonomy_get_tree($term->vid, $tid, 1, FALSE) as $child) {
    $options["$prefix/{$child->tid}/$suffix"] = check_plain($child->name);
  }
  asort($options);
  $options = array($default => check_plain($term->name)) + $options;
  return g2migrate_breadcrumb_form($form, $form_state, $options, $default);
}

function g2migrate_breadcrumb_form($form, &$form_state, $options, $default) {

  $form['#validate'][] = 'g2migrate_breadcrumb_form_validate';
  $form['#submit'][] = 'g2migrate_breadcrumb_form_submit';
  $path = drupal_get_path('module', 'g2migrate');
  $form['#attached']['js'][] = "$path/g2migrate.js";

  $form['choice'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $default,
  );
  $form['options'] = array(
    '#type' => 'value',
    '#value' => $options,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
  );

  return $form;
}

function g2migrate_breadcrumb_form_validate($form, &$form_state) {
  $choice = $form_state['values']['choice'];
  if (!array_key_exists($choice, $form_state['values']['options'])) {
    form_set_error('choice');
  }
}

function g2migrate_breadcrumb_form_submit($form, &$form_state) {
  preg_match('/^\/([^#]*)#?(.*)/', $form_state['values']['choice'], $matches);
  drupal_goto($matches[1], array('fragment' => $matches[2]));
}

