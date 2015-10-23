<?php

/**
 * @file
 * The UI and menu includes.
 *
 * Provides the default options array and generates and
 * validates the settings form.
 */

/**
 * Implements hook_views_jqfx_jqfx_types().
 */
function views_jqfx_supersized_views_jqfx_jqfx_types() {
  $options = array(
    'views_jqfx_supersized' => t('Supersized'),
  );
  return $options;
}

/**
 * Implements hook_views_jqfx_option_definition().
 */
function views_jqfx_supersized_views_jqfx_option_definition() {
  // Set our default options.
  $options['views_jqfx_supersized'] = array(
    'contains' => array(
      // Display options.
      'pack_js' => array('default' => TRUE),
      'start_slide' => array('default' => '1'),
      'slideshow' => array('default' => TRUE),
      'ss_theme' => array('default' => 'shutter'),
      // Theme options.
      'theme' => array(
        'contains' => array(
          'progress_bar' => array('default' => FALSE),
        ),
      ),
      // Transition options.
      'transition' => array(
        'contains' => array(
          'autoplay' => array('default' => TRUE),
          'transition' => array('default' => '1'),
          'transition_speed' => array('default' => '750'),
          'slide_interval' => array('default' => '7000'),
          'random' => array('default' => FALSE),
          'stop_loop' => array('default' => FALSE),
        ),
      ),
      // Size & Position options.
      'size' => array(
        'contains' => array(
          'min_width' => array('default' => '0'),
          'min_height' => array('default' => '0'),
          'vertical_center' => array('default' => TRUE),
          'horizontal_center' => array('default' => TRUE),
          'fit_always' => array('default' => FALSE),
          'fit_portrait' => array('default' => FALSE),
          'fit_landscape' => array('default' => FALSE),
        ),
      ),

      // Navigation options.
      'navigation' => array(
        'contains' => array(
          'keyboard_nav' => array('default' => TRUE),
          'slide_links' => array('default' => 'number'),
          'thumb_links' => array('default' => FALSE),
          'thumbnail_navigation' => array('default' => FALSE),
          'pause_hover' => array('default' => TRUE),
        ),
      ),

    ),
  );
  return $options;
}

/**
 * Implements hook_views_jqfx_views_jqfx_type_form().
 */
