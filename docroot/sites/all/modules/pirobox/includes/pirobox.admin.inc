<?php
/**
 * @file
 * Administrative page callbacks for the Pirobox module.
 */

function pirobox_general_settings_form() {
  // Pirobox jQuery plugin.
  $pirobox_library_path = pirobox_get_library_path('pirobox');
  // Blur gaussian jQuery plugin.
  $blur_gaussian_library_path = pirobox_get_library_path('blur-gaussian');

  $pirobox_styles = array();

  $eim_module = FALSE;
  if (module_exists('eim')) {
    $eim_module = TRUE;
  }

  // Plugin settings.
  $form['pirobox_plugin_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pirobox plugin settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  // Pirobox
  $form['pirobox_plugin_settings']['pirobox_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Pirobox path'),
    '#description' => t('The location where Pirobox jQuery plugin content are installed. Relative path from Drupal root directory without beginning and trailing slash.'),
    '#default_value' => $pirobox_library_path,
    '#after_build' => array('_pirobox_admin_settings_check_plugin_path')
  );
  // Blur gaussian
  $form['pirobox_plugin_settings']['blur_gaussian_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Blur gaussian path'),
    '#description' => t('The location where Blur gaussian jQuery plugin content are installed. Relative path from Drupal root directory without beginning and trailing slash.'),
    '#default_value' => $blur_gaussian_library_path,
    '#after_build' => array('_pirobox_admin_settings_check_plugin_path')
  );
  $form['pirobox_plugin_settings']['pirobox_compression_type'] = array(
    '#type' => 'radios',
    '#title' => t('JS compression'),
    '#description' => t('Choose Pirobox and Blur gaussian JS files compression level.'),
    '#options' => _pirobox_get_js_options(),
    '#default_value' => variable_get('pirobox_compression_type', 'min')
  );

  // Module settings.
  $form['pirobox_module_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Module settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['pirobox_module_settings']['pirobox_module_compression_type'] = array(
    '#type' => 'radios',
    '#title' => t('JS compression'),
    '#description' => t('Choose the compression level for all Pirobox module JS files.'),
    '#options' => _pirobox_get_js_options(),
    '#default_value' => variable_get('pirobox_module_compression_type', 'min')
  );
  $form['pirobox_module_settings']['pirobox_relative_paths'] = array(
    '#type' => 'checkbox',
    '#title' => t('Relative paths'),
    '#description' => t('By default are used absolute paths when creating image URLs. Overwriting this feature and use relative paths. If you are unsure do not use this option.'),
    '#default_value' => variable_get('pirobox_relative_paths', FALSE)
  );

  // Styles and options.
  $form['pirobox_custom_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styles and options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['pirobox_custom_settings']['pirobox_images_sbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Side by side images'),
    '#description' => t('Display groups of images in the content side by side.'),
    '#default_value' => variable_get('pirobox_images_sbs', 1)
  );
  // Prepare the styles array.
  $pirobox_templates = _pirobox_get_styles();
  foreach ($pirobox_templates as $templates => $template) {
    $pirobox_styles[$template->template_machinename] = $template->template_humanname;
  }
  $form['pirobox_custom_settings']['pirobox_style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#description' => t('Select the style to use for the Pirobox.'),
    '#options' => $pirobox_styles,
    '#default_value' => variable_get('pirobox_style', 'demo1')
  );

  // Overlay settings.
  $form['pirobox_custom_settings']['pirobox_overlay_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overlay settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['pirobox_custom_settings']['pirobox_overlay_settings']['pirobox_overlayclose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overlay close'),
    '#description' => t('Enable closing Pirobox by clicking on the background overlay.'),
    '#default_value' => variable_get('pirobox_overlayclose', 1)
  );
  $form['pirobox_custom_settings']['pirobox_overlay_settings']['pirobox_overlay_color'] = array(
    '#type' => 'radios',
    '#title' => t('Overlay color'),
    '#options' =>  array(
      'transparent' => t('No color, image'),
      'style' => t('Color settings from style'),
      'white' => t('White'),
      'black' => t('Black')
    ),
    '#default_value' => variable_get('pirobox_overlay_color', 'style')
  );
  $background_images = _pirobox_get_background_images();
  $form['pirobox_custom_settings']['pirobox_overlay_settings']['pirobox_overlay_bgimage'] = array(
    '#type' => 'select',
    '#title' => t('Overlay background image'),
    '#options' =>  $background_images,
    '#default_value' => variable_get('pirobox_overlay_bgimage', 'white'),
    '#states' => array(
      'visible' => array(
        ':input[name="pirobox_overlay_color"]' => array('value' => 'transparent')
      )
    )
  );
  $form['pirobox_custom_settings']['pirobox_overlay_settings']['pirobox_overlay_opacity'] = array(
    '#type' => 'select',
    '#title' => t('Overlay opacity'),
    '#description' => t('Determines how visible the background page is behind the Pirobox box.'),
    '#options' => drupal_map_assoc(array('0', '0.10', '0.15', '0.20', '0.25', '0.30', '0.35', '0.40', '0.45', '0.50', '0.55', '0.60', '0.65', '0.70', '0.75', '0.80', '0.85', '0.90', '0.95', '1')),
    '#default_value' => variable_get('pirobox_overlay_opacity', '0.60')
  );
  $form['pirobox_custom_settings']['pirobox_overlay_settings']['pirobox_animation_speed'] = array(
    '#type' => 'select',
    '#title' => t('Animation speed'),
    '#description' => t('Sets the speed to open and close the Pirobox.'),
    '#options' => drupal_map_assoc(array(1, 5, 25, 50, 100, 200, 300, 400, 500, 600, 800, 1000)),
    '#default_value' => variable_get('pirobox_animation_speed', 400)
  );

  $form['pirobox_custom_settings']['pirobox_overlay_settings']['pirobox_linkto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide open image'),
    '#description' => t('Hide the Pirobox link <em>Open image in a new window</em>.'),
    '#default_value' => variable_get('pirobox_linkto', 0)
  );

  // Lightbox image effects.
  $form['pirobox_custom_settings']['pirobox_lightbox_image_effects'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lightbox image effects'),
    '#description' => t('These effects are used to display images in the Pirobox.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['pirobox_custom_settings']['pirobox_lightbox_image_effects']['pirobox_image_effect'] = array(
    '#type' => 'select',
    '#title' => t('Image effect'),
    '#description' => '',
    '#options' => array(
      'fadein' => t('Fade in'),
      'blurgauss' => t('Blur')
    ),
    '#default_value' => variable_get('pirobox_image_effect', 'fadein')
  );
  // Blur settings.
  $form['pirobox_custom_settings']['pirobox_lightbox_image_effects']['pirobox_blurgauss_factor'] = array(
    '#type' => 'select',
    '#title' => t('Blur factor'),
    '#description' => t('Degree of blur of an image. A higher value means more blur.'),
    '#options' => array(
      '1'   => '1',
      '1.5' => t('1.5'),
      '2'   => '2',
      '2.5' => t('2.5'),
      '3'   => '3',
      '3.5' => t('3.5'),
      '4'   => '4',
      '4.5' => t('4.5'),
      '5'   => '5'
    ),
    '#default_value' => variable_get('pirobox_blurgauss_factor', 2),
    '#states' => array(
      'visible' => array(
        ':input[name="pirobox_image_effect"]' => array('value' => 'blurgauss')
      )
    )
  );
  $form['pirobox_custom_settings']['pirobox_lightbox_image_effects']['pirobox_blurgauss_time'] = array(
    '#type' => 'select',
    '#title' => t('Blur time'),
    '#description' => t('As long as you can see the blur. A higher value means longer time.'),
    '#options' => drupal_map_assoc(array(1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000)),
    '#default_value' => variable_get('pirobox_blurgauss_time', 3000),
    '#states' => array(
      'visible' => array(
        ':input[name="pirobox_image_effect"]' => array('value' => 'blurgauss')
      )
    )
  );

  // Slideshow settings.
  $form['pirobox_custom_settings']['pirobox_slideshow_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slideshow settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['pirobox_custom_settings']['pirobox_slideshow_settings']['pirobox_slideshow'] = array(
    '#type' => 'radios',
    '#title' => t('Slideshow'),
    '#description' => t('A slideshow for image groups.'),
    '#options' => array(
      FALSE => t('Off'),
      TRUE => t('On')
    ),
    '#default_value' => variable_get('pirobox_slideshow', FALSE)
  );
  $form['pirobox_custom_settings']['pirobox_slideshow_settings']['pirobox_slideshowspeed'] = array(
    '#type' => 'select',
    '#title' => t('Slideshow speed'),
    '#description' => t('Sets the speed of the slideshow, in seconds.'),
    '#options' => drupal_map_assoc(array(4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30)),
    '#default_value' => variable_get('pirobox_slideshowspeed', 6),
    '#states' => array(
      'visible' => array(
        ':input[name="pirobox_slideshow"]' => array('value' => '1')
      )
    )
  );
  // Advanced settings.
  $form['pirobox_advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $caption_trim_description = t('If the caption should be made shorter in the Pirobox to avoid layout problems.');
  if ($eim_module == FALSE) {
    $caption_trim_description .= '<br />';
    $caption_trim_description .= t('With the !eim-url module it is possible, to extend the allowed text-length of the <em>Alt</em> field of images.', array('!eim-url' => l(t('EIM'), 'http://http://drupal.org/project/eim')));
  }
  $form['pirobox_advanced_settings']['pirobox_caption_trim'] = array(
    '#type' => 'radios',
    '#title' => t('Caption shortening'),
    '#description' => $caption_trim_description,
    '#options' => array(0 => t('Default'), 1 => t('Yes')),
    '#default_value' => variable_get('pirobox_caption_trim', 0)
  );

  $caption_trim_length_options = drupal_map_assoc(array(40, 45, 50, 55, 60, 70, 75));
  if ($eim_module == TRUE) {
    $caption_trim_length_options = drupal_map_assoc(array(40, 45, 50, 55, 60, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125));
  }

  $form['pirobox_advanced_settings']['pirobox_caption_trim_length'] = array(
    '#type' => 'select',
    '#title' => t('Caption trim length'),
    '#options' => $caption_trim_length_options,
    '#default_value' => variable_get('pirobox_caption_trim_length', 75),
    '#states' => array(
      'visible' => array(
        ':input[name="pirobox_caption_trim"]' => array('value' => '1')
      )
    )
  );
  $form['pirobox_advanced_settings']['pirobox_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Deactivate Pirobox on specific pages'),
    '#description' => t("Enter one page per line as Drupal path. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#default_value' => variable_get('pirobox_pages', "admin*\nimg_assist*\nimce*\nnode/add/*\nnode/*/edit")
  );

  return system_settings_form($form);
}

/**
 * Check the Pirobox and Blur gaussian jQuery plugin installation.
 *
 * Checks if the directory in $form_element exists and contains
 *   - the Pirobox JS files
 *     - pirobox.js
 *     - pirobox.min.js
 *   - the Pirobox CSS files
 *     - piroboxstyle.css
 *       At least one style folder with this file file must exist.
 *   - the Pirobox overlay background images
 *   - the Blur gaussian JS files
 *     - blur-gaussian.js
 *     - blur-gaussian.min.js
 *
 * If validation fails, the form element is flagged with an error from within
 * the file_check_directory function.
 *
 * @param $form_element
 *   The string containing the name of the directory to check and
 *   a bit information of the missing plugin parts.
 *
 * @see pirobox_general_settings_form()
 */
function _pirobox_admin_settings_check_plugin_path($form_element) {
  $library_path = $form_element['#value'];
  $error = FALSE;

  switch ($form_element['#value']) {
    case 'pirobox_path':
      $error_message = t('You need to move all content of the Pirobox jQuery plugin from the module folder <em>pirobox/move_to_libraries</em> to the %path folder of your server.', array('%path' => $library_path));

      // Check Pirobox JS files.
      if (!is_dir($library_path) || !(file_exists($library_path . '/js/pirobox.js') && file_exists($library_path . '/js/pirobox.min.js'))) {
        $error = TRUE;
        $error_message .= '<br /> - ' . t('Missing Pirobox JS files.');
      }
      // Check Pirobox CCS files.
      $styles = _pirobox_get_styles();
      if (count($styles) == 0) {
        $error = TRUE;
        $error_message .= '<br /> - ' . t('Missing Pirobox styles/templates or it exist wrong CSS file names. The correct name of a plugin style CSS file is <em>piroboxstyle.css</em>.');
      }
      // Check Pirobox overlay background images.
      $background_images = _pirobox_get_background_images();
      if (count($background_images) == 0 || (count($background_images) == 1 && isset($background_images['none']))) {
        $error = TRUE;
        $error_message .= '<br /> - ' . t('Missing Pirobox overlay background images.');
      }

      if ($error == TRUE) {
        form_set_error($form_element['#parents'][0], $error_message);
      }

      break;

    case 'blur_gaussian_path':
      $error_message = t('You need to move all content of the Pirobox jQuery plugin from the module folder <em>pirobox/move_to_libraries/blur-gaussian</em> to the %path folder of your server.', array('%path' => $library_path));

      // Check Blur gaussian JS files.
      if (!is_dir($library_path) || !(file_exists($library_path . '/blur-gaussian.js') && file_exists($library_path . '/blur-gaussian.min.js'))) {
        $error = TRUE;
        $error_message .= '<br /> - ' . t('Missing Blur gaussian JS files.');
      }

      if ($error == TRUE) {
        form_set_error($form_element['#parents'][0], $error_message);
      }

      break;
  }

  return $form_element;
}
