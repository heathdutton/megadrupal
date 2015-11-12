<?php

/**
 * @file
 * Preprocessors and helper functions to make theming easier.
 */

/**
 * Returns HTML for the region.
 *
 * @param array $variables
 *   An associative array containing:
 *   - pane: The rendering pane.
 *   - region: The rendering region.
 *   - level: The level of region.
 *
 * @return string
 *   The HTML for region.
 */
function theme_dynamic_panes_ipe_region($variables) {
  $output = '';

  $pane = $variables['pane'];
  $region = $variables['region'];
  $level = $variables['level'];

  if (!empty($pane) && !empty($region) && !empty($level)) {
    $wrapper_attributes = array();
    $wrapper_attributes['class'][] = 'dynamic-panes-ipe-sort-container';
    $wrapper_attributes['data-form-element-name'] = $region->getFormElementName($level);

    $output .= theme('dynamic_panes_ipe_region_toolbar', array(
      'pane' => $pane,
      'region' => $region,
      'level' => $level,
    ));

    $output .= '<div' . drupal_attributes($wrapper_attributes) . '>';
    foreach ($region->getBlocksByLevel($level) as $block) {
      $output .= theme('dynamic_panes_ipe_block', array('block' => $block));
    }

    $output .= '</div>';
  }

  return $output;
}

/**
 * Returns HTML for the region's toolbar.
 *
 * @param array $variables
 *   An associative array containing:
 *   - pane: The rendering pane.
 *   - region: The rendering region.
 *   - level: The level of region.
 *
 * @return string
 *   The HTML for region's toolbar.
 */
function theme_dynamic_panes_ipe_region_toolbar($variables) {
  $output = '';

  $pane = $variables['pane'];
  $region = $variables['region'];
  $level = $variables['level'];

  if (!empty($pane) && !empty($region) && !empty($level)) {
    $region_info = t('Region: @region_name (@type) | Level: @level', array(
      '@region_name' => $region->getRegionName(),
      '@type' => $pane->getTypeName(),
      '@level' => $level == 'all' ? t('- None -') : $level,
    ));

    $attributes = array(
      'class' => array(
        'dynamic-panes-ipe-toolbar',
        'dynamic-panes-ipe-innerwrapper',
        'dynamic-panes-ipe-innerwrapper-pane',
        'clearfix',
      ),
    );

    $links = '';
    if (!empty($variables['links'])) {
      $links = theme('links', array(
        'links' => $variables['links'],
        'attributes' => array('class' => array('dynamic-panes-ipe-linkbar')),
      ));
    }

    $toolbar = '<div' . drupal_attributes($attributes) . '>';
    $toolbar .= $links;
    $toolbar .= '<span class="pane-info">' . $region_info . '</span>';
    $toolbar .= '</div>';

    $wrapper_attributes = array(
      'class' => array(
        'dynamic-panes-ipe-on',
        'dynamic-panes-ipe-toolbar-wrapper',
        'dynamic-panes-ipe-toolbar-wrapper-pane',
      ),
    );

    $output .= '<div' . drupal_attributes($wrapper_attributes) . '>';
    $output .= $toolbar;
    $output .= '</div>';
  }

  return $output;
}

/**
 * Returns HTML for the IPE block.
 *
 * @param array $variables
 *   An associative array containing:
 *   - block: The rendering block.
 *
 * @return string
 *   The HTML for the IPE block.
 */
