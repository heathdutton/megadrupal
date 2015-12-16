<?php

/**
 * @file
 * Publish to Apple News ad unit component definition
 */

class AppleNewsAdComponent extends AppleNewsComponent {

  public function __construct($article_type, $entity = NULL) {
    parent::__construct($article_type, $entity);
    
    $this->entityMachineName = 'publish_to_apple_news_ad_component';
    $this->name = t('Ad unit');
    $this->role = 'banner_advertisement';
    $this->acceptsComponents = FALSE;
    $this->acceptsTextStyles = FALSE;
    $this->acceptsBehavior = FALSE;
  }
  
  public function schema() {
    $schema = parent::schema();
    $schema['fields']['banner_type'] = array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
      'default' => '',
    );
    
    return $schema;
  }
  
  public function generateJson($node, &$files = array(), &$failedStyles) {
    $json = parent::generateJson($node, $files, $failedStyles);
    $json['bannerType'] = $this->entity->banner_type;
    
    return $json;
  }
  
  public function form() {
    $form = parent::form();
    
    $form['banner_type'] = array(
      '#type' => 'select',
      '#options' => array(
        'any' => t('Any'),
        'standard' => t('Standard'),
        'double_height' => t('Double height'),
        'large' => t('Large'),
      ),
      '#title' => t('Banner type'),
      '#description' => t('The banner type that should be shown.'),
    );
    
    return $form;
  }
}