function views_jqfx_supersized_views_jqfx_jqfx_type_form(&$form, &$form_state, &$view) {
  ctools_include('dependent');

  // Display options.
  $form['views_jqfx_supersized']['pack_js'] = array(
    '#type' => 'select',
    '#title' => t('Use packed javascript'),
    '#description' => t('Enabling this loads the packed version of the Supersized plugin.'),
    '#default_value' => $view->options['views_jqfx_supersized']['pack_js'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );

  $form['views_jqfx_supersized']['start_slide'] = array(
    '#type' => 'textfield',
    '#title' => t('Start slide!'),
    '#description' => t('Set starting Slide. Use "0" for a random start postion.'),
    '#default_value' => $view->options['views_jqfx_supersized']['start_slide'],
  );
  $form['views_jqfx_supersized']['slideshow'] = array(
    '#type' => 'select',
    '#title' => t('Slideshow'),
    '#description' => t('Turns the slideshow on/off. Disables navigation and transitions.'),
    '#default_value' => $view->options['views_jqfx_supersized']['slideshow'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_supersized']['ss_theme'] = array(
    '#type' => 'select',
    '#title' => t('Supersized Theme'),
    '#description' => t('Use Supersized or the included theme "Shutter".'),
    '#default_value' => $view->options['views_jqfx_supersized']['ss_theme'],
    '#options' => array(
      'none' => t('Supersized'),
      'shutter' => t('Supersized Shutter'),
    ),
  );

  // Theme options.
  $form['views_jqfx_supersized']['theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['views_jqfx_supersized']['theme']['progress_bar'] = array(
    '#type' => 'select',
    '#title' => t('Progress Bar'),
    '#description' => t('Timer for each slide.'),
    '#default_value' => $view->options['views_jqfx_supersized']['theme']['progress_bar'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );

  // Size & Position  options.
  $form['views_jqfx_supersized']['size'] = array(
    '#type' => 'fieldset',
    '#title' => t('Size & Position options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['views_jqfx_supersized']['size']['min_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Min Width'),
    '#description' => t('Min width allowed (in pixels)'),
    '#default_value' => $view->options['views_jqfx_supersized']['size']['min_width'],
  );
  $form['views_jqfx_supersized']['size']['min_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Min Height'),
    '#description' => t('Min height allowed (in pixels)'),
    '#default_value' => $view->options['views_jqfx_supersized']['size']['min_height'],
  );
  $form['views_jqfx_supersized']['size']['vertical_center'] = array(
    '#type' => 'select',
    '#title' => t('Vertical Center'),
    '#description' => t('Should the image be centered vertically.'),
    '#default_value' => $view->options['views_jqfx_supersized']['size']['vertical_center'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_supersized']['size']['horizontal_center'] = array(
    '#type' => 'select',
    '#title' => t('Horizontal Center'),
    '#description' => t('Should the image be centered horizontally.'),
    '#default_value' => $view->options['views_jqfx_supersized']['size']['horizontal_center'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_supersized']['size']['fit_always'] = array(
    '#type' => 'select',
    '#title' => t('Fit Always'),
    '#description' => t('Image will never exceed browser width or height (Ignores min. dimensions).'),
    '#default_value' => $view->options['views_jqfx_supersized']['size']['fit_always'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_supersized']['size']['fit_portrait'] = array(
    '#type' => 'select',
    '#title' => t('Fit Portrait'),
    '#description' => t('Portrait images will not exceed browser height.'),
    '#default_value' => $view->options['views_jqfx_supersized']['size']['fit_portrait'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_supersized']['size']['fit_landscape'] = array(
    '#type' => 'select',
    '#title' => t('Fit Landscape'),
    '#description' => t('Landscape images will not exceed browser width.'),
    '#default_value' => $view->options['views_jqfx_supersized']['size']['fit_landscape'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );

  // Transition options.
  $form['views_jqfx_supersized']['transition'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image transition options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['views_jqfx_supersized']['transition']['autoplay'] = array(
    '#type' => 'select',
    '#title' => t('Autoplay'),
    '#description' => t('Determines whether to start the slideshow on page load.'),
    '#default_value' => $view->options['views_jqfx_supersized']['transition']['autoplay'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $effect = array(
    0 => t('None'),
    1 => t('Fade'),
    2 => t('Slide in from Top'),
    3 => t('Slide in from Right'),
    4 => t('Slide in from Bottom'),
    5 => t('Slide in from Left'),
    6 => t('Carousel from Right to Left'),
    7 => t('Carousel from Left to Right'),
  );
  $form['views_jqfx_supersized']['transition']['transition'] = array(
    '#type' => 'select',
    '#title' => t('Effect'),
    '#description' => t('The transition effect between images'),
    '#default_value' => $view->options['views_jqfx_supersized']['transition']['transition'],
    '#options' => $effect,
  );

  $form['views_jqfx_supersized']['transition']['transition_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation Speed'),
    '#description' => t('Animation speed in milliseconds'),
    '#default_value' => $view->options['views_jqfx_supersized']['transition']['transition_speed'],
  );
  $form['views_jqfx_supersized']['transition']['slide_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Display Time'),
    '#description' => t('Time between slide changes in milliseconds.'),
    '#default_value' => $view->options['views_jqfx_supersized']['transition']['slide_interval'],
  );
  $form['views_jqfx_supersized']['transition']['random'] = array(
    '#type' => 'select',
    '#title' => t('Random'),
    '#description' => t('Display Slides in a Random Order.'),
    '#default_value' => $view->options['views_jqfx_supersized']['transition']['random'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_supersized']['transition']['stop_loop'] = array(
    '#type' => 'select',
    '#title' => t('Stop on End'),
    '#description' => t('Pauses slideshow upon reaching the last slide.'),
    '#default_value' => $view->options['views_jqfx_supersized']['transition']['stop_loop'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );

  // Navigation options.
  $form['views_jqfx_supersized']['navigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['views_jqfx_supersized']['navigation']['keyboard_nav'] = array(
    '#type' => 'select',
    '#title' => t('Keyboard Navigation'),
    '#description' => t('Allows controlls via keyboard'),
    '#default_value' => $view->options['views_jqfx_supersized']['navigation']['keyboard_nav'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $slide_links = array(
    FALSE => t('None'),
    'num' => t('Numbers'),
    'name' => t('Title of Slide'),
    'empty' => t('Empty (useful for custom themeing)'),
  );
  $form['views_jqfx_supersized']['navigation']['slide_links'] = array(
    '#type' => 'select',
    '#title' => t('Slide Links'),
    '#description' => t('Adds a pager'),
    '#default_value' => $view->options['views_jqfx_supersized']['navigation']['slide_links'],
    '#options' => $slide_links,
  );
  $form['views_jqfx_supersized']['navigation']['thumb_links'] = array(
    '#type' => 'select',
    '#title' => t('Thumb Links'),
    '#description' => t('Generates a list of Thumb Links to jump to a specific slide.'),
    '#default_value' => $view->options['views_jqfx_supersized']['navigation']['thumb_links'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_supersized']['navigation']['thumbnail_navigation'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail Navigation'),
    '#description' => t('Toggles Forward/Backward thumbnail navigation.'),
    '#default_value' => $view->options['views_jqfx_supersized']['navigation']['thumbnail_navigation'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );

  $form['views_jqfx_supersized']['navigation']['pause_hover'] = array(
    '#type' => 'select',
    '#title' => t('Pause on Hover'),
    '#description' => t('Stop animation while hovering.'),
    '#default_value' => $view->options['views_jqfx_supersized']['navigation']['pause_hover'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
}

/**
 *  Validate form selections.
 */
function views_jqfx_supersized_views_jqfx_options_form_validate(&$form, &$form_state, &$view) {
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_supersized']['start_slide'])) {
    form_error($form['views_jqfx_supersized']['start_slide'], t('Start Slide must be numeric', array('Start slide')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_supersized']['size']['min_width'])) {
    form_error($form['views_jqfx_supersized']['size']['min_width'], t('Min Width must be numeric. Use 0 to disable.', array('Min Width')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_supersized']['size']['min_height'])) {
    form_error($form['views_jqfx_supersized']['size']['min_height'], t('Min Width must be numeric. Use 0 to disable.', array('Min Height')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_supersized']['transition']['transition_speed'])) {
    form_error($form['views_jqfx_supersized']['transition']['transition_speed'], t('Transition Speed must be numeric.', array('Transition Speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_supersized']['transition']['slide_interval'])) {
    form_error($form['views_jqfx_supersized']['transition']['slide_interval'], t('Display time must be numeric.', array('Display Time')));
  }
}
