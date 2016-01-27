<?php
/**
 * @file
 * The default options available with Views jQFX: imageflow.
 */

/**
 * Implements hook_views_jqfx_jqfx_types().
 */
function views_jqfx_imageflow_views_jqfx_jqfx_types() {
  $options = array(
    'views_jqfx_imageflow' => t('Imageflow'),
  );
  return $options;
}

/**
 * Implements hook_views_jqfx_option_definition().
 */
function views_jqfx_imageflow_views_jqfx_option_definition() {
  // Set our default options.
  $options['views_jqfx_imageflow'] = array(
    'contains' => array(
      // Custom style options
      'custom_styles'      => array('default' => FALSE),
      'text_color'         => array('default' => '#FFFFFF'),
      'background_color'   => array('default' => '#000000'),

      // ImageFlow options
      'animation_speed'    => array('default' => '50'),
      'aspect_ratio'       => array('default' => '1.964'),
      'buttons'            => array('default' => FALSE),
      'captions'           => array('default' => TRUE),
      'circular'           => array('default' => FALSE),
      'glide_to_start_id'  => array('default' => TRUE),
      'image_cursor'       => array('default' => 'pointer'),
      'image_focus_m'      => array('default' => '1.0'),
      'image_focus_max'    => array('default' => '4'),
      'image_path'         => array('default' => '../../../..'),
      'image_scaling'      => array('default' => TRUE),
      'images_height'      => array('default' => '0.67'),
      'images_m'           => array('default' => '1.0'),
      'on_click'           => array('default' => NULL),
      'opacity'            => array('default' => FALSE),
      'opacity_array'      => array('default' => '[10,8,6,4,2]'),
      'percent_landscape'  => array('default' => '118'),
      'percent_other'      => array('default' => '100'),
      'preload_images'     => array('default' => TRUE),
      'reflections'        => array('default' => TRUE),
      'reflection_get'     => array('default' => '&cache=true'),
      'reflection_p'       => array('default' => NULL),
      'reflection_png'     => array('default' => FALSE),
      'reflect_path'       => array('default' => '/sites/all/libraries/ImageFlow/'),
      'scrollbar_p'        => array('default' => '0.6'),
      'slider'             => array('default' => TRUE),
      'slider_cursor'      => array('default' => 'e-resize'),
      'slider_width'       => array('default' => '14'),
      'slideshow'          => array('default' => FALSE),
      'slideshow_autoplay' => array('default' => FALSE),
      'slideshow_speed'    => array('default' => 1500),
      'start_id'           => array('default' => '1'),
      'start_animation'    => array('default' => FALSE),
      'x_step'             => array('default' => '150'),
    ),
  );
  return $options;
}

/**
 * Implements hook_views_jqfx_views_jqfx_type_form().
 */
