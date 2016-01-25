<?php

/**
 * Build the ImageScale settings form (to enter API key).
 */
function imagescale_settings_form($form, &$form_state) {
  $form = array();

  $styles = imagescale_styles();

  // If there are no styles defined (empty array), prompt to make them.
  if (!$styles) {
    $form['markup_make_styles'] = array(
      '#markup' => '<p>' . t('You haven\'t created any automations (aka styles) yet.  <a href="@imagescale">Go to @imagescale to create an automation</a>.', array('@imagescale' => IMAGESCALE_MANAGEMENT_URI)) . '</p>',
    );
  }

  // Path to example image.
  $path = drupal_get_path('module', 'imagescale') . '/example_images/Rabbit_small.JPG';
  global $base_url;
  $url = $base_url . '/' . $path;
  if ($styles) {
    // Pick off the first style to use as a demonstration.
    $style = array_pop($styles);
    $src = imagescale_url($style['label'], $url);
    $form['example_image'] = array(
      '#markup' => t('Your ImageScale style @label will turn the <a href="!url" alt="A cute rabbit.">example image</a> into this transformed image:<br />

<img src="!src" alt="A cute rabbit, image modified by ImageScale." /><br />

      The path to the transformed image looks like this: <code>!src</code>',
        array('@label' => $style['label'], '!url' => $url, '!src' => $src)),
    );
  }

  $form['imagescale_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#size' => 70,
    '#maxlength' => 70,
    '#required' => TRUE,
    '#default_value' => variable_get('imagescale_apikey', ''),
  );
  $form['imagescale_namespace'] = array(
    '#type' => 'textfield',
    '#title' => t('Namespace'),
    '#required' => TRUE,
    '#default_value' => variable_get('imagescale_namespace', ''),
  );

  return system_settings_form($form);

}

/**
 * @file
 * Administration pages for image settings.
 *
 * Menu callback; Listing of all current image styles.
 */
function imagescale_style_list() {
  $page = array();

  $styles = imagescale_styles();
  $page['imagescale_style_list'] = array(
    '#markup' => theme('imagescale_style_list', array('styles' => $styles)),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'image') . '/imagescale.admin.css' => array()),
    ),
  );

  return $page;

}

/**
 * Returns HTML for the page containing the list of image styles.
 *
 * @param $variables
 *   An associative array containing:
 *   - styles: An array of all the image styles returned by imagescale_get_styles().
 *
 * @see imagescale_get_styles()
 * @ingroup themeable
 */
function theme_imagescale_style_list($variables) {
  $styles = $variables['styles'];

  $header = array(t('Style name'), t('Settings'), array('data' => t('Operations'), 'colspan' => 3));
  $rows = array();
  foreach ($styles as $style) {
    $row = array();
    $row[] = l($style['name'], 'admin/config/media/imagescale-styles/edit/' . $style['name']);
    $link_attributes = array(
      'attributes' => array(
        'class' => array('imagescale-style-link'),
      ),
    );
    if ($style['storage'] == IMAGE_STORAGE_NORMAL) {
      $row[] = t('Custom');
      $row[] = l(t('edit'), 'admin/config/media/imagescale-styles/edit/' . $style['name'], $link_attributes);
      $row[] = l(t('delete'), 'admin/config/media/imagescale-styles/delete/' . $style['name'], $link_attributes);
    }
    elseif ($style['storage'] == IMAGE_STORAGE_OVERRIDE) {
      $row[] = t('Overridden');
      $row[] = l(t('edit'), 'admin/config/media/imagescale-styles/edit/' . $style['name'], $link_attributes);
      $row[] = l(t('revert'), 'admin/config/media/imagescale-styles/revert/' . $style['name'], $link_attributes);
    }
    else {
      $row[] = t('Default');
      $row[] = l(t('edit'), 'admin/config/media/imagescale-styles/edit/' . $style['name'], $link_attributes);
      $row[] = '';
    }
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array(
      'colspan' => 4,
      'data' => t('There are currently no styles. <a href="!url">Add a new one</a>.', array('!url' => url('admin/config/media/imagescale-styles/add'))),
    ));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Returns HTML for a preview of an image style.
 *
 * @param $variables
 *   An associative array containing:
 *   - style: The image style array being previewed.
 *
 * @ingroup themeable
 */
function theme_imagescale_style_preview($variables) {
  $style = $variables['style'];

  $sample_image = variable_get('imagescale_style_preview_image', drupal_get_path('module', 'image') . '/sample.png');
  $sample_width = 160;
  $sample_height = 160;

  // Set up original file information.
  $original_path = $sample_image;
  $original_image = imagescale_get_info($original_path);
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
  $preview_file = imagescale_style_path($style['name'], $original_path);
  if (!file_exists($preview_file)) {
    imagescale_style_create_derivative($style, $original_path, $preview_file);
  }
  $preview_image = imagescale_get_info($preview_file);
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

  // In the previews, timestamps are added to prevent caching of images.
  $output = '<div class="imagescale-style-preview preview clearfix">';

  // Build the preview of the original image.
  $original_url = file_create_url($original_path);
  $output .= '<div class="preview-imagescale-wrapper">';
  $output .= t('original') . ' (' . l(t('view actual size'), $original_url) . ')';
  $output .= '<div class="preview-image original-image" style="' . $original_attributes['style'] . '">';
  $output .= '<a href="' . $original_url . '">' . theme('image', array('path' => $original_path, 'alt' => t('Sample original image'), 'title' => '', 'attributes' => $original_attributes)) . '</a>';
  $output .= '<div class="height" style="height: ' . $original_height . 'px"><span>' . $original_image['height'] . 'px</span></div>';
  $output .= '<div class="width" style="width: ' . $original_width . 'px"><span>' . $original_image['width'] . 'px</span></div>';
  $output .= '</div>'; // End preview-image.
  $output .= '</div>'; // End preview-imagescale-wrapper.

  // Build the preview of the image style.
  $preview_url = file_create_url($preview_file) . '?cache_bypass=' . REQUEST_TIME;
  $output .= '<div class="preview-imagescale-wrapper">';
  $output .= check_plain($style['name']) . ' (' . l(t('view actual size'), file_create_url($preview_file) . '?' . time()) . ')';
  $output .= '<div class="preview-image modified-image" style="' . $preview_attributes['style'] . '">';
  $output .= '<a href="' . file_create_url($preview_file) . '?' . time() . '">' . theme('image', array('path' => $preview_url, 'alt' => t('Sample modified image'), 'title' => '', 'attributes' => $preview_attributes)) . '</a>';
  $output .= '<div class="height" style="height: ' . $preview_height . 'px"><span>' . $preview_image['height'] . 'px</span></div>';
  $output .= '<div class="width" style="width: ' . $preview_width . 'px"><span>' . $preview_image['width'] . 'px</span></div>';
  $output .= '</div>'; // End preview-image.
  $output .= '</div>'; // End preview-imagescale-wrapper.

  $output .= '</div>'; // End imagescale-style-preview.

  return $output;
}
