<?php

/**
 * Implements hook_html_head_alter().
 */
function neato_html_head_alter(&$head_elements) {
  // HTML5 charset declaration.
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );

  // Optimize mobile viewport.
  $head_elements['mobile_viewport'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1',
    ),
  );
}

/**
 * Implements template_preprocess_page().
 * @param $variables
 */
function neato_preprocess_page(&$variables) {
  // Add page--node_type.tpl.php suggestions.
  if (!empty($variables['node'])) {
    $variables['theme_hook_suggestions'][] = 'page__' . $variables['node']->type;
  }

  $variables['logo_img'] = '';
  if (!empty($variables['logo'])) {
    $variables['logo_img'] = theme('image', array(
      'path' => $variables['logo'],
      'alt' => strip_tags($variables['site_name']) . ' ' . t('logo'),
      'title' => strip_tags($variables['site_name']) . ' ' . t('Home'),
      'attributes' => array(
        'class' => array('logo'),
      ),
    ));
  }

  $variables['linked_logo'] = '';
  if (!empty($variables['logo_img'])) {
    $variables['linked_logo'] = l($variables['logo_img'], '<front>', array(
      'attributes' => array(
        'rel' => 'home',
        'title' => strip_tags($variables['site_name']) . ' ' . t('Home'),
      ),
      'html' => TRUE,
    ));
  }

  $variables['linked_site_name'] = '';
  if (!empty($variables['site_name'])) {
    $variables['linked_site_name'] = l($variables['site_name'], '<front>', array(
      'attributes' => array(
        'rel' => 'home',
        'title' => strip_tags($variables['site_name']) . ' ' . t('Home'),
      ),
      'html' => TRUE,
    ));
  }

  if (!empty($variables['main_menu'])) {
    $variables['alt_main_menu'] = theme('links__system_main_menu', array(
      'links' => $variables['main_menu'],
      'attributes' => array(
        'id' => 'main-menu-links',
        'class' => array('links', 'inline-list', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }
}

/**
 * Implements theme_breadcrumb().
 * @param $variables
 * @return string
 */
function neato_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb) && count($breadcrumb) > 1) {
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $output .= '<div class="breadcrumb">' . implode('', $breadcrumb) . '</div>';
    return $output;
  }
}

/**
 * Implements hook_css_alter().
 * @param $css
 */
function neato_css_alter(&$css) {
  // Remove Drupal core css
  $exclude = array(
//    'modules/aggregator/aggregator.css' => FALSE,
//    'modules/block/block.css' => FALSE,
//    'modules/dblog/dblog.css' => FALSE,
//    'modules/field/theme/field.css' => FALSE,
//    'modules/file/file.css' => FALSE,
//    'modules/filter/filter.css' => FALSE,
//    'modules/forum/forum.css' => FALSE,
//    'modules/help/help.css' => FALSE,
//    'modules/menu/menu.css' => FALSE,
//    'modules/node/node.css' => FALSE,
//    'modules/poll/poll.css' => FALSE,
//    'modules/profile/profile.css' => FALSE,
//    'modules/search/search.css' => FALSE,
//    'modules/syslog/syslog.css' => FALSE,
//    'modules/system/system.admin.css' => FALSE,
//    'modules/system/maintenance.css' => FALSE,
//    'modules/system/system.admin.css' => FALSE,
//    'modules/system/system.base.css' => FALSE,
    'modules/system/system.maintenance.css' => FALSE,
    'modules/system/system.messages.css' => FALSE,
    'modules/system/system.menus.css' => FALSE,
//    'modules/system/system.theme.css' => FALSE,
//    'modules/taxonomy/taxonomy.css' => FALSE,
//    'modules/tracker/tracker.css' => FALSE,
//    'modules/update/update.css' => FALSE,
//    'modules/user/user.css' => FALSE,
//    'misc/vertical-tabs.css' => FALSE,
  );

  $css = array_diff_key($css, $exclude);
}