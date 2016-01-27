<?php
/**
* @file
*   Views query plugin for Imagestyles info.
*/

/**
* Views query plugin for the imagestyles info.
*/
class architecture_imagestyles_plugin_query extends views_plugin_query {
  /**
   * Overrides the query() method.
   * @inheritdoc
   */
  function query($get_count = FALSE) {

  }

  /**
   * Reproducing add_field() here so the spaghetti doesn't stick together.
   */
  function add_field($table, $field, $alias = '', $params = array()) {
    $alias = views_plugin_query_default::add_field($table, $field, $alias = '', $params = array());
    return $alias;
  }

  /**
   * Overrides the execute() method.
   * @inheritdoc
   */
  function execute(&$view) {
    foreach(image_styles() as $style) {
      foreach($style['effects'] as $effect) {
        $row = new stdClass();
        $row->title = $style['name'];
        $row->effects = $effect['label'];
        $row->effect_weight = $effect['weight'];
        switch ($effect['label']) {
          case 'Resize':
          case'Scale and crop':
            $row->settings = $effect['data']['width'] . 'x' . $effect['data']['height'];
            break;
          case 'Scale':
            $row->settings = $effect['data']['width'] . 'x' . $effect['data']['height'];
            $row->settings .= $effect['data']['upscale'] ? t(' (Upscaling allowed.)') : t(' (No upscaling.)');
            break;
          case 'Rotate':
            $row->settings = $effect['data']['degrees'] . t(' degrees, ') . $effect['data']['bgcolor'] . t(' background');
            $row->settings .= $effect['data']['random'] ? t(', randomized.') : '.';
            break;
          case 'Crop':
            $row->settings = $effect['data']['width'] . 'x' . $effect['data']['height'];
            $row->settings .= t(', anchored to ') . $effect['data']['anchor'];
            break;
          default:
            $settings = print_r($effect['data'], 1);
            $row->settings = empty($effect['data']) ? t('[none]') : $settings;
            break;
        }
        $row->effect_desc = $effect['help'];
        $preview = $this->architecture_imagestyle_preview(array('style' =>$style));
        $row->preview = $preview;
        $view->result[] = $row;
      }
    }
  }

  function architecture_imagestyle_preview($variables) {
    $style = $variables['style'];

    $sample_image = variable_get('image_style_preview_image', drupal_get_path('module', 'image') . '/sample.png');
    $sample_width = 160;
    $sample_height = 160;

    // Set up original file information.
    $original_path = $sample_image;
    $original_image = image_get_info($original_path);
    if ($original_image['width'] > $original_image['height']) {
      $original_width = min($original_image['width'], $sample_width);
      $original_height = round($original_width / $original_image['width'] * $original_image['height']);
    }
    else {
      $original_height = min($original_image['height'], $sample_height);
      $original_width = round($original_height / $original_image['height'] * $original_image['width']);
    }
    $original_attributes = array_intersect_key($original_image, array('width' => '', 'height' => ''));
    $original_attributes['style'] = 'width: ' . $original_width . 'px; height: ' . $original_height . 'px;';

    // Set up preview file information.
    $preview_file = image_style_path($style['name'], $original_path);
    if (!file_exists($preview_file)) {
      image_style_create_derivative($style, $original_path, $preview_file);
    }
    $preview_image = image_get_info($preview_file);
    if ($preview_image['width'] > $preview_image['height']) {
      $preview_width = min($preview_image['width'], $sample_width);
      $preview_height = round($preview_width / $preview_image['width'] * $preview_image['height']);
    }
    else {
      $preview_height = min($preview_image['height'], $sample_height);
      $preview_width = round($preview_height / $preview_image['height'] * $preview_image['width']);
    }
    $preview_attributes = array_intersect_key($preview_image, array('width' => '', 'height' => ''));
    $preview_attributes['style'] = 'width: ' . $preview_width . 'px; height: ' . $preview_height . 'px;';

    // Build the preview of the image style.
    $preview_url = file_create_url($preview_file) . '?cache_bypass=' . REQUEST_TIME;
    $output = check_plain($style['name']) . ' (' . l(t('view actual size'), file_create_url($preview_file) . '?' . time()) . ')';
    $output .= '<div class="preview-image modified-image" style="' . $preview_attributes['style'] . '">';
    $output .= '<a href="' . file_create_url($preview_file) . '?' . time() . '">' . theme('image', array('path' => $preview_url, 'alt' => t('Sample modified image'), 'title' => '', 'attributes' => $preview_attributes)) . '</a>';
    $output .= '</div>'; // End preview-image.

    return $output;
  }
}

