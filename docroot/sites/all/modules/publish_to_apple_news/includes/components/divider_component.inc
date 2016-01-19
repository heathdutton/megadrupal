<?php

/**
 * @file
 * Publish to Apple News divider component definition
 */

class AppleNewsDividerComponent extends AppleNewsComponent {

  public function __construct($article_type, $entity = NULL) {
    parent::__construct($article_type, $entity);
    
    $this->entityMachineName = 'publish_to_apple_news_divider_component';
    $this->name = t('Divider');
    $this->role = 'divider';
    $this->acceptsTextStyles = FALSE;
  }
  
  public function schema() {
    $schema = parent::schema();
    
    $schema['fields']['stroke_color'] = array(
      'type' => 'varchar',
      'length' => 10,
      'not null' => TRUE,
      'default' => '',
    );
    $schema['fields']['stroke_width'] = array(
      'type' => 'int',
      'not null' => TRUE,
      'default' => 0,
    );
    
    return $schema;
  }
  
  public function generateJson($node, &$files = array(), &$failedStyles) {
    $json = parent::generateJson($node, $files, $failedStyles);
    
    $json['stroke'] = array(
      'color' => $this->entity->stroke_color,
      'width' => (int)$this->entity->stroke_width,
    );
    
    return $json;
  }
  
  public function form() {
    $form = parent::form();
    
    $form['stroke_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Stroke color'),
      '#description' => t("The stroke color, defined as a 3- to 8-character RGBA hexadecimal string. Should include '#'."),
      '#required' => TRUE,
    );
    $form['stroke_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Stroke width'),
      '#description' => t('The width, in points.'),
      '#element_validate' => array('element_validate_number'),
      '#required' => TRUE,
    );
    
    return $form;
  }
}
