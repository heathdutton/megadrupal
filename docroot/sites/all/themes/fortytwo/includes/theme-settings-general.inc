<?php

/**
 * @file
 * General theme settings elements.
 */

/**
 * Setup the theme settings form.
 */
function fortytwo_theme_settings_general(&$form, &$form_state) {
  drupal_add_css(fortytwo_get_staticpath() . 'css/admin.css');
  $form = _fortytwo_theme_basic($form);
  $form['fortytwo_settings'][] = _fortytwo_theme_get_layout();
  $form['fortytwo_settings'][] = _fortytwo_theme_get_javascript();
  $form['fortytwo_settings'][] = _fortytwo_theme_get_metatags();
  $form['fortytwo_settings'][] = _fortytwo_theme_get_debuggers();
  $form['fortytwo_settings'][] = _fortytwo_theme_get_breadcrumb();
}

/**
 * Basic form fields.
 */
function _fortytwo_theme_basic(&$form) {
  $ret = array();
  $ret['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
  );

  $form['theme_settings']['#prefix'] = _fortytwo_theme_create_prefix(t('Toggle display'));
  $form['logo']['#prefix'] = _fortytwo_theme_create_prefix(t('Logo image settings'));
  $form['favicon']['#prefix'] = _fortytwo_theme_create_prefix(t('Shortcut icon settings'));

  $ret['basic']['theme_settings'] = $form['theme_settings'];
  $ret['basic']['logo'] = $form['logo'];
  $ret['basic']['favicon'] = $form['favicon'];

  unset($form['theme_settings']);
  unset($form['logo']);
  unset($form['favicon']);

  $form['fortytwo_settings'][] = $ret;

  return $form;
}

/**
 * Layout form fields.
 */
function _fortytwo_theme_get_layout() {
  $ret = array();
  $ret['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
  );

  $ret['layout']['ft_layout_style'] = array(
    '#type' => 'radios',
    '#title' => t('Layout type'),
    '#description' => t('Pick a layout type you want to use.'),
    '#default_value' => theme_get_setting('ft_layout_style'),
    '#options' => array(
      'columns-left' => theme('image', array('path' => fortytwo_get_staticpath() . 'images/layout1.png')),
      'columns-middle' => theme('image', array('path' => fortytwo_get_staticpath() . 'images/layout2.png')),
      'columns-right' => theme('image', array('path' => fortytwo_get_staticpath() . 'images/layout3.png')),
      'columns-none' => theme('image', array('path' => fortytwo_get_staticpath() . 'images/layout4.png')),
    ),
  );

  $options = array(
    'layout-fixed' => t('Fixed (not responsive)'),
    'Responsiveness' => array(
      'layout-adaptive' => t('Adaptive'),
      'layout-fluid' => t('Fluid'),
    ),
  );

  $ret['layout']['ft_layout_responsive'] = array(
    '#type' => 'select',
    '#title' => t('Enable responsive styling'),
    '#default_value' => theme_get_setting('ft_layout_responsive'),
    '#options' => $options,
  );

  return $ret;
}

/**
 * Javascript form fields.
 */
