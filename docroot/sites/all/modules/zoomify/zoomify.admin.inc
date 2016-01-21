<?php

/**
 * @file
 * Provides administration pages for the Zoomify module
 */

/**
 * Form callback for Zoomify admin settings.
 */
function zoomify_admin_settings($form, &$form_state) {
  $options = array(
    ZOOMIFY_BACKEND_PHP => t('PHP (might not be suitable for very large images)'),
    ZOOMIFY_BACKEND_PYTHON => t('Python (requires <a href="@url">ZoomifyImage</a> Python package)', array('@url' => 'http://sourceforge.net/projects/zoomifyimage/')),
  );
  if (function_exists('zip_open')) {
    $options[ZOOMIFY_BACKEND_MANUAL] = t('None (node creator is responsible for uploading tiled images in a Zip archive)');
  }
  else {
    drupal_set_message(t('The option to upload your own tiles is disabled because the PHP Zip extension is not loaded.'), 'warning', FALSE);
  }
  $form['zoomify_backend'] = array(
    '#type' => 'radios',
    '#title' => t('Tiling backend'),
    '#options' => $options,
    '#default_value' => variable_get('zoomify_backend', ZOOMIFY_BACKEND_PHP),
  );
  $form['zoomify_frontend'] = array(
    '#type' => 'radios',
    '#title' => t('Zoomify frontend'),
    '#options' => array(
      ZOOMIFY_FRONTEND_FLASH => t('Flash (requires ZoomifyViewer.swf)'),
      ZOOMIFY_FRONTEND_HTML5 => t('HTML5 (requires ZoomifyImageViewer.js)'),
    ),
    '#default_value' => variable_get('zoomify_frontend', ZOOMIFY_FRONTEND_FLASH),
  );
  $form['zoomify_minimum_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum width'),
    '#default_value' => variable_get('zoomify_minimum_width', 1024),
    '#description' => t('Minimum image width that will be zoomified.'),
    '#field_suffix' => t('px'),
    '#size' => 12,
  );
  $form['zoomify_minimum_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum height'),
    '#default_value' => variable_get('zoomify_minimum_height', 768),
    '#description' => t('Minimum image height that will be zoomified.'),
    '#field_suffix' => t('px'),
    '#size' => 12,
  );
  $form['zoomify_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Frame width'),
    '#default_value' => variable_get('zoomify_width', '800'),
    '#description' => t('Width of the Zoomify frame.'),
    '#field_suffix' => t('px'),
    '#size' => 12,
  );
  $form['zoomify_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Frame height'),
    '#default_value' => variable_get('zoomify_height', '600'),
    '#description' => t('Height of the Zoomify frame.'),
    '#field_suffix' => t('px'),
    '#size' => 12,
  );
  $form['zoomify_fixframe'] = array(
    '#title' => t('Fix frame size'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('zoomify_fixframe', TRUE),
    '#description' => t('Check this OFF to allow the Zoomify Viewer frame to be resized to fit the image\'s aspect ratio.'),
  );
  $form['zoomify_showtab'] = array(
    '#title' => t('Show tab'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('zoomify_showtab', TRUE),
    '#description' => t('Check this OFF if you are showing the Zoomify applet in your default view and thus don\'t need the extra tab.'),
  );
  $form['zoomify_types'] = array(
    '#title' => t('Content types'),
    '#type' => 'checkboxes',
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('zoomify_types', array()),
    '#description' => t('Content types that the module will intercept to check for images.'),
  );
  // @TODO: reimplement Token support for D7
  /**if (module_exists('token')) {
    // TODO Please change this theme call to use an associative array for the $variables parameter.
    $token = t('<p>You can also use tokens with the FlashVars: <fieldset class="collapsed collapsible"><legend>Available tokens</legend>!tokens</fieldset>', array('!tokens' => theme('token_help', array('node', 'global'))));
    drupal_add_js('misc/collapse.js');
  }
  else {
    $token = '';
  }*/
  $token = '';
  $form['zoomify_flashvars'] = array(
    '#title' => t('Extra options'),
    '#type' => 'textarea',
    '#description' => t('Additional options passed to the Zoomify viewer (Flash or HTML5), separated by ampersands (&). For example: <code>zoomifyNavigatorVisible=false</code>.
                         The full list of variables is poorly documented but you can <a href="@faq">find it here</a>.', array('@faq' => 'http://www.zoomify.com/support.htm#a20081222_2050')) . $token,
    '#default_value' => variable_get('zoomify_flashvars', NULL),
  );

  return system_settings_form($form);
}
