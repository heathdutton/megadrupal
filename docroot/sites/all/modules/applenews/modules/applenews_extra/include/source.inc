<?php

/**
 * @file
 * Source classes.
 */

/**
 * Exports a Drupal link_field.
 */
class ApplenewsExtraSourceFieldLink extends ApplenewsSourceField {

  /**
   * {@inheritdoc}
   */
  static public function label() {
    return t('Link');
  }

  /**
   * {@inheritdoc}
   */
  static public function attributes() {
    return [
      'url' => 'URL',
    ];
  }

  /**
   * Field types this class applies to.
   */
  static public function types() {
    return [
      'link_field',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function export($value = NULL) {
    $return = [
      'url' => '',
    ];
    if (!isset($value)) {
      try {
        /** @var EntityMetadataWrapper $value */
        $value = $this->wrapper->{$this->fieldName};
      }
      catch (EntityMetadataWrapperException $e) {
        return $return;
      }
    }
    $v = $value->value();
    if (!empty($v['url'])) {
      $return['url'] = $v['url'];
    }
    return $return;
  }

}

/**
 * Exports a Drupal video_embed_field.
 */
class ApplenewsExtraSourceFieldVideoEmbedField extends ApplenewsSourceField {

  /**
   * {@inheritdoc}
   */
  static public function label() {
    return t('Video Embed');
  }

  /**
   * {@inheritdoc}
   */
  static public function attributes() {
    return [
      'video_url' => 'Video URL',
      'caption' => 'Video Caption',
    ];
  }

  /**
   * Field types this class applies to.
   */
  static public function types() {
    return [
      'video_embed_field',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function export($value = NULL) {
    $return = [
      'video_url' => '',
      'caption' => '',
    ];
    if (!isset($value)) {
      try {
        /** @var EntityMetadataWrapper $value */
        $value = $this->wrapper->{$this->fieldName};
      }
      catch (EntityMetadataWrapperException $e) {
        return $return;
      }
    }
    $v = $value->value();
    if (!empty($v['video_url'])) {
      $return['video_url'] = $v['video_url'];
      $return['caption'] = !empty($v['description']) ? $v['description'] : '';
    }
    return $return;
  }

}