function _fortytwo_theme_get_javascript() {
  $ret = array();
  $ret['js'] = array(
    '#type' => 'fieldset',
    '#title' => t('Javascript settings'),
  );
  $ret['js']['polyfills'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polyfills'),
    '#prefix' => _fortytwo_theme_create_prefix(t('Polyfills')),
  );
  $ret['js']['polyfills']['ft_placeholder_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable placeholder.js'),
    '#description' => t('Enable the placeholder.js javascript plugin'),
    '#default_value' => theme_get_setting('ft_placeholder_js'),
  );
  $ret['js']['libraries'] = array(
    '#type' => 'fieldset',
    '#title' => t('Libraries'),
    '#prefix' => _fortytwo_theme_create_prefix(t('Libraries')),
  );
  $link = l(t('enquire.js'), 'http://wicky.nillia.ms/enquire.js', array('attributes' => array('target' => '_blank')));
  $ret['js']['libraries']['ft_enquire_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable enquire.js'),
    '#description' => t('Enable the !link javascript plugin. This includes the matchMedia.js library', array('!link' => $link)),
    '#default_value' => theme_get_setting('ft_enquire_js'),
  );
  $link = l(t('lodash.js'), 'http://lodash.com', array('attributes' => array('target' => '_blank')));
  $ret['js']['libraries']['ft_lodash_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable lodash.js'),
    '#description' => t('Enable the !link javascript plugin', array('!link' => $link)),
    '#default_value' => theme_get_setting('ft_lodash_js'),
  );
  $link = l(t('transit.js'), 'http://ricostacruz.com/jquery.transit', array('attributes' => array('target' => '_blank')));
  $ret['js']['libraries']['ft_transit_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable transit.js'),
    '#description' => t('Enable the !link javascript plugin', array('!link' => $link)),
    '#default_value' => theme_get_setting('ft_transit_js'),
  );
  $ret['js']['libraries']['ft_prettify_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable form field prettyfier'),
    '#description' => t('Prettifies fileinputs and selectboxes.'),
    '#default_value' => theme_get_setting('ft_prettify_js'),
  );
  $ret['js']['libraries']['ft_prettify_js_hide_upload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide upload button on file fields'),
    '#description' => t('Since the files are uploaded when submitting a form, it is possible to hide the upload button.'),
    '#default_value' => theme_get_setting('ft_prettify_js_hide_upload'),
    '#states' => array(
      'visible' => array(
        ':input[name="ft_prettify_js"]' => array('checked' => TRUE),
      ),
    ),
  );
  $ret['js']['libraries']['ft_prettify_js_add_remove'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show clear button on file fields'),
    '#description' => t('After selecting a file a user needs to browser and cancel to deselect. The clear button will provide an easy way to clear the upload field.'),
    '#default_value' => theme_get_setting('ft_prettify_js_add_remove'),
    '#states' => array(
      'visible' => array(
        ':input[name="ft_prettify_js"]' => array('checked' => TRUE),
      ),
    ),
  );
  return $ret;
}

/**
 * Meta tags form fields.
 */
function _fortytwo_theme_get_metatags() {
  $ret = array();
  $ret['metatags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Metatags and HTTP headers'),
  );

  $ret['metatags']['ft_apple_touch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apple touch icons'),
    '#description' => t('You can set an Apple Touch Icon for your website just like you can set a favorite icon.'),
    '#default_value' => theme_get_setting('ft_apple_touch'),
  );

  $ret['metatags']['ft_cleartype'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cleartype support for Windows'),
    '#description' => t('Mobile Internet Explorer allows us to activate ClearType technology for smoothing fonts for easy reading.'),
    '#default_value' => theme_get_setting('ft_cleartype'),
  );

  $ret['metatags']['ft_viewport'] = array(
    '#type' => 'checkbox',
    '#title' => t('Viewport'),
    '#description' => t('This is more widely supported by modern smartphones, including but not limited to: iOS, Android, Palm Pre, Blackberry, Windows Phone 7.'),
    '#default_value' => theme_get_setting('ft_viewport'),
  );

  $ret['metatags']['viewport_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Viewport settings'),
    '#states' => array(
      'visible' => array(
        ':input[name="ft_viewport"]' => array('checked' => TRUE),
      ),
    ),
  );

  $options = array('_none' => t('Default'));
  foreach (range(0.1, 10, 0.1) as $value) {
    // Format the value to display with one decimal.
    $value = number_format($value, 1);
    $options[(string) $value] = $value;
  }

  $ret['metatags']['viewport_settings']['ft_viewport_initial_scale'] = array(
    '#type' => 'select',
    '#title' => t('Initial scale'),
    '#default_value' => theme_get_setting('ft_viewport_initial_scale'),
    '#options' => $options,
    '#description' => t('The initial scaling of the page. Has to be in the range of the minimum- and maximum scale options.'),
  );

  $ret['metatags']['viewport_settings']['ft_viewport_user_scaleable'] = array(
    '#type' => 'checkbox',
    '#title' => t('User scalable'),
    '#default_value' => theme_get_setting('ft_viewport_user_scaleable'),
    '#description' => t("Whether users are allowed to zoom in and out. Defaults to 'yes'. This is usually accomplished via multi-touch gestures on iOS and Android devices. You should only disallow zooming (uncheck this checkbox) if your mobile theme is very customized and presented with good typography and graphics for a reduced mobile size."),
  );

  $ret['metatags']['viewport_settings']['scaling_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scaling settings'),
    '#states' => array(
      'visible' => array(
        ':input[name="ft_viewport_user_scaleable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $ret['metatags']['viewport_settings']['scaling_settings']['ft_viewport_minimum_scale'] = array(
    '#type' => 'select',
    '#title' => t('Minimum scale'),
    '#default_value' => theme_get_setting('ft_viewport_minimum_scale'),
    '#options' => $options,
    '#description' => t('The minimum allowed scaling of the site.'),
  );

  $ret['metatags']['viewport_settings']['scaling_settings']['ft_viewport_maximum_scale'] = array(
    '#type' => 'select',
    '#title' => t('Maximum scale'),
    '#default_value' => theme_get_setting('ft_viewport_maximum_scale'),
    '#options' => $options,
    '#description' => t('The maximum allowed scaling of the site.'),
  );

  return $ret;
}

