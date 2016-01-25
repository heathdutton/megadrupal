<?php

/**
 * @file
 * Publish to Apple News author component definition
 */

class AppleNewsAuthorComponent extends AppleNewsComponent {

  public function __construct($article_type, $entity = NULL) {
    parent::__construct($article_type, $entity);
    
    $this->entityMachineName = 'publish_to_apple_news_author_component';
    $this->name = t('Author');
    $this->role = 'author';
    $this->acceptsTextStyles = TRUE;
    $this->supportsReplacementPatterns = TRUE;
  }
  
  public function schema() {
    $schema = parent::schema();
    
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
    $text = publish_to_apple_news_process_text_value($this->entity->text, array('node' => $node));;
    
    if (!empty($text) && !$this->anyTokensLeft($text)) {
      $json['text'] = $text;
      $json['format'] = 'markdown';
      return $json;
    } else {
      return FALSE;
    }
  }
  
  public function form() {
    $form = parent::form();
    
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text'),
      '#description' => $this->tokensDescription(),
      '#required' => TRUE,
    );
    
    return $form;
  }
}
