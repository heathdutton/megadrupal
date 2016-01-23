<?php

class ScrollshowEffectDefault extends ScrollshowEffect {
  function getSettingsDefaults() {
    return array(
      'smoothscroll_easing' => 'swing',
      'smoothscroll_duration' => '500',
      'slideposition' => '50',
      'slidespacing' => '0.0',
      'fallback_text' => 'Too slow? <a href="!url">Click here</a>.',
    );
  }

  function getSettingsForm($settings) {
    $form = array();
    $form['smoothscroll_easing'] = array(
      '#type' => 'select',
      '#title' => t('Smoothscroll easing function'),
      '#options' => scrollshow_easing_options(),
      '#description' => t('The easing function for the scrolling animation used when the user clicks on an item in the menu. See <a href="!url">this page</a> for graphs of each function.', array('!url' => 'http://jqueryui.com/resources/demos/effect/easing.html')),
      '#default_value' => $settings['smoothscroll_easing'],
    );
    $form['smoothscroll_duration'] = array(
      '#type' => 'textfield',
      '#title' => t('Smoothscroll duration'),
      '#description' => t('The length of the scroll animation in milliseconds'),
      '#default_value' => $settings['smoothscroll_duration'],
    );
    $form['slideposition'] = array(
      '#type' => 'textfield',
      '#title' => t('Slide position'),
      '#description' => t('Where on the screen to place the slide from 0-100. 0 is the top of the slide, 50 is the middle and 100 is the bottom.'),
      '#default_value' => $settings['slideposition'],
    );
    $form['slidespacing'] = array(
      '#type' => 'textfield',
      '#title' => t('Slide spacing'),
      '#description' => t('The number "screen heights" to place between each slide. Can be fractional, for example: 0.5, 1.5, etc.'),
      '#default_value' => $settings['slidespacing'],
    );
    $form['fallback_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Fallback text'),
      '#description' => t('The message shown to users, letting them fallback on a non-Javascript view. <em>!url</em> will be replaced with the URL.'),
      '#default_value' => $settings['fallback_text'],
    );
    return $form;
  }

  function prepareSettingsForJavascript($settings, $items) {
    $settings['slidespacing'] = (float)$settings['slidespacing'];

    if (is_numeric($settings['smoothscroll_duration'])) {
      $settings['smoothscroll_duration'] = (int)$settings['smoothscroll_duration'];
    }

    return $settings;
  }

  function getJS($settings) {
    return array(
      drupal_get_path('module', 'scrollshow') . '/js/scrollshow.default.js',
    );
  }

  function getLibrary($settings) {
    $libraries = array();

    // if we're using an advanced easing function, make sure to load the library
    // for it.
    if (!in_array($settings['smoothscroll_easing'], array('swing', 'linear'))) {
      $libraries[] = array('system', 'effects');
    }

    return $libraries;
  }
}

