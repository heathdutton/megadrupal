<?php
/**
 * @file
 * Provides admin functions for the Vimeo Link Formatter.
 */


/**
 * Implements hook_field_formatter_settings_summary_FORMATTER().
 *
 * @return String
 *    Translated text string summarizing the formatter settings.
 */
function vimeo_link_formatter_field_formatter_settings_summary_vimeo_link_formatter_player($field, $instance, $view_mode) {
  // Access the formatter settings for this instance of the Link field.
  $settings = $instance['display'][$view_mode]['settings'];

  // Build a comma-separated list of available options.
  $options = array(
    'portrait' => t('Portrait'),
    'title' => t('Title'),
    'byline' => t('Byline'),
    'autoplay' => t('Autoplay'),
    'loop' => t('Loop'),
    'js_api' => t('Javascript API'),
  );
  foreach (array_keys($options) as $name) {
    if (!$settings[$name]) {
      unset($options[$name]);
    }
  }

  // Join the enabled options with commas and end with a period.
  // Set $options to an empty string if none are enabled.
  $options = empty($options) ? '' : implode(', ', $options) . '.';

  // This CSS is (mostly) semantic, not style.  Inline CSS is appropriate.
  $style = '
    background-color: #' . $settings['color'] . ';
    color: black;
    font-weight: bold;
    padding-left: 0.2em;
    padding-right: 0.2em;
  ';
  $vars = array(
    '!width' => $settings['width'],
    '!height' => $settings['height'],
    // Inline CSS is not translatable.
    '!attributes' => drupal_attributes(compact('style')),
    '!color' => $settings['color'],
    '!options' => $options,
  );

  return t('!width x !height <code !attributes>!color</code> Player options: !options', $vars);
}

/**
 * Implements hook_field_formatter_settings_form_FORMATTER().
 */
function vimeo_link_formatter_field_formatter_settings_form_vimeo_link_formatter_player($field, $instance, $view_mode, $form, &$form_state) {
  // Access the formatter settings for this instance of the Link field.
  $settings = $instance['display'][$view_mode]['settings'];
  $defaults = vimeo_link_formatter_default_settings_player();

  // A render()-compatible array that is embeddeble into a Form API form.
  $form = array();

  // Width textbox.
  $form['width'] = array(
    '#title' => t('Width'),
    '#description' => t('The width of the player in pixels or relative to the container element.  Do not include "<em>px</em>".  E.g. "<em>500</em>" or "<em>100%</em>".  Defaults to "<em>!default</em>".', array('!default' => $defaults['width'])),
  );

  // Height textbox.
  $form['height'] = array(
    '#title' => t('Height'),
    '#description' => t('The height of the player in pixels or relative to the container element.  Do not include "<em>px</em>".  E.g. "<em>280</em>" or "<em>56%</em>".  Defaults to "<em>!default</em>".', array('!default' => $defaults['height'])),
  );

  // Color textbox.
  // Inline CSS is not translatable.
  $style = 'font-weight: bold; padding-left: 0.2em; padding-right: 0.2em;';
  $example_style = "background-color: #F90; color: black; $style";
  $default_style = "background-color: #{$defaults['color']} ; color: white; $style;";
  $vars = array(
    '!default' => $defaults['color'],
    '!default_style' => drupal_attributes(array('style' => $default_style)),
    '!example_style' => drupal_attributes(array('style' => $example_style)),
  );
  $form['color'] = array(
    '#title' => t('Color'),
    '#description' => t('The color of links and controls (on hover) of the player, such as the title and byline.  CSS colors are not valid.  Six digit hexadecimal colors <em>without</em> the hash/pound character ("<em>#</em>") are valid.  E.g. <code !example_style>FF9900</code>. Defaults to <code !default_style>!default</code>.', $vars),
  );

  // Video information.
  $form['portrait'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Portrait'),
    '#description' => t("Display the video submitter's picture or avatar."),
  );

  $form['title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Title'),
    '#description' => t('Display the name of the video.'),
  );

  $form['byline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Byline'),
    '#description' => t('Display who the video is by.'),
  );

  // Autoplay checkbox.
  $form['autoplay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoplay'),
    '#description' => t('Automatically play the video on load.  This also causes the portrait, title and byline to be hidden.'),
  );

  // Loop textbox.
  $form['loop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop'),
    '#description' => t('Play the video repeatedly.'),
  );

  // js api.
  $form['js_api'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable javascript API'),
    '#description' => t('Enable the vimeo javascript api (this has performance implications-- don\'t use unless you know what you\'re doing. See <a href="!url">Vimeo Player Javascript API</a> for more information).', array('!url' => 'http://vimeo.com/api/docs/player-js')),
  );


  // Set some Form API attributes that apply to all elements.
  $size = max(drupal_strlen('000000'), drupal_strlen('100%'));
  foreach (array_keys($form) as $key) {
    // Textboxes are smaller than default.  #size is ignored for checkboxes.
    $form[$key]['#size'] = $size;

    // Default to #type => textfield if #type is not already set.
    if (!isset($form[$key]['#type'])) {
      $form[$key]['#type'] = 'textfield';
    }

    // Set #default_value too.
    if (isset($settings[$key])) {
      $form[$key]['#default_value'] = $settings[$key];
    }
  }

  return $form;
}

/**
 * Implementation of hook_field_formatter_settings_summary_FORMATTER().
 *
 * @return String
 *    Translated text string summarizing the formatter settings.
 */
function vimeo_link_formatter_field_formatter_settings_summary_vimeo_link_formatter_thumbnail($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  $summary[] = t('Use Vimeo video thumb of size "@thumb_size"', array(
    '@thumb_size'     => $settings['thumb_size'],
  )); // we use t() for translation and placeholders to guard against attacks

  $link_types = array(
    'content' => t('Linked to content'),
    'video' => t('Linked to video'),
  );

  // Display this setting only if image is linked.
  if (isset($link_types[$settings['image_link']])) {
    $summary[] = $link_types[$settings['image_link']];
  }

  return implode('<br />', $summary);
}

/**
 * Implementation of hook_field_formatter_settings_form_FORMATTER().
 */
function vimeo_link_formatter_field_formatter_settings_form_vimeo_link_formatter_thumbnail($field, $instance, $view_mode, $form, &$form_state) {
  // This gets the view_mode where our settings are stored
  $display = $instance['display'][$view_mode];

  // This gets the actual settings
  $settings = $display['settings'];

  // Initialize the element variable
  $element = array();

  // Add your select box
  if (module_exists('imagecache_external')) {
    $size_options = image_style_options();
  }
  else {
    $size_options = array(
      'small' => 'Small',
      'medium' => 'Medium',
      'large' => 'Large',
    );
  }
  $element['thumb_size'] = array(
    '#type'           => 'select',                                 // Use a select box widget
    '#title'          => t('Thumbnail Size'),                      // Widget label
    '#description'    => t('Select what size of Vimeo thumbnail'), // Helper text
    '#default_value'  => $settings['thumb_size'],                  // Get the value if it's already been set
    '#options'        => $size_options,
  );

  $link_types = array(
    'content' => t('Content'),
    'video' => t('Video'),
  );
  $element['image_link'] = array(
    '#title' => t('Link image to'),
    '#type' => 'select',
    '#default_value' => $settings['image_link'],
    '#empty_option' => t('Nothing'),
    '#options' => $link_types,
  );
  return $element;
}
