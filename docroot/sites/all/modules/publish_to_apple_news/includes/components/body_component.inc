<?php

/**
 * @file
 * Publish to Apple News body component definition
 */

class AppleNewsBodyComponent extends AppleNewsComponent {
  private $jsonProperties;
  private $bodyLayout;
  private $components;

  public function __construct($article_type, $entity = NULL) {
    parent::__construct($article_type, $entity);

    $this->entityMachineName = 'publish_to_apple_news_body_component';
    $this->name = t('Body');
    $this->role = 'body';
    $this->acceptsTextStyles = TRUE;
    $this->supportsReplacementPatterns = TRUE;
    
    $this->components = array();
  }

  public function schema() {
    $schema = parent::schema();

    $schema['fields']['text'] = array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
      'default' => '',
    );
    $schema['fields']['image_column_start'] = array(
      'type' => 'int',
      'not null' => TRUE,
      'default' => 0,
    );
    $schema['fields']['image_column_span'] = array(
      'type' => 'int',
      'not null' => TRUE,
      'default' => 0,
    );

    return $schema;
  }

  public function generateJson($node, &$files = array(), &$failedStyles) {
    $this->jsonProperties = parent::generateJson($node, $files, $failedStyles);
    $this->jsonProperties['format'] = 'markdown';
    $body_html = publish_to_apple_news_process_text_value($this->entity->text, array('node' => $node), FALSE);
    $body_markdown = publish_to_apple_news_process_text_value($this->entity->text, array('node' => $node));
    
    if (!empty($body_markdown) && substr($body_markdown, 0, 1) != '[') {
      $components[] = $this->jsonProperties + array(
        'text' => $body_markdown,
      );
    }
    
    $images = array();
    $this->getBodyLayout();
    $this->findInlineImages($body_html, $images, $files);
    $this->addAndAnchorImages($images, $components);
    
    return $components;
  }
  
  private function getBodyLayout() {
    // Get the body component's layout
    if (!empty($this->jsonProperties['layout'])) {
      $query = new EntityFieldQuery();
      $result = $query->entityCondition('entity_type', 'publish_to_apple_news_component_layout')
        ->propertyCondition('identifier', $this->jsonProperties['layout'])
        ->propertyCondition('tid', $this->articleType->tid)
        ->execute();
      
      if (isset($result['publish_to_apple_news_component_layout'])) {
        $id = array_keys($result['publish_to_apple_news_component_layout']);
        $this->bodyLayout = entity_load_single('publish_to_apple_news_component_layout', $id[0]);
      }
    }
  }
  
  /**
   * Locate <img> tags in the body's text field and save their text positions.
   * Position is calculated from HTML-stripped string, which seems to be closest to Apple New's rendering engine.
   *
   * @param $body_text
   *  The token_replace'd body, which may include HTML.
   * @param &$images
   *  An array to append to, containing the base filename and text position of each image.
   *  These will be used to anchor the images to the body.
   *
   * @return
   *  Appends to $images and $files.
   */
  private function findInlineImages($body_text, &$images = array(), &$files = array()) {
    // Strip out all tags, minus any images
    $body_only_images = strip_tags($body_text, '<img>');
    
    // Get the start and end position of the first occurance of an image
    $img_start = strpos($body_only_images, '<img');
    $img_end = $img_start !== FALSE ? strpos($body_only_images, '/>', $img_start) + 2 : FALSE;
    
    $body_before_img = substr($body_only_images, 0, $img_start);
    $body_after_img = substr($body_only_images, $img_end);
    $body_minus_img = trim($body_before_img . $body_after_img);
    
    if ($img_start !== FALSE && $img_end !== FALSE) {      
      // Get the image src
      $image_html = substr($body_only_images, $img_start, $img_end);
      preg_match('/<img.+src=[\'"](?P<src>.+?)[\'"].*>/i', $image_html, $image);
      
      // Add the image to our $files array
      $image_to_add = $this->addImageFromSrc($image['src'], $files);
      
      if ($image_to_add) {
        // Add the bundle filename to our $images array
        $base_name = basename($image_to_add);
        $base_name = urldecode($base_name);
        $base_name = str_replace(' ', '-', $base_name);
        $bundle_name = 'bundle://' . $base_name;
        
        // If the image is full-width, position it at the beginning of the paragraph
        if ($this->isFullWidthImage($this->entity->image_column_start, $this->entity->image_column_span)) {
          $body_after_img = trim($body_after_img);
          if (!empty($body_after_img)) {
            $new_line_pos = strrpos($body_before_img, "\n");
            $img_start = $new_line_pos;
          }
        }
        
        $images[] = array(
          'bundle_name' => $bundle_name,
          'position' => $img_start,
        );
      }
      
      // Recursion - pass the body, minus this image tag, back to the function
      if (!empty($body_minus_img)) {
        $this->findInlineImages($body_minus_img, $images, $files);
      }
    }
  }
  
  /**
   * Add the body's inline images as separate photo components.
   * Anchor them to their approximate positions in the text.
   */
  private function addAndAnchorImages($images, &$components) {
    foreach ($images as $image) {
      $img_col_start = $this->entity->image_column_start;
      $img_col_span = $this->entity->image_column_span;
      
      $image_component = array(
        'role' => 'photo',
        'identifier' => $this->jsonProperties['identifier'] . '-image-' . uniqid(),
        'URL' => $image['bundle_name'],
        'layout' => array(
          'columnStart' => (int)$img_col_start,
          'columnSpan' => (int)$img_col_span,
        ),
        'anchor' => array(
          'targetComponentIdentifier' => $this->entity->identifier,
          'targetAnchorPosition' => 'top',
          'rangeStart' => $image['position'],
          'rangeLength' => 1,
        ),
      );
      
      // If the photo is full-width and anchored to text position 0,
      // remove the anchor and place it before the body component.
      if ($this->isFullWidthImage($img_col_start, $img_col_span) && empty($image['position'])) {
        unset($image_component['anchor']);
        array_unshift($components, $image_component);
      } else {
        $components[] = $image_component;
      }
    }
  }
  
  /**
   * Determine if an image is full-width (same layout as the body).
   */
  private function isFullWidthImage($img_col_start, $img_col_span) {
    if ($img_col_start == $this->bodyLayout->column_start && $img_col_span == $this->bodyLayout->column_span) {
      return TRUE;
    } else {
      return FALSE;
    }
  }
  
  /**
   * Given an <img> src, add the local file sytem path, or the external image URL, to the $files array
   * @param $image_src
   *  The image tag's src attribute.
   *
   * @return string
   *  The file sytem path or external URL. False if the local URL is invalid.
   */
  private function addImageFromSrc($image_src, &$files) {
    global $base_url;
    $file_public_path = trim(variable_get('file_public_path', ''), '/');
    $base_url_position = strpos($image_src, $base_url);
    $public_path_position = strpos($image_src, '/' . $file_public_path);
    
    $file_to_add = '';
    if ($base_url_position !== FALSE && $public_path_position !== FALSE) {
      // Local image as a full URL (http://site.com/sites/default/files/image.jpg)
      $imageData = publish_to_apple_news_convert_image($image_src);
      if ($imageData) {
        $file_to_add = $imageData['realpath'];
      }
    } elseif (substr($image_src, 0, 1) == '/' && $public_path_position !== FALSE) {
      // Local image as a relative path (/sites/default/files/image.jpg)
      $imageData = publish_to_apple_news_convert_image($base_url . $image_src);
      if ($imageData) {
        $file_to_add = $imageData['realpath'];
      }
    } elseif (substr($image_src, 0, 4) == 'http') {
      // Externally-hosted image
      $file_to_add = $image_src;
    }
    
    if ($file_to_add) {
      if (!in_array($file_to_add, $files)) {
        $files[] = $file_to_add;
      }
      
      return $file_to_add;
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
    $form['image_column_start'] = array(
      '#type' => 'textfield',
      '#title' => t('Image column start'),
      '#size' => 10,
      '#element_validate' => array('element_validate_number'),
      '#required' => TRUE,
    );

    $form['image_column_span'] = array(
      '#type' => 'textfield',
      '#title' => t('Image column span'),
      '#size' => 10,
      '#element_validate' => array('element_validate_integer_positive'),
      '#required' => TRUE,
    );

    return $form;
  }
}
