<?php

class ScrollshowEffectCurtain extends ScrollshowEffect {
  function getSettingsDefaults() {
    return array(
      'effect'        => 'slide',
      'image_url'     => '',
      'take_up_space' => TRUE,
      'above_slides'  => TRUE,
    );
  }

  function getSettingsForm($settings) {
    $form = array();
    $form["effect"] = array(
      '#type' => 'select',
      '#title' => t('Effect'),
      '#options' => array(
        'slide' => t('Slide up'),
        'fade' => t('Fade out'),
      ),
      '#description' => t('The effect by which the curtain disappears.'),
      '#default_value' => $settings["effect"],
    );
    $form["image_url"] = array(
      '#type' => 'textfield',
      '#title' => t('Image URL'),
      '#description' => t('The location of the curtain image.'),
      '#default_value' => $settings["image_url"],
    );
    $form["take_up_space"] = array(
      '#type' => 'checkbox',
      '#title' => t('Take up space like a slide'),
      '#description' => t('If checked, the curtain will take up space as if it were a slide.'),
      '#default_value' => $settings["take_up_space"],
    );
    $form["above_slides"] = array(
      '#type' => 'checkbox',
      '#title' => t('Appear on top of the slides'),
      '#description' => t('If checked, the curtain will appear on top of the slides (the default). If unchecked, the slides will actually appear on top of the curtain.'),
      '#default_value' => $settings["above_slides"],
    );
    return $form;
  }

  function getJS($settings) {
    return array(
      drupal_get_path('module', 'scrollshow') . '/js/scrollshow.curtain.js',
    );
  }
}

