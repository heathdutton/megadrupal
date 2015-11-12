<?php

/**
 * @file
 * Publish to Apple News mosaic component definition
 */

class AppleNewsMosaicComponent extends AppleNewsComponent {
  
  public function __construct($articleType, $entity = NULL) {
    parent::__construct($articleType, $entity);
    
    $this->entityMachineName = 'publish_to_apple_news_mosaic_component';
    $this->name = t('Mosaic');
    $this->role = 'mosaic';
    $this->supportsReplacementPatterns = TRUE;
  }
  
  public function schema() {
    $schema = parent::schema();
    
    $schema['fields']['images_field'] = array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
      'default' => '',
    );
    
    return $schema;
  }
  
  public function form() {
    $form = parent::form();
    
    $form['images_field'] = array(
      '#type' => 'textfield',
      '#title' => t('Images field'),
      '#description' => t('Enter the <a href="#ui-replacement-patterns" rel="modal:open">replacement pattern</a> for a multi-value image field. Images will be automatically added and their titles will be used as captions.'),
      '#required' => TRUE,
    );
    
    return $form;
  }
  
  public function generateJson($node, &$files = array(), &$failedStyles) {
    $json = parent::generateJson($node, $files, $failedStyles);
    
    $json['items'] = $this->getGalleryItems($node, $files);
    
    // Only output JSON for this component if we have at least one item
    if (!empty($json['items'])) {
      return $json;
    } else {
      return FALSE;
    }
  }
}
