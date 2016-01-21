<?php

/**
 * @file
 * Administrative page callbacks for the prettyphoto_formatters module.
 */

/**
 * Configuration form for controlling the prettyPhoto behaviour.
 */
function prettyphoto_formatters_settings_form() {
  $form['settings_js'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customize prettyPhoto'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['settings_js']['prettyphoto_formatters_animation_speed'] = array(
    '#type' => 'select',
    '#title' => t('Animation speed'),
    '#description' => t('Animation speed, e.g. open and close modal.'),
    '#options' => array(
      'fast' => t('Fast'),
      'slow' => t('Slow'),
      'normal' => t('Normal'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_animation_speed', 'fast'),
  );

  $form['settings_js']['prettyphoto_formatters_slideshow'] = array(
    '#type' => 'textfield',
    '#title' => t('Slideshow speed'),
    '#description' => t('false OR interval time in ms'),
    '#field_suffix' => 'ms',
    '#size' => 5,
    '#default_value' => variable_get('prettyphoto_formatters_slideshow', 5000),
  );

  $form['settings_js']['prettyphoto_formatters_autoplay_slideshow'] = array(
    '#type' => 'select',
    '#title' => t('Autoplay slideshow'),
    '#description' => t('If enabled, slideshow will start after opening modal.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_autoplay_slideshow', 0),
  );

  $form['settings_js']['prettyphoto_formatters_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Opacity'),
    '#description' => t('Value between 0 and 1'),
    '#size' => 5,
    '#default_value' => variable_get('prettyphoto_formatters_opacity', 0.80),
    '#required' => TRUE,
  );

  $form['settings_js']['prettyphoto_formatters_show_title'] = array(
    '#type' => 'select',
    '#title' => t('Show title'),
    '#description' => t('If enabled, title will appear over the modal.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_show_title', 1),
  );

  $form['settings_js']['prettyphoto_formatters_allow_resize'] = array(
    '#type' => 'select',
    '#title' => t('Allow resize'),
    '#description' => t('Resize the photos bigger than viewport.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_allow_resize', 1),
  );

  $form['settings_js']['prettyphoto_formatters_default_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Default width'),
    '#description' => t('Default width of modal.'),
    '#field_suffix' => 'px',
    '#size' => 5,
    '#default_value' => variable_get('prettyphoto_formatters_default_width', 500),
    '#element_validate' => array('element_validate_number'),
    '#required' => TRUE,
  );

  $form['settings_js']['prettyphoto_formatters_default_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Default height'),
    '#description' => t('Default height of modal.'),
    '#field_suffix' => 'px',
    '#size' => 5,
    '#default_value' => variable_get('prettyphoto_formatters_default_height', 344),
    '#element_validate' => array('element_validate_number'),
    '#required' => TRUE,
  );

  $form['settings_js']['prettyphoto_formatters_counter_separator_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Counter separator label'),
    '#description' => t('The separator for the gallery counter 1 "of" 2.'),
    '#size' => 5,
    '#default_value' => variable_get('prettyphoto_formatters_counter_separator_label', '/'),
  );

  $form['settings_js']['prettyphoto_formatters_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('Theme for modal.'),
    '#options' => array(
      'pp_default' => t('Default'),
      'light_rounded' => t('Light rounded'),
      'dark_rounded' => t('Dark rounded'),
      'light_square' => t('Light square'),
      'dark_square' => t('Dark square'),
      'facebook' => t('Facebook'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_theme', 'pp_default'),
  );

  $form['settings_js']['prettyphoto_formatters_horizontal_padding'] = array(
    '#type' => 'textfield',
    '#title' => t('Padding'),
    '#description' => t('The padding on each side of the picture.'),
    '#field_suffix' => 'px',
    '#size' => 5,
    '#default_value' => variable_get('prettyphoto_formatters_horizontal_padding', 20),
    '#element_validate' => array('element_validate_number'),
    '#required' => TRUE,
  );

  $form['settings_js']['prettyphoto_formatters_hideflash'] = array(
    '#type' => 'select',
    '#title' => t('Hide flash'),
    '#description' => t('Hides all the flash object on a page, set to TRUE if flash appears over prettyPhoto.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_hideflash', 0),
  );

  $form['settings_js']['prettyphoto_formatters_wmode'] = array(
    '#type' => 'select',
    '#title' => t('wmode'),
    '#description' => t('Set the flash wmode attribute.'),
    '#options' => array(
      'window' => t('window'),
      'direct' => t('direct'),
      'opaque' => t('opaque'),
      'transparent' => t('transparent'),
      'gpu' => t('gpu'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_wmode', 'opaque'),
  );

  $form['settings_js']['prettyphoto_formatters_autoplay'] = array(
    '#type' => 'select',
    '#title' => t('Autoplay'),
    '#description' => t('Automatically start videos.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_autoplay', 1),
  );

  $form['settings_js']['prettyphoto_formatters_modal'] = array(
    '#type' => 'select',
    '#title' => t('Close mode'),
    '#description' => t('If set to true, only the close button will close the window.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_modal', 0),
  );

  $form['settings_js']['prettyphoto_formatters_overlay_gallery'] = array(
    '#type' => 'select',
    '#title' => t('Overlay gallery'),
    '#description' => t('If set to true, a gallery will overlay the fullscreen image on mouse over.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_overlay_gallery', 1),
  );

  $form['settings_js']['prettyphoto_formatters_keyboard_shortcuts'] = array(
    '#type' => 'select',
    '#title' => t('Keyboard shortcuts'),
    '#description' => t('Set to false if you open forms inside prettyPhoto.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_keyboard_shortcuts', 1),
  );

  $form['settings_js']['prettyphoto_formatters_ie6_fallback'] = array(
    '#type' => 'select',
    '#title' => t('IE6 fallback'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_ie6_fallback', 1),
  );

  $form['settings_social'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['settings_social']['prettyphoto_formatters_social_tools'] = array(
    '#type' => 'select',
    '#title' => t('Social tools'),
    '#description' => t('If enabled, social buttons appear in modal.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_social_tools', 1),
  );

  $form['settings_social']['prettyphoto_formatters_social_items'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Social buttons'),
    '#options' => array(
      'twitter' => t('Twitter'),
      'fb_like' => t('Facebook Like'),
      'fb_share' => t('Facebook Share'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_social_items', array(
      'twitter',
      'fb_like',
      'fb_share',
    )),
  );

  $form['settings_social']['prettyphoto_formatters_deeplinking'] = array(
    '#type' => 'select',
    '#title' => t('Deep linking'),
    '#description' => t('Allow prettyPhoto to update the url to enable deeplinking.'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('prettyphoto_formatters_deeplinking', 1),
  );

  $form['settings_paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['settings_paths']['prettyphoto_formatters_exclude_action'] = array(
    '#type' => 'radios',
    '#options' => array(
      'page_enable' => t('Load only on the listed pages.'),
      'page_disable' => t('Load on every page except the listed pages.'),
    ),
    '#title' => t('Enable lightbox2 on specific pages'),
    '#default_value' => variable_get('prettyphoto_formatters_exclude_action', 'page_disable'),
  );

  $form['settings_paths']['prettyphoto_formatters_exclude_these_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude these paths'),
    '#description' => t('List one page per line as Drupal paths. The * character is a wildcard. Example paths are "node/add/page" and "node/add/*".  Use &lt;front&gt; to match the front page.'),
    '#default_value' => variable_get('prettyphoto_formatters_exclude_these_paths', ''),
  );

  return system_settings_form($form);
}

/**
 * Validation function for the configuration form.
 *
 * Ensure that the hex color value submitted is valid and that the speeds are
 * numeric.
 */
function prettyphoto_formatters_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Save configuration')) {
    $values = $form_state['values'];

    $slideshow = $values['prettyphoto_formatters_slideshow'];
    $opacity = (float) $values['prettyphoto_formatters_opacity'];

    if (!is_numeric($slideshow) && $slideshow != 'false') {
      form_set_error('prettyphoto_formatters_slideshow', t('You must enter an interval time in ms. Or set it to false.'));
    }

    if (empty($opacity) || !is_numeric($opacity) || $opacity < 0 || $opacity > 1) {
      form_set_error('prettyphoto_formatters_opacity', t('You must enter a value between 0 and 1.'));
    }
  }
}
