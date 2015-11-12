<?php

/**
 * @file
 * Publish to Apple News heading component definition
 */

class AppleNewsHeadingComponent extends AppleNewsComponent {
  
  public function __construct($article_type, $entity = NULL) {
    parent::__construct($article_type, $entity);
    
    $this->entityMachineName = 'publish_to_apple_news_heading_component';
    $this->name = t('Heading');
    $this->role = 'heading';
    $this->supportsReplacementPatterns = TRUE;
    $this->acceptsTextStyles = TRUE;
  }
  
  public function schema() {
    $schema = parent::schema();
    
    $schema['fields']['size'] = array(
      'type' => 'varchar',
      'length' => 10,
      'not null' => TRUE,
      'default' => '',
    );
    $schema['fields']['text'] = array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
      'default' => '',
    );
    
    return $schema;
  }
  
  public function generateJson($node, &$files = array(), &$failedStyles) {
    $json = parent::generateJson($node, $files, $failedStyles);
    $json['size'] = $this->entity->size;
    $text = publish_to_apple_news_process_text_value($this->entity->text, array('node' => $node));
    
    if (!empty($text) && !$this->anyTokensLeft($text)) {
      $json['text'] = $text;
      return $json;
    } else {
      return FALSE;
    }
  }
  
  public function form() {
    $form = parent::form();
    
    $form['size'] = array(
      '#type' => 'select',
      '#options' => array(
        'heading1' => t('H1'),
        'heading2' => t('H2'),
        'heading3' => t('H3'),
        'heading4' => t('H4'),
        'heading5' => t('H5'),
        'heading6' => t('H6'),
      ),
      '#title' => t('Size'),
      '#required' => TRUE,
      '#description' => t('Note: This does not affect the actual text size. Select the appropriate text style above.'),
    );
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text'),
      '#description' => $this->tokensDescription(),
      '#required' => TRUE,
    );
    
    return $form;
  }
}