function theme_dynamic_panes_ipe_block($variables) {
  $output = '';

  $block = $variables['block'];

  if (!empty($block)) {
    $toolbar = '';

    if (!empty($variables['links'])) {
      $attributes = array(
        'class' => array(
          'dynamic-panes-ipe-dragbar',
          'dynamic-panes-ipe-draghandle',
          'clearfix',
        ),
      );

      $links = '<div' . drupal_attributes($attributes) . '>';
      $links .= theme('links', array(
        'links' => $variables['links'],
        'attributes' => array('class' => array('dynamic-panes-ipe-linkbar')),
      ));
      $links .= '<span class="dynamic-panes-ipe-draghandle-icon"><span class="dynamic-panes-ipe-draghandle-icon-inner"></span></span>';
      $links .= '</div>';

      $attributes = array(
        'class' => array(
          'dynamic-panes-ipe-handlebar-wrapper',
          'dynamic-panes-ipe-on dynamic-panes-ipe-toolbar-wrapper',
          'dynamic-panes-ipe-toolbar-wrapper-block',
        ),
      );

      $toolbar .= '<div' . drupal_attributes($attributes) . '>';
      $toolbar .= $links;
      $toolbar .= '</div>';
    }

    $wrapper_attributes = array();
    $wrapper_attributes['class'][] = 'dynamic-panes-ipe-block';
    $wrapper_attributes['data-block-id'][] = $block->getID();

    $output .= '<div' . drupal_attributes($wrapper_attributes) . '>';
    $output .= $toolbar . $block->render();
    $output .= '</div>';
  }

  return $output;
}

/**
 * Returns HTML for the IPE toolbar.
 *
 * @param array $variables
 *   An associative array containing:
 *   - buttons: The buttons.
 *   - forms: The forms.
 *
 * @return string
 *   The HTML for the IPE toolbar.
 */
function theme_dynamic_panes_ipe_toolbar($variables) {
  $buttons = $variables['buttons'];

  $output = '<div id="dynamic-panes-ipe-control-container" class="clearfix">';

  foreach ($buttons as $key => $ipe_buttons) {
    $id = 'dynamic-panes-ipe-control-' . $key;
    $output .= '<div id="' . $id . '" class="dynamic-panes-ipe-control">';

    // Controls in this container will appear when the IPE is not on.
    $output .= '<div class="dynamic-panes-ipe-button-container clearfix">';
    foreach ($ipe_buttons as $button) {
      $output .= is_string($button) ? $button : drupal_render($button);
    }
    $output .= '</div>';
    $output .= '</div>';
  }

  $output .= '<div class="dynamic-panes-ipe-form-container clearfix">';
  $form = drupal_get_form('dynamic_panes_ipe_sort_form');
  $output .= render($form);
  $output .= '</div>';

  $output .= '</div>';

  return $output;
}

/**
 * Preprocess variables for theme('dynamic_panes_ipe_region_toolbar').
 */
function template_preprocess_dynamic_panes_ipe_region_toolbar(&$variables) {
  $region = $variables['region'];
  $level = $variables['level'];

  if (!empty($region) && !empty($level)) {
    if ($links = $region->getAdminLinksByType('layout-add-block')) {
      if (!empty($links[$level])) {
        $link = $links[$level];

        $attributes = array('class' => array('add', 'layout-add-block'));
        $variables['links']['layout-add-block'] = array(
          'title' => '<span>' . $link['title'] . '</span>',
          'href' => $link['href'],
          'query' => isset($link['query']) ? $link['query'] : array(),
          'attributes' => drupal_array_merge_deep($attributes, $link['attributes']),
          'html' => TRUE,
        );
      }
    }
  }
}

/**
 * Preprocess variables for theme('dynamic_panes_ipe_block').
 */
function template_preprocess_dynamic_panes_ipe_block(&$variables) {
  $block = $variables['block'];

  if (isset($block)) {
    if ($link = $block->getAdminLinksByType('edit-block')) {
      $attributes = array('class' => array('edit-block'));
      $variables['links']['edit-block'] = array(
        'title' => '<span>' . $link['title'] . '</span>',
        'href' => $link['href'],
        'query' => isset($link['query']) ? $link['query'] : array(),
        'attributes' => drupal_array_merge_deep($attributes, $link['attributes']),
        'html' => TRUE,
      );
    }

    if ($link = $block->getAdminLinksByType('delete-block')) {
      $attributes = array('class' => array('delete-block'));
      $variables['links']['delete-block'] = array(
        'title' => '<span>' . $link['title'] . '</span>',
        'href' => $link['href'],
        'query' => isset($link['query']) ? $link['query'] : array(),
        'attributes' => drupal_array_merge_deep($attributes, $link['attributes']),
        'html' => TRUE,
      );
    }
  }
}
