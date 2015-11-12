<?php

/**
 * @file
 * Publish to Apple News photo component definition
 */

class AppleNewsPhotoComponent extends AppleNewsComponent {

  public function __construct($article_type, $entity = NULL) {
    parent::__construct($article_type, $entity);

    $this->entityMachineName = 'publish_to_apple_news_photo_component';
    $this->name = t('Photo');
    $this->role = 'photo';
    $this->supportsReplacementPatterns = TRUE;
    $this->addsFiles = TRUE;
  }

  public function schema() {
    $schema = parent::schema();

    $schema['fields']['url'] = array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
      'default' => '',
    );
    $schema['fields']['caption'] = array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
      'default' => '',
    );

    return $schema;
  }

  public function generateJson($node, &$files = array(), &$failedStyles) {
    $json = parent::generateJson($node, $files, $failedStyles);

    $url = publish_to_apple_news_process_text_value($this->entity->url, array('node' => $node));
    $imageData = publish_to_apple_news_convert_image($url);
    
    if ($imageData) {
      $json['URL'] = $imageData['bundle'];
      
      if (!in_array($imageData['realpath'], $files)) {
        $files[] = $imageData['realpath'];
      }
      
      $caption = publish_to_apple_news_process_text_value($this->entity->caption, array('node' => $node));
      if (!empty($caption) && !$this->anyTokensLeft($caption)) {
        $json['caption'] = $caption;
      }
      
      return $json;
    } else {
      return FALSE;
    }
  }

  public function form() {
    $form = parent::form();

    $form['url'] = array(
      '#type' => 'textfield',
      '#title' => t('URL'),
      '#description' => $this->tokensDescription(),
      '#required' => TRUE,
    );
    $form['caption'] = array(
      '#type' => 'textfield',
      '#title' => t('Caption'),
      '#description' => $this->tokensDescription(),
    );

    return $form;
  }
}