/**
 * Debugger form fields.
 */
function _fortytwo_theme_get_debuggers() {
  $ret = array();
  $ret['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Developer help'),
  );

  $ret['debug']['ft_show_window_size'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show window size - appears in the bottom right corner'),
    '#default_value' => theme_get_setting('ft_show_window_size'),
  );

  $ret['debug']['ft_show_responsive_identifier'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show responsive identifier'),
    '#description' => t('Appears as a border on the top of the toolbar'),
    '#default_value' => theme_get_setting('ft_show_responsive_identifier'),
  );

  $ret['debug']['ft_show_grid'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Show column grid(s)'),
    '#description' => t('matchMedia and SVG capable browser needed (Chrome, Safari or Firefox) to view the debug grid.'),
    '#default_value' => theme_get_setting('ft_show_grid'),
  );

  return $ret;
}

/**
 * Breadcrumb form fields.
 */
function _fortytwo_theme_get_breadcrumb() {
  $ret = array();
  $ret['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb settings'),
  );
  $ret['breadcrumb']['ft_breadcrumb'] = array(
    '#type' => 'select',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('ft_breadcrumb'),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
  );
  $ret['breadcrumb']['breadcrumb_options'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        ':input[name="ft_breadcrumb"]' => array('value' => 'no'),
      ),
    ),
  );
  $ret['breadcrumb']['breadcrumb_options']['ft_breadcrumb_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Breadcrumb separator'),
    '#description' => t('Text only. Don’t forget to include spaces.'),
    '#default_value' => theme_get_setting('ft_breadcrumb_separator'),
    '#size' => 5,
    '#maxlength' => 10,
  );
  $ret['breadcrumb']['breadcrumb_options']['ft_breadcrumb_home'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show home page link in breadcrumb'),
    '#default_value' => theme_get_setting('ft_breadcrumb_home'),
  );
  $ret['breadcrumb']['breadcrumb_options']['ft_breadcrumb_trailing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append a separator to the end of the breadcrumb'),
    '#default_value' => theme_get_setting('ft_breadcrumb_trailing'),
    '#description' => t('Useful when the breadcrumb is placed just before the title.'),
    '#states' => array(
      'disabled' => array(
        ':input[name="ft_breadcrumb_title"]' => array('checked' => TRUE),
      ),
    ),
  );
  $ret['breadcrumb']['breadcrumb_options']['ft_breadcrumb_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append the content title to the end of the breadcrumb'),
    '#default_value' => theme_get_setting('ft_breadcrumb_title'),
    '#description' => t('Useful when the breadcrumb is not placed just before the title.'),
  );
  return $ret;
}

/**
 * A helper function to create a h3 tag.
 *
 * @param string $value
 *   The translated value for in the h3.
 *
 * @return string
 *   The h3 tag.
 */
function _fortytwo_theme_create_prefix($value) {
  return theme('html_tag', array(
    'element' => array(
      '#tag' => 'h3',
      '#value' => $value,
    ),
  ));
}
