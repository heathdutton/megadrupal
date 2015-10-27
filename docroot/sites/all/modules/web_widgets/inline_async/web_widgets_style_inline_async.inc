<?php
/**
 * @file
 * Inline-embedding specific functions, preprocessing
 */

/**
 * Define the theme functions for the web_widgets module
 *
 * @return unknown
 */
function web_widgets_style_inline_async() {
  $definition = array(
    'theme' => array(
      // The HTML-Javascript code lines what should be pasted into the external site
      'web_widgets_inline_async' => array(
        'template' => 'web_widgets_inline_async',
        'arguments' => array(
          'path' => NULL,
          'width' => NULL,
          'height' => NULL,
        ),
        'path' => drupal_get_path('module', 'web_widgets') . '/inline_async',
      ),
      // Possible wrapper code around the widget
      'web_widgets_inline_async_wrapper' => array(
        'template' => 'web_widgets_inline_async_wrapper',
        'arguments' => array(
          'content' => NULL,
          'title' => NULL,
        ),
        'path' => drupal_get_path('module', 'web_widgets') . '/inline_async',
      ),
    ),
    'human_readable' => t('Inline (Async)'),
  );
  return $definition;
}

/**
 * Process variables for web_widgets_inline_async.tpl.php
 */
function template_preprocess_web_widgets_inline_async(&$variables) {
  $variables['wid'] = 'web_widgets_inline_async_' . md5($variables['path']);

  // Create Javascript variables
  $widgetcontext = new stdClass();
  $widgetcontext->widgetid = $variables['wid'];

  $variables['js_variables'] = drupal_json_encode($widgetcontext);
  $variables['script_url'] = $variables['path'];
}

/**
 * Process variables for web_widgets_inline_async_wrapper.tpl.php
 */
function template_preprocess_web_widgets_inline_async_wrapper(&$variables) {
  $variables['head'] = drupal_get_html_head();
  $variables['styles'] = drupal_get_css();
  $variables['scripts'] = drupal_get_js();
  $variables['content'] = $variables['styles'] . $variables['scripts'] . $variables['content'];
  $variables['js_string'] = drupal_json_encode($variables['content']);

  if (empty($variables['path'])) {
    global $base_url;
    $variables['path'] = $base_url . request_uri();
  }
  $variables['wid'] = 'web_widgets_inline_async_' . md5($variables['path']);
}
