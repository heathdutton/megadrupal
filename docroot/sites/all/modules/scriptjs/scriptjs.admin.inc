<?php
/**
 * @file
 * Script.js admin form. 
 */

/**
 * Form callback.
 * 
 * @see scriptjs_menu(). 
 */
function scriptjs_settings_form() {
  $form = array();

  // Clear javascript cache when settings are updated.
  $form['#submit'][] = 'drupal_clear_js_cache';

  $form['scriptjs_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable $script.js'),
    '#description' => t('$script.js is an async JavaScript loader and dependency manager'),
    '#default_value' => variable_get('scriptjs_enable', TRUE),
  );
  
  $form['scriptjs_disable_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable $script.js in Admin Pages'),
    '#description' => t('Check this option if you are having problems with some complex administration pages such as Views.'),
    '#default_value' => variable_get('scriptjs_disable_admin', TRUE),
    '#states' => array(
      'visible' => array(
        'input[name="scriptjs_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['scriptjs_min'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use minified $script.js library'),
    '#default_value' => variable_get('scriptjs_min', TRUE),
    '#description' => t('You should always use the minified version in production sites. Use the regular version only when debugging.'),
  );
  
  $form['scriptjs_embed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Embed script instead of including'),
    '#default_value' => variable_get('scriptjs_embed', FALSE),
    '#description' => t('Embed $script.js inline instead of including it externally for an even greater performance boost.'),
  );
  
  $form['scriptjs_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('URLs to exclude/include'),
    '#cols' => 60,
    '#rows' => 5,
    '#default_value' => variable_get('scriptjs_exclude', ''),
    '#description' => t('Exclude $script.js from the following paths. Put each path on a separate line. You can use the "*" character as a wildcard. Use &lt;front&gt; for the site front page.'),
    '#wysiwyg' => FALSE
  );

  return system_settings_form($form);
}