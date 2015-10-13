<?php

/**
 * Hijacking theme_admin_block_content().
 * Since we already hijack the complete theme function
 * as a bonus we convert this to utilize jQueryUI accordion
 */
function jqueryui_vtcore_admin_block_content($variables) {
  $content = $variables['content'];
  $output = '';

  if (!empty($content)) {
    global $theme_key;
    _jqueryui_selected_theme_init('load', $theme_key);
    _jqueryui_load_js(BASE_THEME, 'misc');
    _jqueryui_load_js(BASE_THEME, 'accordion');
    _jqueryui_load_css(BASE_THEME, 'base');
    drupal_add_library('system', 'ui.accordion');

    $wrapper_attributes = array(
      'id' => 'admin-list-accordion',
      'class' => array(
        'ui-accordion-icons',
        'ui-accordion',
        'ui-helper-reset',
      ),
    );

    $header_attributes = array(
      'class' => array(
        'ui-accordion-header',
        'ui-state-default',
        'ui-state-active',
        'ui-corner-top',
        'ui-helper-reset',
      ),
    );

    $content_attributes = array(
      'class' => array(
        'ui-accordion-content',
        'ui-widget-content',
        'ui-corner-bottom',
        'ui-accordion-content-active',
        'ui-helper-reset',
      ),
    );

    $button_attributes = array(
      'class' => array(
        'ui-state-default',
        'ui-corner-all',
        'ui-header-accordion-button',
        'ui-helper-hidden',
      ),
    );

    $icon_attributes = array(
      'class' => array(
        'ui-icon',
        'ui-icon-circle-arrow-s',
      ),
    );

    $icon = '<span' . drupal_attributes($icon_attributes) . '></span>';

    $output .= '<div' . drupal_attributes($wrapper_attributes) . '>';
    foreach ($content as $item) {
      $pseudo_link = l($item['title'], '#', array('attributes' => array('class' => 'header')));

      $link = l(t('Open'), $item['href'], array('attributes' => $button_attributes));

      $output .= '<h3' . drupal_attributes($header_attributes) . '>';
      $output .= $icon;
      $output .= $pseudo_link;
      $output .= $link;
      $output .= '</h3>';
      $output .= '<div' . drupal_attributes($content_attributes) . '>' . filter_xss_admin($item['description']) . '</div>';
    }
    $output .= '</div>';
  }
  return $output;
}

/**
 * Hijacking theme_admin_block();
 */
function jqueryui_vtcore_admin_block($variables) {
  $block = $variables['block'];
  $output = '';

  // Don't display the block if it has no content to display.
  if (empty($block['show'])) {
    return $output;
  }

  $output .= '<div class="admin-panel ui-corner-all ui-widget-content">';
  if (!empty($block['title'])) {
    $output .= '<h3>' . $block['title'] . '</h3>';
  }
  if (!empty($block['content'])) {
    $output .= '<div class="body">' . $block['content'] . '</div>';
  }
  else {
    $output .= '<div class="description">' . $block['description'] . '</div>';
  }
  $output .= '</div>';

  return $output;
}

/**
 * Hijacking theme_admin_page()
 */
function jqueryui_vtcore_admin_page($variables) {
  $blocks = $variables['blocks'];

  $stripe = 0;
  $container = array();

  foreach ($blocks as $block) {
    if ($block_output = theme('admin_block', array('block' => $block))) {
      if (empty($block['position'])) {
        // perform automatic striping.
        $block['position'] = ++$stripe % 2 ? 'left' : 'right';
      }
      if (!isset($container[$block['position']])) {
        $container[$block['position']] = '';
      }
      $container[$block['position']] .= $block_output;
    }
  }

  $output = '<div class="admin clearfix">';

  foreach ($container as $id => $data) {
    $output .= '<div class="' . $id . ' clearfix">';
    $output .= $data;
    $output .= '</div>';
  }
  $output .= '</div>';
  return $output;
}


/**
 * Hijacking theme_node_add_list()
 */
function jqueryui_vtcore_node_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    global $theme_key;
    _jqueryui_selected_theme_init('load', $theme_key);
    _jqueryui_load_js(BASE_THEME, 'misc');
    _jqueryui_load_js(BASE_THEME, 'accordion');
    _jqueryui_load_css(BASE_THEME, 'base');
    drupal_add_library('system', 'ui.accordion');

    $wrapper_attributes = array(
      'id' => 'admin-list-accordion',
      'class' => array(
        'ui-accordion-icons',
        'ui-accordion',
        'ui-helper-reset',
      ),
    );

    $header_attributes = array(
      'class' => array(
        'ui-accordion-header',
        'ui-state-default',
        'ui-state-active',
        'ui-corner-top',
        'ui-helper-reset',
      ),
    );

    $content_attributes = array(
      'class' => array(
        'ui-accordion-content',
        'ui-widget-content',
        'ui-corner-bottom',
        'ui-accordion-content-active',
        'ui-helper-reset',
      ),
    );

    $button_attributes = array(
      'class' => array(
        'ui-state-default',
        'ui-corner-all',
        'ui-header-accordion-button',
        'ui-helper-hidden',
      ),
    );

    $icon_attributes = array(
      'class' => array(
        'ui-icon',
        'ui-icon-circle-arrow-s',
      ),
    );
    $icon = '<span' . drupal_attributes($icon_attributes) . '></span>';

    $output .= '<div' . drupal_attributes($wrapper_attributes) . '>';
    foreach ($content as $item) {
      $pseudo_link = l($item['title'], '#', array('attributes' => array('class' => 'header')));

      $link = l(t('Open'), $item['href'], array('attributes' => $button_attributes));

      $output .= '<h3' . drupal_attributes($header_attributes) . '>';
      $output .= $icon;
      $output .= $pseudo_link;
      $output .= $link;
      $output .= '</h3>';
      $output .= '<div' . drupal_attributes($content_attributes) . '>' . filter_xss_admin($item['description']) . '</div>';
    }
    $output .= '</div>';

  }
  else {
    $output = '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }
  return $output;
}