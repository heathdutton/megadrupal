<?php

/**
 * @file
 * Destination classes.
 */

use \ChapterThree\AppleNewsAPI\Document\Components;

/**
 * Instagram component.
 */
class ApplenewsExtraDestinationInstagram extends ApplenewsDestinationComponent {

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return [
      'source' => NULL,
    ] + parent::settings();
  }

  /**
   * {@inheritdoc}
   */
  public function validateSettings() {
    return $this->validateSettingsSources(['source']) &&
      parent::validateSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {

    $form = parent::getConfigForm($values);

    $element = $this->getConfigFormSourceElement('source');
    $element['#title'] = t('Link');
    $element['#required'] = TRUE;
    $element['#description'] = t('URL format <em>https://instagram.com/p/6Lg1xVkNjJ</em>');
    $form['settings']['source'] = $element;

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('source'));

    $components = [];

    foreach ($values as $value) {
      if (!empty($value[$key])) {
        $components[] = new Components\Instagram($value[$key]);
      }
    }

    return $components;
  }

}

/**
 * Tweet component.
 */
class ApplenewsExtraDestinationTweet extends ApplenewsDestinationComponent {

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return [
      'source' => NULL,
    ] + parent::settings();
  }

  /**
   * {@inheritdoc}
   */
  public function validateSettings() {
    return $this->validateSettingsSources(['source']) &&
      parent::validateSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {

    $form = parent::getConfigForm($values);

    $element = $this->getConfigFormSourceElement('source');
    $element['#title'] = t('Link');
    $element['#required'] = TRUE;
    $element['#description'] = t('URL format <em>https://twitter.com/chapter_three/status/623547179563495424</em>');
    $form['settings']['source'] = $element;

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('source'));

    $components = [];

    foreach ($values as $value) {
      if (!empty($value[$key])) {
        $components[] = new Components\Tweet($value[$key]);
      }
    }

    return $components;
  }

}

/**
 * Embed Web Video component.
 */
class ApplenewsExtraDestinationEmbedWebVideo extends ApplenewsDestinationComponent {

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return [
      'video_url' => NULL,
      'caption'   => NULL,
    ] + parent::settings();
  }

  /**
   * {@inheritdoc}
   */
  public function validateSettings() {
    return $this->validateSettingsSources(['video_url']) &&
      parent::validateSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {

    $form = parent::getConfigForm($values);

    $element = $this->getConfigFormSourceElement('video_url');
    $element['#title'] = t('Video URL');
    $element['#required'] = TRUE;
    $element['#description'] = t('URL format <em>https://www.youtube.com/embed/r4DQKtYL62Q</em> (Youtube) or <em>https://player.vimeo.com/video/121450839</em> (Vimeo)');
    $form['settings']['video_url'] = $element;

    $element = $this->getConfigFormSourceElement('caption');
    $element['#title'] = t('Caption');
    $form['settings']['caption'] = $element;

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('video_url'));

    $components = [];

    foreach ($values as $value) {
      if (!empty($value[$key])) {
        $component = new Components\EmbedWebVideo($value[$key]);
        if (!empty($value['caption'])) {
          $component->setCaption($value['caption']);
        }
        $components[] = $component;
      }
    }

    return $components;
  }

}

/**
 * Audio component.
 */
class ApplenewsExtraDestinationAudio extends ApplenewsDestinationComponent {

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return [
      'type'            => 'music',
      'url'             => NULL,
      'caption'         => NULL,
      'imageURL'        => NULL,
      'explicitContent' => NULL,
    ] + parent::settings();
  }

  /**
   * {@inheritdoc}
   */
  public function validateSettings() {
    return $this->validateSettingsSources(['url']) &&
      parent::validateSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {

    $form = parent::getConfigForm($values);

    $form['settings']['type'] = [
      '#title'         => t('Type'),
      '#required'      => TRUE,
      '#type'          => 'select',
      '#options'       => [
        'music' => t('Music'),
      ],
      '#default_value' => $this->getSetting('type'),
      '#description'   => t('The type of audio file.'),
    ];

    $element = $this->getConfigFormSourceElement('url', TRUE, TRUE);
    $element['#title'] = t('Audio');
    $element['#required'] = TRUE;
    $form['settings']['url'] = $element;

    $element = $this->getConfigFormSourceElement('imageURL', TRUE, TRUE);
    $element['#title'] = t('Image');
    $form['settings']['imageURL'] = $element;

    $element = $this->getConfigFormSourceElement('caption');
    $element['#title'] = t('Caption');
    $form['settings']['caption'] = $element;

    $element = $this->getConfigFormSourceElement('explicitContent');
    $element['#title'] = t('Explicit Content');
    $form['settings']['explicitContent'] = $element;

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('url'));
    list($imageurl_key, $imageurl_values) = $this->getSourceValues(
      $this->export->source, $this->getSetting('imageURL'));
    list($caption_key, $caption_value) = $this->getSourceValues(
      $this->export->source, $this->getSetting('caption'));
    list($explicitcontent_key, $explicitcontent_value)
      = $this->getSourceValues($this->export->source,
          $this->getSetting('explicitContent'));

    $components = [];

    foreach ($values as $delta => $value) {
      if (!empty($value[$key])) {
        switch ($this->getSetting('type')) {

          case 'music':
            $url = @$this->getSetting('url')['bundle']
              ? $this->export->addAsset($value[$key]) : $value[$key];
            $component = new Components\Music($url);
            if (!empty($caption_value[$delta][$caption_key])) {
              $component->setCaption($caption_value[$delta][$caption_key]);
            }
            if (!empty($imageurl_values[$delta][$imageurl_key])) {
              $url = @$this->getSetting('imageURL')['bundle']
                ? $this->export->addAsset($imageurl_values[$delta][$imageurl_key])
                : $imageurl_values[$delta][$imageurl_key];
              $component->setImageURL($url);
            }
            if (!empty($explicitcontent_value[$delta][$explicitcontent_key])) {
              $component->setExplicitContent(TRUE);
            }
            $components[] = $component;
            break;

        }
      }
    }

    return $components;
  }

}
