<?php

/**
 * @file
 * Administration forms for the jQuery Nicescroll module.
 */

/**
 * Form constructor for jQuery Nicescroll settings form.
 *
 * @todo Figure out railoffset parameter
 *
 * @ingroup forms
 */
function jquery_nicescroll_settings($form, &$form_state) {
  $library = libraries_detect('jquery.nicescroll');

  if ($library['installed']) {
    $params = variable_get('jquery_nicescroll_parameters', jquery_nicescroll_default_parameters());

    $form['jquery_nicescroll_library'] = array(
      '#type' => 'radios',
      '#title' => t('jQuery Nicescroll Library'),
      '#default_value' => variable_get('jquery_nicescroll_library', 'minified'),
      '#options' => array(
        'minified' => t('Minified'),
        'source' => t('Source/Uncompressed'),
      ),
    );

    $form['jquery_nicescroll_element'] = array(
      '#type' => 'textfield',
      '#title' => t('Element'),
      '#default_value' => variable_get('jquery_nicescroll_element', 'html'),
      '#description' => t('A comma-separated list of jQuery selectors to apply jQueryNicescroll plugin.'),
    );

    $form['jquery_nicescroll_parameters'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configuration parameters'),
      '#collapsible' => TRUE,
      '#tree' => TRUE,
    );
    $form['jquery_nicescroll_parameters']['cursorcolor'] = array(
      '#type' => 'textfield',
      '#title' => t('Cursor Color'),
      '#description' => t('Change cursor color in hex'),
    );
    $form['jquery_nicescroll_parameters']['cursoropacitymin'] = array(
      '#type' => 'numberfield',
      '#title' => t('Minimum Cursor Opacity'),
      '#description' => t('Change opacity when cursor is inactive (scrollbar <strong>hidden</strong> state), range from 1 to 0'),
      '#step' => 0.01,
      '#min' => 0,
      '#max' => 1,
    );
    $form['jquery_nicescroll_parameters']['cursoropacitymax'] = array(
      '#type' => 'numberfield',
      '#title' => t('Maximum Cursor Opacity'),
      '#description' => t('Change opacity when cursor is active (scrollbar <strong>visible</strong> state), range from 1 to 0'),
      '#step' => 0.01,
      '#min' => 0,
      '#max' => 1,
    );
    $form['jquery_nicescroll_parameters']['cursorwidth'] = array(
      '#type' => 'numberfield',
      '#title' => t('Cursor Width'),
      '#description' => t('Cursor width in pixel'),
    );
    $form['jquery_nicescroll_parameters']['cursorborder'] = array(
      '#type' => 'textfield',
      '#title' => t('Cursor Border'),
      '#description' => t('CSS definition for cursor border'),
    );
    $form['jquery_nicescroll_parameters']['cursorborderradius'] = array(
      '#type' => 'textfield',
      '#title' => t('Cursor Border Radius'),
      '#description' => t('Border radius in pixel for cursor'),
    );
    $form['jquery_nicescroll_parameters']['zindex'] = array(
      '#type' => 'numberfield',
      '#title' => t('Z-Index'),
      '#description' => t('Change z-index for scrollbar div'),
    );
    $form['jquery_nicescroll_parameters']['scrollspeed'] = array(
      '#type' => 'numberfield',
      '#title' => t('Scroll Speed'),
      '#description' => t('Scrolling speed'),
    );
    $form['jquery_nicescroll_parameters']['mousescrollstep'] = array(
      '#type' => 'numberfield',
      '#title' => t('Mouse Scroll Step'),
      '#description' => t('Scrolling speed with mouse wheel'),
    );
    $form['jquery_nicescroll_parameters']['touchbehavior'] = array(
      '#type' => 'radios',
      '#title' => t('Touch Behavior'),
      '#description' => t('Enable cursor-drag scrolling like touch devices in desktop computer'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['hwacceleration'] = array(
      '#type' => 'radios',
      '#title' => t('Hardware Acceleration'),
      '#description' => t('Use hardware accelerated scroll when supported'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['boxzoom'] = array(
      '#type' => 'radios',
      '#title' => t('Box Zoom'),
      '#description' => t('Enable zoom for box content'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['dblclickzoom'] = array(
      '#type' => 'radios',
      '#title' => t('Double Click Zoom'),
      '#description' => t('Zoom activated when double click on box'),
      '#options' => array('Off', 'On'),
      '#states' => array(
        'visible' => array(
          ':input[name="jquery_nicescroll_parameters[boxzoom]"]' => array('value' => 1),
        ),
      ),
    );
    $form['jquery_nicescroll_parameters']['gesturezoom'] = array(
      '#type' => 'radios',
      '#title' => t('Gesture Zoom'),
      '#description' => t('Zoom activated when pitch out/in on box'),
      '#options' => array('Off', 'On'),
      '#states' => array(
        'visible' => array(
          ':input[name="jquery_nicescroll_parameters[boxzoom]"]' => array('value' => 1),
        ),
      ),
    );
    $form['jquery_nicescroll_parameters']['grabcursorenabled'] = array(
      '#type' => 'radios',
      '#title' => t('Grab Cursor'),
      '#description' => t('Display <strong>grab</strong> icon for div with touchbehavior'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['autohidemode'] = array(
      '#type' => 'select',
      '#title' => t('Auto Hide Mode'),
      '#description' => t('How hide the scrollbar works'),
      '#options' => array(
        TRUE => t('Default'),
        'cursor' => t('Only Cursor Hidden'),
        FALSE => t('Do not hide'),
      ),
    );
    $form['jquery_nicescroll_parameters']['background'] = array(
      '#type' => 'textfield',
      '#title' => t('Background'),
      '#description' => t('Change CSS for rail background'),
    );
    $form['jquery_nicescroll_parameters']['iframeautoresize'] = array(
      '#type' => 'radios',
      '#title' => t('Iframe Auto Resize'),
      '#description' => t('Autoresize iframe on load event'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['cursorminheight'] = array(
      '#type' => 'numberfield',
      '#title' => t('Minimum Cursor Height'),
      '#description' => t('Set the minimum cursor height in pixel'),
    );
    $form['jquery_nicescroll_parameters']['preservenativescrolling'] = array(
      '#type' => 'radios',
      '#title' => t('Preserve Native Scrolling'),
      '#description' => t('You can scroll native scrollable areas with mouse, bubbling mouse wheel event'),
      '#options' => array('Off', 'On'),
    );
    // $form['jquery_nicescroll_parameters']['railoffset'] = array();
    $form['jquery_nicescroll_parameters']['bouncescroll'] = array(
      '#type' => 'radios',
      '#title' => t('Bounce Scroll'),
      '#description' => t('Enable scroll bouncing at the end of content as mobile-like'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['spacebarenabled'] = array(
      '#type' => 'radios',
      '#title' => t('Space Bar Enabled'),
      '#description' => t('Enable page down scrolling when space bar has pressed'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['railpadding'] = array();
    $form['jquery_nicescroll_parameters']['railpadding']['top'] = array(
      '#type' => 'numberfield',
      '#title' => t('Rail Padding Top'),
      '#description' => t('Set top padding for rail bar'),
      '#default_value' => $params['railpadding']['top'],
    );
    $form['jquery_nicescroll_parameters']['railpadding']['right'] = array(
      '#type' => 'numberfield',
      '#title' => t('Rail Padding Right'),
      '#description' => t('Set right padding for rail bar'),
      '#default_value' => $params['railpadding']['right'],
    );
    $form['jquery_nicescroll_parameters']['railpadding']['bottom'] = array(
      '#type' => 'numberfield',
      '#title' => t('Rail Padding Bottom'),
      '#description' => t('Set bottom padding for rail bar'),
      '#default_value' => $params['railpadding']['bottom'],
    );
    $form['jquery_nicescroll_parameters']['railpadding']['left'] = array(
      '#type' => 'numberfield',
      '#title' => t('Rail Padding Left'),
      '#description' => t('Set left padding for rail bar'),
      '#default_value' => $params['railpadding']['left'],
    );
    $form['jquery_nicescroll_parameters']['disableoutline'] = array(
      '#type' => 'radios',
      '#title' => t('Disable Outline'),
      '#description' => t('For chrome browser, disable outline (orange hightlight) when selecting a div with nicescroll'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['horizrailenabled'] = array(
      '#type' => 'radios',
      '#title' => t('Horizontal Rail Enabled'),
      '#description' => t('Nicescroll can manage horizontal scroll'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['railalign'] = array(
      '#type' => 'select',
      '#title' => t('Rail Align'),
      '#description' => t('Alignment of horizontal rail'),
      '#options' => array(
        'left' => t('Left'),
        'right' => t('Right'),
      ),
    );
    $form['jquery_nicescroll_parameters']['railvalign'] = array(
      '#type' => 'select',
      '#title' => t('Rail Align'),
      '#description' => t('Alignment of vertical rail'),
      '#options' => array(
        'top' => t('Top'),
        'bottom' => t('Bottom'),
      ),
    );
    $form['jquery_nicescroll_parameters']['enabletranslate3d'] = array(
      '#type' => 'radios',
      '#title' => t('Enable Translate 3D'),
      '#description' => t('Nicescroll can use CSS translate to scroll content'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['enablemousewheel'] = array(
      '#type' => 'radios',
      '#title' => t('Enable Mouse Wheel'),
      '#description' => t('Nicescroll can manage mouse wheel events'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['enablekeyboard'] = array(
      '#type' => 'radios',
      '#title' => t('Enable Keyboard'),
      '#description' => t('Nicescroll can manage keyboard events'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['smoothscroll'] = array(
      '#type' => 'radios',
      '#title' => t('Smooth Scroll'),
      '#description' => t('Scroll with ease movement'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['sensitiverail'] = array(
      '#type' => 'radios',
      '#title' => t('Sensitive Rail'),
      '#description' => t('Click on rail make a scroll'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['enablemouselockapi'] = array(
      '#type' => 'radios',
      '#title' => t('Enable Mouse Lock API'),
      '#description' => t('Can use mouse caption lock API'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['cursorfixedheight'] = array(
      '#type' => 'numberfield',
      '#title' => t('Cursor Fixed Height'),
      '#description' => t('Set fixed height for cursor in pixel (0 = auto)'),
      '#default_value' => 0,
    );
    $form['jquery_nicescroll_parameters']['hidecursordelay'] = array(
      '#type' => 'numberfield',
      '#title' => t('Hide Cursor Delay'),
      '#description' => t('Set the delay in microseconds to fading out scrollbars'),
    );
    $form['jquery_nicescroll_parameters']['directionlockdeadzone'] = array(
      '#type' => 'numberfield',
      '#title' => t('Direction Lock Dead Zone'),
      '#description' => t('Dead zone in pixels for direction lock activation'),
    );
    $form['jquery_nicescroll_parameters']['nativeparentscrolling'] = array(
      '#type' => 'radios',
      '#title' => t('Native Parent Scrolling'),
      '#description' => t('Detect bottom of content and let parent to scroll, as native scroll does'),
      '#options' => array('Off', 'On'),
    );
    $form['jquery_nicescroll_parameters']['enablescrollonselection'] = array(
      '#type' => 'radios',
      '#title' => t('Enable Scroll on Selection'),
      '#description' => t('Enable auto-scrolling of content when selection text'),
      '#options' => array('Off', 'On'),
    );

    foreach ($params as $param => $value) {
      if ($param != 'railpadding') {
        if ($form['jquery_nicescroll_parameters'][$param]['#type'] == 'radios') {
          settype($value, 'int');
        }
        $form['jquery_nicescroll_parameters'][$param]['#default_value'] = $value;
      }
    }
    if (is_bool($form['jquery_nicescroll_parameters']['autohidemode']['#default_value'])) {
      settype($form['jquery_nicescroll_parameters']['autohidemode']['#default_value'], 'int');
    }

    $form['#submit'][] = 'jquery_nicescroll_settings_submit';
    return system_settings_form($form);
  }
  else {
    drupal_set_message($library['error message'], 'error');
  }
}

/**
 * Form submission handler for jquery_nicescroll_settings().
 */
function jquery_nicescroll_settings_submit($form, &$form_state) {
  $element = &$form_state['values']['jquery_nicescroll_element'];
  $element = (trim($element)) ? trim($element) : 'html';

  $params = &$form_state['values']['jquery_nicescroll_parameters'];

  // Define which parameters should be set to boolean when adding javascript
  // settings.
  $boolean = array(
    'touchbehavior',
    'hwacceleration',
    'boxzoom',
    'dblclickzoom',
    'gesturezoom',
    'grabcursorenabled',
    'iframeautoresize',
    'preservenativescrolling',
    'bouncescroll',
    'spacebarenabled',
    'disableoutline',
    'horizrailenabled',
    'enabletranslate3d',
    'enablemousewheel',
    'enablekeyboard',
    'smoothscroll',
    'sensitiverail',
    'enablemouselockapi',
    'nativeparentscrolling',
    'enablescrollonselection',
  );

  // Define which parameters should be set to float when adding javascript
  // settings.
  $float = array(
    'cursoropacitymin',
    'cursoropacitymax',
    'cursorwidth',
    'zindex',
    'scrollspeed',
    'mousescrollstep',
    'cursorminheight',
    // 'railoffset',
    'cursorfixedheight',
    'hidecursordelay',
    'directionlockdeadzone',
  );

  // Set the type of parameter to its the right type.
  foreach ($boolean as $param) {
    settype($params[$param], 'bool');
  }
  foreach ($float as $param) {
    settype($params[$param], 'float');
  }
  foreach ($params['railpadding'] as $param => $padding) {
    settype($params['railpadding'][$param], 'float');
  }
  if ($params['autohidemode'] != 'cursor') {
    settype($params['autohidemode'], 'bool');
  }
  if (!$params['cursorfixedheight']) {
    settype($params['cursorfixedheight'], 'bool');
  }
}
