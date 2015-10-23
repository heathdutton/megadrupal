<?php
/**
 * @file
 * Orbit Media Flexslider Block configuration.
 */

/**
 * Oms_flexslider_block_configure_form.
 *
 * @access public
 * @return array
 *   form array
 */
function oms_flexslider_block_configure_form($delta) {
  $form['flex_animation'] = array(
    '#type' => 'radios',
    '#title' => t('Animation Type'),
    '#options' => array(
      'fade' => t('fade'),
      'slide' => t('slide'),
    ),
    '#required' => TRUE,
    '#default_value' => 'fade',
  );
  $form['flex_direction'] = array(
    '#type' => 'radios',
    '#title' => t('Animation Direction'),
    '#options' => array(
      'horizontal' => t('horizontal'),
      'vertical' => t('vertical'),
    ),
    '#required' => TRUE,
    '#default_value' => 'horizontal',
  );
  $form['flex_reverse'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reverse Animation'),
    '#default_value' => '0',
  );
  $form['flex_animationLoop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Animation Loop'),
    '#default_value' => '1',
  );
  $form['flex_smoothHeight'] = array(
    '#type' => 'checkbox',
    '#title' => t('Smooth Height'),
    '#default_value' => '0',
  );
  $form['flex_startAt'] = array(
    '#type' => 'textfield',
    '#title' => t('Start At'),
    '#default_value' => '0',
  );
  $form['flex_slideshow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Animate Slider automatically'),
    '#default_value' => '1',
  );
  $form['flex_slideshowSpeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Slideshow speed in milliseconds'),
    '#default_value' => '7000',
  );
  $form['flex_animationSpeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation speed in milliseconds'),
    '#default_value' => '600',
  );
  $form['flex_pauseOnAction'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause the slideshow on control actions'),
    '#default_value' => '1',
  );
  $form['flex_pauseOnHover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause the slideshow on hover'),
    '#default_value' => '0',
  );
  $form['flex_useCSS'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use CSS3 transitions'),
    '#default_value' => '1',
  );
  $form['flex_touch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow touch swipe navigation'),
    '#default_value' => '1',
  );
  $form['flex_video'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prevent CSS3 3D Transforms on videos'),
    '#default_value' => '0',
  );
  $form['flex_controlNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create Navigation for paging control'),
    '#default_value' => '1',
  );
  $form['flex_directionNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add previous and next navigation buttons'),
    '#default_value' => '1',
  );
  $form['flex_prevText'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for previous button'),
    '#default_value' => 'Previous',
  );
  $form['flex_nextText'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for next button'),
    '#default_value' => 'Next',
  );
  $form['flex_keyboard'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow left right keyboard navigation'),
    '#default_value' => '1',
  );
  $form['flex_pausePlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create Pause/Play Dynamic Element'),
    '#default_value' => '0',
  );
  $form['flex_pausetext'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for pause button'),
    '#default_value' => 'Pause',
  );
  $form['flex_playText'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for play button'),
    '#default_value' => 'Play',
  );

  $results = oms_flexslider_block_query($delta);
  if ($results) {
    foreach ($results as $record) {
      $values = json_decode($record->slider_options, TRUE);
      foreach ($values as $key => $value) {
        $form['flex_' . $key]['#default_value'] = $value;
      }
    }
  }
  return $form;
}
