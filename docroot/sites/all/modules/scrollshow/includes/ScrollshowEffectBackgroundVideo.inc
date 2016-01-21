<?php

class ScrollshowEffectBackgroundVideo extends ScrollshowEffect {
  function getSettingsDefaults() {
    return array(
      'hires_path' => 'public://scrollshow/video/hi-res/image-[index].jpg',
      'lores_path' => 'public://scrollshow/video/lo-res/image-[index].jpg',
      'first_index' => '1',
      'last_index' => '10',
      'zfill' => '4',
      'hires_fadein_duration' => '1000',
      'maintain_aspect_ratio' => TRUE,
      'maintain_aspect_ratio_full' => TRUE,
    );
  }

  function getSettingsForm($settings) {
    $form = array();
    $form['hires_path'] = array(
      '#type' => 'textfield',
      '#title' => t('High resolution image path'),
      '#description' => t('The path to the high resolution images. Include <em>[index]</em> to denote the image number, for example: <em>public://scrollshow/video/hi-res/image-[index].jpg</em>.'),
      '#default_value' => $settings['hires_path'],
      '#required' => TRUE,
    );
    $form['lores_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Low resolution image path'),
      '#description' => t('The path to the high resolution images. Include <em>[index]</em> to denote the image number, for example: <em>public://scrollshow/video/lo-res/image-[index].png</em>.'),
      '#default_value' => $settings['lores_path'],
      '#required' => TRUE,
    );
    $form['first_index'] = array(
      '#type' => 'textfield',
      '#title' => t('First index'),
      '#description' => t('The index that the image list starts on. Typically <em>0</em> or <em>1</em>.'),
      '#default_value' => $settings['first_index'],
      '#required' => TRUE,
    );
    $form['last_index'] = array(
      '#type' => 'textfield',
      '#title' => t('Last index'),
      '#description' => t('The index of the last image. Typically the number of frames in the "video".'),
      '#default_value' => $settings['last_index'],
      '#required' => TRUE,
    );
    $form['zfill'] = array(
      '#type' => 'textfield',
      '#title' => t('Zero fill'),
      '#description' => t('The number of digits that the index should have, filling in missing digits with zeros. For example, with a zero fill of 4 the index <em>27</em> will be filled out to <em>0027</em>.'),
      '#default_value' => $settings['zfill'],
      '#required' => TRUE,
    );
    $form['hires_fadein_duration'] = array(
      '#type' => 'textfield',
      '#title' => t('High resolution fade in duration'),
      '#description' => t('The number of milliseconds to fade in the high resolution image.'),
      '#default_value' => $settings['hires_fadein_duration'],
      '#required' => TRUE,
    );
    $form['maintain_aspect_ratio'] = array(
      '#type' => 'checkbox',
      '#title' => t('Maintain aspect ratio'),
      '#description' => t('Avoid distorting the background image by maintaining the aspect ratio. <em>Warning: This may cause the bottom of the image to get cut off or blank space to remain!</em>'),
      '#default_value' => $settings['maintain_aspect_ratio'],
    );
    $form['maintain_aspect_ratio_full'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make sure the full window is covered.'),
      '#description' => t('When maintaining the aspect ratio, don\'t leave any blank area at the bottom. <em>Warning: This may cause the left and right of the image to get cut off.</em>'),
      '#default_value' => $settings['maintain_aspect_ratio_full'],
      '#states' => array(
        'invisible' => array(":input[name='scrollshow[effect_settings][background_video][maintain_aspect_ratio]']" => array('checked' => FALSE)),
      ),
    );
    return $form;
  }

  function prepareSettingsForJavascript($settings, $items) {
    // make paths into real URLs
    foreach (array('hires_path', 'lores_path') as $name) {
      $settings[$name] = str_replace(rawurlencode('[index]'), '[index]', file_create_url($settings[$name]));
    }
    // convert all the integers into proper integers
    foreach (array('first_index', 'last_index', 'zfill', 'hires_fadein_duration') as $name) {
      $settings[$name] = (int)$settings[$name];
    }
    // convert boolean values
    foreach (array('maintain_aspect_ratio', 'maintain_aspect_ratio_full') as $name) {
      $settings[$name] = (bool)$settings[$name];
    }

    return $settings;
  }

  function getJS($settings) {
    return array(
      drupal_get_path('module', 'scrollshow') . '/js/scrollshow.background_video.js',
    );
  }
}

