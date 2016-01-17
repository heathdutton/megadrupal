<?php

/**
 * @file
 * Provides settings for the branded seven theme.
 */

/**
 * Implements template_preprocess_html().
 */
function brand_seven_preprocess_html(&$variables) {

  // Has the user added branding colours?
  if (theme_get_setting('brand_seven_branding_light') != ''
    && theme_get_setting('brand_seven_branding_light') != ''
    && theme_get_setting('brand_seven_page_background') != '') {
    $background = theme_get_setting('brand_seven_page_background');
    $light = theme_get_setting('brand_seven_branding_light');
    $dark = theme_get_setting('brand_seven_branding_dark');
    // drupal_static_reset('drupal_add_css');
    drupal_add_css(_brand_seven_branding_css($background, $light, $dark),
      array(
        'type' => 'inline',
        'group' => CSS_THEME,
        'media' => 'screen',
        'weight' => 999,
        'preprocess' => FALSE,
      )
    );
    drupal_add_css(drupal_get_path("theme", "brand_seven") . '/css/fontello.css', array(
      'group' => CSS_THEME,
      'weight' => 115,
      'preprocess' => FALSE,
      )
    );
    drupal_add_css(drupal_get_path("theme", "brand_seven") . '/css/fontello-ie7.css', array(
      'group' => CSS_THEME,
      'weight' => 115,
      'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE),
      'preprocess' => FALSE,
      )
    );
  }
}

/**
 * Return branded CSS for the Seven theme.
 */
function _brand_seven_branding_css($background, $light, $dark) {
  $output = '';
  $output .= '#page, body{ background: ' . $background . '; }';
  $output .= 'a{ color: ' . $dark . '; }';
  $output .= 'input.form-submit, a.button {  background: ' . $dark . '; color: ' . $light . '; border: 1px solid ' . $dark . '; }';
  $output .= 'ul.primary li.active a, ul.primary li.active a.active, ul.primary li.active a:active, ul.primary li.active a:visited { background: ' . $background . '; color: ' . $dark . '; }';
  $output .= 'ul.primary li a:link, ul.primary li a.active, ul.primary li a:active, ul.primary li a:visited, ul.primary li a:hover, ul.primary li.active a { background: ' . $light . '; color: ' . $dark . '; border-color: ' . $light . '; }';
  $output .= 'ul.secondary { background: ' . $background . '; }';

  $output .= '.breadcrumb a{ color: ' . $light . '; }';
  $output .= '#branding{ background-color: ' . $dark . '; }';
  $output .= '#branding h1.page-title{ color: ' . $light . '; }';
  $output .= '#branding div.breadcrumb{ color: ' . $light . '; }';
  $output .= 'div.add-or-remove-shortcuts a span.icon, div.add-or-remove-shortcuts a:focus span.text, div.add-or-remove-shortcuts a:hover span.text{ color: ' . $light . '; }';

  $output .= 'ul.admin-list li { color: ' . $dark . '; }';

  $output .= 'fieldset{ border: 1px solid ' . $dark . '; }';
  $output .= 'table th{ background: ' . $dark . '; color: ' . $light . '; }';
  $output .= 'table th a{ color: ' . $light . '; }';
  $output .= 'table th.active{ background: ' . $light . '; }';
  $output .= 'table th.active a{ color: ' . $dark . '; }';
  return $output;
}

/**
 * Override of theme_tablesort_indicator().
 *
 * Use our own image versions, so they show up as black and not gray on gray.
 */
function brand_seven_tablesort_indicator($variables) {
  $style = $variables['style'];
  $theme_path = drupal_get_path('theme', 'brand_seven');
  if ($style == 'asc') {
    return '<i class="icon-down-open"></i>';
  }
  else {
    return '<i class="icon-up-open"></i>';
  }
}

/**
 * Implements hook_form_alter().
 */
function brand_seven_form_alter(&$form, &$form_state, $form_id) {
  if (!isset($form['#bundle'])) {
    $form['#bundle'] = '0EzelP40I33r';
  }
  switch ($form_id) {
    case 'node_admin_content':
      // Remove status dropdown.
      if (theme_get_setting('brand_seven_hide_status')
        && theme_get_setting('brand_seven_disable_publishing')
        && theme_get_setting('brand_seven_disable_sticky')
        && theme_get_setting('brand_seven_disable_promoted')) {
        unset($form['filter']['filters']['status']['filters']['status']);
      }
      // Remove update options.
      if (theme_get_setting('brand_seven_hide_update_options')
        && theme_get_setting('brand_seven_disable_publishing')
        && theme_get_setting('brand_seven_disable_sticky')
        && theme_get_setting('brand_seven_disable_promoted')) {
        unset($form['admin']['options']);
      }
      // Site uses published content.
      if (theme_get_setting('brand_seven_disable_publishing')) {
        $options = array('publish', 'unpublish', 'status-0', 'status-1');
        foreach ($options as $key) {
          unset($form['admin']['options']['operation']['#options'][$key]);
          unset($form['filter']['filters']['status']['filters']['status']['#options'][$key]);
        }
        unset($form['admin']['nodes']['#header']['status']);
        foreach ($form['admin']['nodes']['#options'] as $row) {
          unset($row['status']);
        }
      }
      // Site uses promoted content.
      if (theme_get_setting('brand_seven_disable_promoted')) {
        $options = array('demote', 'promote', 'promote-0', 'promote-1');
        foreach ($options as $key) {
          unset($form['admin']['options']['operation']['#options'][$key]);
          unset($form['filter']['filters']['status']['filters']['status']['#options'][$key]);
        }
      }
      // Site uses sticky content.
      if (theme_get_setting('brand_seven_disable_sticky')) {
        $options = array('sticky', 'unsticky', 'sticky-0', 'sticky-1');
        foreach ($options as $key) {
          unset($form['admin']['options']['operation']['#options'][$key]);
          unset($form['filter']['filters']['status']['filters']['status']['#options'][$key]);
        }
      }
      $form['filter']['filters']['status']['filters']['type']['#title'] = 'Content Type is';
      break;

    case $form['#bundle'] . '_node_form':
      if (theme_get_setting('brand_seven_disable_publishing')) {
        $form['options']['status']['#disabled'] = 'disabled';
      }
      if (theme_get_setting('brand_seven_disable_promoted')) {
        unset($form['options']['promote']);
      }
      if (theme_get_setting('brand_seven_disable_sticky')) {
        unset($form['options']['sticky']);
      }
      break;

  }
}
