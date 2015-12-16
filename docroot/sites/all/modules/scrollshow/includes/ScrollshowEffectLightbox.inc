<?php

class ScrollshowEffectLightbox extends ScrollshowEffect {
  function getSettingsDefaults() {
    return array();
  }

  function getSettingsForm($settings) {
    $form = array();
    $form["message"] = array(
      '#markup' => '<p>' . t('Requires the <a href="http://drupal.org/project/lightbox2">Lightbox2</a> module to function.') . '</p>',
    );
    return $form;
  }

  function getJS($settings) {
    return array(
      drupal_get_path('module', 'scrollshow') . '/js/scrollshow.lightbox.js',
    );
  }
}

