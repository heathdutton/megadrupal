<?php

/**
 * @file
 * Publish to Apple News section component definition
 */

class AppleNewsSectionComponent extends AppleNewsComponent {
  
  public function __construct($article_type, $entity = NULL) {
    parent::__construct($article_type, $entity);
    
    $this->entityMachineName = 'publish_to_apple_news_section_component';
    $this->name = t('Section');
    $this->role = 'section';
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
