<?php

/**
 * @file
 * Publish to Apple News header component definition
 */

class AppleNewsHeaderComponent extends AppleNewsComponent {
  
  public function __construct($article_type, $entity = NULL) {
    parent::__construct($article_type, $entity);
    
    $this->entityMachineName = 'publish_to_apple_news_header_component';
    $this->name = t('Header');
    $this->role = 'header';
    $this->acceptsComponents = TRUE;
    $this->acceptsTextStyles = FALSE;
  }
  
  public function schema() {
    $schema = parent::schema();
    
    // No additional fields needed for this component type
    
    return $schema;
  }
  
  public function generateJson($node, &$files = array(), &$failedStyles) {
    $json = parent::generateJson($node, $files, $failedStyles);
    
    // Add additional fields specific to this component type
    
    return $json;
  }
  
  public function form() {
    return parent::form();
  }
}
