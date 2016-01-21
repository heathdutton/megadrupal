<?php

/**
 * @file
 * The admin form include file
 *
 * Defines the FAPI object and renders the system settings form for
 * the History.js Library Module
 */

/**
 * Defines the admin setings form
 *
 * @return
 *   The admin form rendered through system_settings_form
 */
function history_js_admin() {
  $form = array();
  $form['history_js_compatibility'] = array(
    '#type' => 'select',
    '#title' => t('Compatibility Mode'),
    '#options' => array(
      'html4+html5' => t('HTML4 + HTML5 (default)'),
      'html5' => t('HTML 5')
    ),
    '#default_value' => variable_get('history_js_compatibility', 'html4+5'),
    '#description' => t('Choose which compatibility mode you would like to use. See <a href="@url">The History.js Wiki on Intelligent State Handling</a> for more information.', array('@url' => url('https://github.com/balupton/history.js/wiki/Intelligent-State-Handling'))),
  );
  $form['history_js_implementation'] =array(
    '#type' => 'select',
    '#title' => t('JS Library Implementation'),
    '#options' => array(
      'jquery' => t('jQuery (Default)'),
      /*
      // TODO: include support for more libraries (will require dependencies)
      'mootools' => l('MooTools','http://mootools.net'),
      'right' => l('Right','http://rightjs.org'),
      'zepto' => l('Zepto','http://zeptojs.com'),
      */
      'native' => t('Use the native History.js implementaion')
    ),
    '#default_value' => variable_get('history_js_implementation', 'jquery'),
    '#description' => t('Choose which JS framework you would like to implement.')
  );

  return system_settings_form($form);
}
