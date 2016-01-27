<?php

/**
 * @file
 * Definition of the 'parchment_scroll' panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Parchment scroll'),
  'description' => t('Presents the panes or panels with a parchment scroll background'),
  'render region' => 'panels_beautified_parchment_scroll_render_region',
  'render pane' => 'panels_beautified_parchment_scroll_render_pane',
  'settings form' => 'panels_beautified_parchment_scroll_settings_form',
  'hook theme' => array(
    'panels_beautified_parchment_scroll' => array(
      'variables' => array('content' => NULL),
      'path' => drupal_get_path('module', 'panels_beautified') . '/plugins/styles/parchment_scroll',
      'template' => 'parchment-scroll',
    ),
  ),
);

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_panels_beautified_parchment_scroll_render_region($vars) {
  $display = $vars['display'];
  $region_id = $vars['region_id'];
  $panes = $vars['panes'];
  $settings = $vars['settings'];

  $output = '';

  // Determine where to put the box. If empty or 'pane' around each pane. If
  // 'panel' then just around the whole panel.
  $where = empty($settings['corner_location']) ? 'pane' : $settings['corner_location'];

  $print_separator = FALSE;
  foreach ($panes as $pane_id => $pane) {
    // Add the separator if we've already displayed a pane.
    if ($print_separator) {
      $output .= '<div class="panel-separator">&nbsp;</div>';
    }

    if ($where == 'pane') {
      $output .= theme('panels_beautified_parchment_scroll', array('content' => $pane));
    }
    else {
      $output .= $pane;
      $print_separator = TRUE;
    }
  }

  if ($where == 'panel') {
    $output = theme('panels_beautified_parchment_scroll', array('content' => $output));
  }

  panels_beautified_add_parchment_scroll_css($display, $where);

  return $output;
}

function panels_beautified_add_parchment_scroll_css($display, $where) {
  static $displays_used = array();
  if (empty($displays_used[$display->css_id])) {
    panels_beautified_parchment_scroll_css($display, $where);
    $displays_used[$display->css_id] = TRUE;
  }
}

/**
 * Render callback for a single pane.
 */
function theme_panels_beautified_parchment_scroll_render_pane($vars) {
  $content = $vars['content'];
  $pane = $vars['pane'];
  $display = $vars['display'];

  if (empty($content->content)) {
    return;
  }

  $output = theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $display));

  // Just stick a box around the standard theme_panels_pane.
  $output = theme('panels_beautified_parchment_scroll', array('content' => $output));
  panels_beautified_add_parchment_scroll_css($display, 'pane');
  return $output;
}

/**
 * Settings form callback.
 */
function panels_beautified_parchment_scroll_settings_form($style_settings) {
  $form['corner_location'] = array(
    '#type' => 'select',
    '#title' => t('Box around'),
    '#options' => array(
      'pane' => t('Each pane'),
      'panel' => t('Each region'),
    ),
    '#default_value' => (isset($style_settings['corner_location'])) ? $style_settings['corner_location'] : 'ul',
    '#description' => t('Choose whether to have the background behind each pane (piece of content) or region (each column or region)'),
  );

  return $form;
}

/**
 * Generates the dynamic CSS.
 *
 * @param $display
 *   A Panels display object.
 */
function panels_beautified_parchment_scroll_css($display) {
  $idstr = empty($display->css_id) ? '.parchment-scroll' : "#$display->css_id";
  $css_id = 'parchment-scroll:' . $idstr;

  ctools_include('css');
  $filename = ctools_css_retrieve($css_id);
  if (!$filename) {
    $filename = ctools_css_store($css_id, _panels_beautified_parchment_scroll_css($idstr), FALSE);
  }

  drupal_add_css($filename, array('preprocess' => TRUE));
}

/**
 * Generates the dynamic CSS.
 */
function _panels_beautified_parchment_scroll_css($idstr) {
  $url = '/' . drupal_get_path('module', 'panels_beautified') . '/plugins/styles/parchment_scroll';

  $css = <<<EOF

$idstr {
  margin-bottom: 1em;
}
$idstr .ps-wrapper {
  position: relative;
  /* hasLayout -1 ? For IE only */
  zoom: 1;
}
$idstr .ps-top-edge {
  background: url($url/top.png) repeat-x 15px top;
  background-size: 100% 15px;
  height: 15px;
  font-size: 1px;
  position: relative;
  /* hasLayout -1 ? For IE only */
  zoom: 1;
}
$idstr .ps-top-edge .ps-left-corner {
  background-image: url($url/top-left.png);
  background-size: 15px 15px;
  height: 15px;
  width: 15px;
  position: absolute;
  top: 0;
  left: 0;
}
$idstr .ps-top-edge .ps-right-corner {
  background-image: url($url/top-right.png);
  background-size: 15px 15px;
  height: 15px;
  width: 15px;
  position: absolute;
  top: 0;
  right: 0;
}
$idstr .ps-left-edge {
  background: url($url/left.png) repeat-y 0 0;
  background-size: 15px 100%;
  position: relative;
  /* hasLayout -1 ? For IE only */
  zoom: 1;
}
$idstr .ps-left-edge .ps-right-edge {
  background: url($url/right.png) repeat-y right 0;
  background-size: 15px 100%;
  position: relative;
  padding: 0 15px;
  /* hasLayout -1 ? For IE only */
  zoom: 1;
}
$idstr .ps-left-edge .ps-right-edge .ps-background {
  background: url($url/center.png) 0 0;
  background-size: 100% 100%;
  padding: 1px;
}
$idstr .ps-bottom-edge {
  background: url($url/bottom.png) repeat-x 15px bottom;
  background-size: 100% 15px;
  height: 15px;
  font-size: 1px;
  position: relative;
  /* hasLayout -1 ? For IE only */
  zoom: 1;
}
$idstr .ps-bottom-edge .ps-left-corner {
  background-image: url($url/bottom-left.png);
  background-size: 15px 15px;
  height: 15px;
  width: 15px;
  position: absolute;
  top: 0;
  left: 0;
}
$idstr .ps-bottom-edge .ps-right-corner {
  background-image: url($url/bottom-right.png);
  background-size: 15px 15px;
  height: 15px;
  width: 15px;
  position: absolute;
  top: 0;
  right: 0;
}
$idstr div.admin-links {
  margin-top: -14px;
  margin-left: -12px;
}

EOF;

  return $css;
}