function views_jqfx_imageflow_views_jqfx_jqfx_type_form(&$form, &$form_state, &$view) {
  ctools_include('dependent');
  if (!file_exists('sites/all/libraries/ImageFlow/imageflow.packed.js')) {
    $form['views_jqfx_imageflow']['no_imageflow_js'] = array(
      '#markup' => '<div style="color: red">' . t('You need to download ImageFlow plugin and copy it to sites/all/libraries/ImageFlow. You can find the plugin at !url.', array('!url' => l(t('http://finnrudolph.de/ImageFlow/Download', array(), array('langcode' => 'en')), 'http://finnrudolph.de/ImageFlow/Download', array('attributes' => array('target' => '_blank')))), array('langcode' => 'en')) . '</div>',
    );
  }

  $form['views_jqfx_imageflow']['custom_styles'] = array(
    '#type' => 'select',
    '#title' => t('Custom colors'),
    '#description' => t('This option enables setting the background and text colors for the ImageFlow.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['custom_styles'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_imageflow']['text_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Text color'),
    '#description' => t("The color of the caption and loader text. You can use any css valid color. Example: '#FFFFFF' or 'white'."),
    '#default_value' => $view->options['views_jqfx_imageflow']['text_color'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-custom-styles' => array(TRUE)),
  );
  $form['views_jqfx_imageflow']['background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => t("The background color for the ImageFlow stage. You can use any css valid color. Example: '#000000' or 'black'."),
    '#default_value' => $view->options['views_jqfx_imageflow']['background_color'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-custom-styles' => array(TRUE)),
  );
  $form['views_jqfx_imageflow']['animation_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation speed'),
    '#description' => t('Animation speed in ms.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['animation_speed'],
  );
  $form['views_jqfx_imageflow']['aspect_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Aspect ratio'),
    '#description' => t('Aspect ratio of the ImageFlow container (width divided by height).'),
    '#default_value' => $view->options['views_jqfx_imageflow']['aspect_ratio'],
  );
  $form['views_jqfx_imageflow']['buttons'] = array(
    '#type' => 'select',
    '#title' => t('Buttons'),
    '#description' => t('Toggles navigation buttons.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['buttons'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['captions'] = array(
    '#type' => 'select',
    '#title' => t('Display captions'),
    '#description' => t('Disables / enables the display of captions.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['captions'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['circular'] = array(
    '#type' => 'select',
    '#title' => t('Circular'),
    '#description' => t('Toggles circular rotation.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['circular'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['glide_to_start_id'] = array(
    '#type' => 'select',
    '#title' => t('Glide to start id'),
    '#description' => t('Toggle glide animation to start ID.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['glide_to_start_id'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['image_cursor'] = array(
    '#type' => 'select',
    '#title' => t('Image cursor'),
    '#description' => t('Cursor type for the images on mouse hover.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['image_cursor'],
    '#options' => array(
      'pointer' => t('pointer'),
      'default' => t('default'),
      'e-resize' => t('e-resize'),
    ),
  );
  $form['views_jqfx_imageflow']['image_focus_m'] = array(
    '#type' => 'textfield',
    '#title' => t('Image focus multiplier'),
    '#description' => t('Multiplier for the focussed image size.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['image_focus_m'],
  );
  $form['views_jqfx_imageflow']['image_focus_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Image focus max'),
    '#description' => t('Maximum number of images on each side of the focussed one.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['image_focus_max'],
  );
  $form['views_jqfx_imageflow']['image_scaling'] = array(
    '#type' => 'select',
    '#title' => t('Image scaling'),
    '#description' => t('Toggle image scaling.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['image_scaling'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['images_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Images height'),
    '#description' => t('Height of the images div container to be multiplied.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['images_height'],
  );
  $form['views_jqfx_imageflow']['images_m'] = array(
    '#type' => 'textfield',
    '#title' => t('Images multiplicator'),
    '#description' => t('Multiplicator for all images in percent.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['images_m'],
  );
  $form['views_jqfx_imageflow']['on_click'] = array(
    '#type' => 'textarea',
    '#title' => t('onClick override'),
    '#description' => t("You may override the onClick JavaScript function of slide show image frames here. This can be useful for opening links in another window or in a colorbox if that module is installed. Colorbox example code: 'function() {jQuery.colorbox({href:this.url,title:this.title});}'."),
    '#default_value' => $view->options['views_jqfx_imageflow']['on_click'],
  );
  $form['views_jqfx_imageflow']['opacity'] = array(
    '#type' => 'select',
    '#title' => t('Opacity'),
    '#description' => t('Disables / enables image opacity.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['opacity'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['opacity_array'] = array(
    '#type' => 'textfield',
    '#title' => t('Opacity array'),
    '#description' => t('Image opacity range - first value is for the focussed image (0 = 100% opacity, 10 = 0% opacity).'),
    '#default_value' => $view->options['views_jqfx_imageflow']['opacity_array'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-opacity' => array(TRUE)),
  );
  $form['views_jqfx_imageflow']['percent_landscape'] = array(
    '#type' => 'textfield',
    '#title' => t('Percent landscape'),
    '#description' => t('Scale landscape format.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['percent_landscape'],
  );
  $form['views_jqfx_imageflow']['percent_other'] = array(
    '#type' => 'textfield',
    '#title' => t('Percent other'),
    '#description' => t('Scale portrait and square format.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['percent_other'],
  );
  $form['views_jqfx_imageflow']['preload_images'] = array(
    '#type' => 'select',
    '#title' => t('Preload images'),
    '#description' => t('Disables / enables the loading bar and image preloading.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['preload_images'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['reflection_p'] = array(
    '#type' => 'textfield',
    '#title' => t('Reflection percent'),
    '#description' => t('Reflection height in % of the source image (1.0 = 100%, 0 = 0%). This affects the vertical image alignment whether reflections are enabled or not. Example: !reflectionp.', array('!reflectionp' => l(t('reflectionP', array(), array('langcode' => 'en')), 'http://finnrudolph.de/ImageFlow/Examples#Image_alignment', array('attributes' => array('target' => '_blank')))), array('langcode' => 'en')),
    '#default_value' => $view->options['views_jqfx_imageflow']['reflection_p'],
  );
  $form['views_jqfx_imageflow']['reflections'] = array(
    '#type' => 'select',
    '#title' => t('Reflections'),
    '#description' => t('Disables / enables the display of reflections.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['reflections'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['reflection_get'] = array(
    '#type' => 'textfield',
    '#title' => t('Reflection get'),
    '#description' => t("Passes variables via the GET method to the reflect2.php and reflect3.php script. This allows setting reflection options. For instance, inputting '&bgc=ffffff&fade_start=20%' would set the reflection background color to white and fade start to 20%. The full list of options for the reflect2 and reflect3 scripts is listed in the README file."),
    '#default_value' => $view->options['views_jqfx_imageflow']['reflection_get'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-reflections' => array(TRUE)),
  );
  $form['views_jqfx_imageflow']['reflection_png'] = array(
    '#type' => 'select',
    '#title' => t('Reflection png'),
    '#description' => t("Switches from the reflect2.php to the reflect3.php for PNG transparency support. Must use PNG images with this option."),
    '#default_value' => $view->options['views_jqfx_imageflow']['reflection_png'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-reflections' => array(TRUE)),
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['reflect_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Reflect path'),
    '#description' => t('Path to the reflect.php file for the image reflections. Leave to default unless you have a reason to change it'),
    '#default_value' => $view->options['views_jqfx_imageflow']['reflect_path'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-reflections' => array(TRUE)),
  );
  $form['views_jqfx_imageflow']['image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path'),
    '#description' => t('Path to the images relative to the location of the reflect.php script. Leave to default unless you have a reason to change it'),
    '#default_value' => $view->options['views_jqfx_imageflow']['image_path'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-reflections' => array(TRUE)),
  );
  $form['views_jqfx_imageflow']['slider'] = array(
    '#type' => 'select',
    '#title' => t('Show slider'),
    '#description' => t('Disables / enables the scrollbar.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['slider'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['scrollbar_p'] = array(
    '#type' => 'textfield',
    '#title' => t('Scrollbar width multiplier'),
    '#description' => t('Multiplier width of the scrollbar (0-1).'),
    '#default_value' => $view->options['views_jqfx_imageflow']['scrollbar_p'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-slider' => array(TRUE)),
  );
  $form['views_jqfx_imageflow']['slider_cursor'] = array(
    '#type' => 'select',
    '#title' => t('Slider cursor'),
    '#description' => t('Cursor type for the slider on mouse hover.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['slider_cursor'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-slider' => array(TRUE)),
    '#options' => array(
      'pointer' => t('pointer'),
      'default' => t('default'),
      'e-resize' => t('e-resize'),
    ),
  );
  $form['views_jqfx_imageflow']['slider_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Slider width'),
    '#description' => t('Width of the slider in px.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['slider_width'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-slider' => array(TRUE)),
  );
  $form['views_jqfx_imageflow']['slideshow'] = array(
    '#type' => 'select',
    '#title' => t('Slideshow'),
    '#description' => t('Enables an slideshow mode.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['slideshow'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['slideshow_autoplay'] = array(
    '#type' => 'select',
    '#title' => t('Slideshow autoplay'),
    '#description' => t('If true the slideshow will be in play mode when the page loads. If this is false and navigation buttons are enabled, it will throw an error in internet explorer when trying to click a button. This is an Imageflow bug.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['slideshow_autoplay'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-slideshow' => array(TRUE)),
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['slideshow_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Slideshow speed'),
    '#description' => t('Time between slides in milliseconds.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['slideshow_speed'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-imageflow-slideshow' => array(TRUE)),
  );
  $form['views_jqfx_imageflow']['start_animation'] = array(
    '#type' => 'select',
    '#title' => t('Start animation'),
    '#description' => t('Animate images moving in from the right on startup.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['start_animation'],
    '#options' => array(
      FALSE => t('False'),
      TRUE => t('True'),
    ),
  );
  $form['views_jqfx_imageflow']['start_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Start id'),
    '#description' => t('Glide to this image number on startup.'),
    '#default_value' => $view->options['views_jqfx_imageflow']['start_id'],
  );
  $form['views_jqfx_imageflow']['x_step'] = array(
    '#type' => 'textfield',
    '#title' => t('X step'),
    '#description' => t('Step width on the x-axis in px. This affects the horizontal distance between the images'),
    '#default_value' => $view->options['views_jqfx_imageflow']['x_step'],
  );
}
