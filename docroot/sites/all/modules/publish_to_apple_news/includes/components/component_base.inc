<?php

/**
 * @file
 * Base class for Publish to Apple News Components
 */

class AppleNewsComponent {
  public $entityMachineName;
  public $articleType;
  public $entity;
  public $role;
  public $acceptsComponents;
  public $acceptsTextStyles;
  public $acceptsLayoutAndStyle;

  /**
   * Constructor for this component.
   *
   * @param $articleType
   *  The fully-loaded article type entity currently being configured/processed.
   * @param $entity
   *  The fully-loaded component entity. Can either be an existing entity or one create via entity_create().
   *  Note: If using entity_create, at minimum the parent_cid and weight fields should be set.
   */
  public function __construct($articleType, $entity = NULL) {
    $this->articleType = $articleType;

    if (isset($entity)) {
      $this->entity = $entity;
    }

    $this->entityMachineName = 'publish_to_apple_news_component';

    // Define the name, to be used when adding a component
    $this->name = t('Component');

    // Define the role, used when generating json for an article
    $this->role = 'component';

    // Defines whether this component accepts other child components
    $this->acceptsComponents = FALSE;

    // Whether this component accepts text styles
    $this->acceptsTextStyles = FALSE;

    // If any fields support relacement patterns, this will add the token tree
    $this->supportsReplacementPatterns = FALSE;

    // Whether this component can reference a layout and style
    $this->acceptsLayoutAndStyle = TRUE;
    
    // Whether this component can use a behavior
    $this->acceptsBehavior = TRUE;
  }

  /**
   * Define the component's db schema. This is called in hook_schema().
   */
  public function schema() {
    $schema = array(
      'fields' => array(
        'cid' => array(
          'type' => 'serial',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
        'tid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'parent_cid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'parent_type' => array(
          'type' => 'varchar',
          'length' => 255,
          'default' => '',
        ),
        'lid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'sid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'tsid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'name' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'default' => '',
        ),
        'identifier' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'default' => '',
        ),
        'behavior' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'default' => '',
        ),
        'anchor_identifier' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'default' => '',
        ),
        'anchor_target_position' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'default' => '',
        ),
        'anchor_origin_position' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'default' => '',
        ),
        'anchor_range_start' => array(
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'anchor_range_length' => array(
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'weight' => array(
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
      ),
      'primary key' => array('cid'),
    );

    return $schema;
  }

  /**
   * Given a node, generate the an array to be json_encode'd for this component, replacing tokens along the way
   *
   * @param $node
   *  Fully-loaded node to replace tokens with
   * @param &$files
   *  An array of files, passed by reference, to be sent to Apple News
   */
  public function generateJson($node, &$files = array(), &$failedStyles) {
    // Get component's layout, style, and text style
    if (!empty($this->entity->lid)) {
      $layout = entity_load_single('publish_to_apple_news_component_layout', $this->entity->lid);
    }
    if (!empty($this->entity->sid)) {
      $style = entity_load_single('publish_to_apple_news_component_style', $this->entity->sid);
    }
    if (!empty($this->entity->tsid)) {
      $textStyle = entity_load_single('publish_to_apple_news_component_text_style', $this->entity->tsid);
    }

    $json = array(
      'role' => $this->role,
    );
    
    if (!empty($this->entity->identifier)) {
      $json['identifier'] = $this->entity->identifier;
    }

    if (!empty($layout)) {
      $json['layout'] = $layout->identifier;
    }
    if (!empty($style) && !in_array($style->identifier, $failedStyles)) {
      $json['style'] = $style->identifier;
    }
    if (!empty($textStyle)) {
      $json['textStyle'] = $textStyle->identifier;
    }

    if (!empty($this->entity->behavior)) {
      $json['behavior'] = array(
        'type' => $this->entity->behavior,
      );
    }
    
    if (!empty($this->entity->anchor_identifier)) {
      $json['anchor'] = array(
        'targetComponentIdentifier' => $this->entity->anchor_identifier,
      );
      if (!empty($this->entity->anchor_target_position)) {
        $json['anchor']['targetAnchorPosition'] = $this->entity->anchor_target_position;
      }
      if (!empty($this->entity->anchor_origin_position)) {
        $json['anchor']['originAnchorPosition'] = $this->entity->anchor_origin_position;
      }
      if (!empty($this->entity->anchor_range_start)) {
        $json['anchor']['rangeStart'] = (int)$this->entity->anchor_range_start;
      }
      if (!empty($this->entity->anchor_range_length)) {
        $json['anchor']['rangeLength'] = (int)$this->entity->anchor_range_length;
      }
    }

    return $json;
  }

  /**
   * Defines the form for adding and editing this component
   */
  public function form() {
    $form = array();

    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Component name'),
      '#required' => TRUE,
      '#weight' => -100,
    );
    $form['identifier'] = array(
      '#type' => 'textfield',
      '#title' => t('Identifier'),
      '#description' => t('A unique identifier for this component, to be referenced by other components.'),
    );

    if ($this->acceptsLayoutAndStyle) {
      $form['lid'] = array(
        '#type' => 'select',
        '#options' => $this->loadEntityList('publish_to_apple_news_component_layout'),
        '#title' => t('Layout'),
        '#validated' => TRUE, // Prevents "Illegal choice" error when using AJAX
      );
      $form['sid'] = array(
        '#type' => 'select',
        '#options' => $this->loadEntityList('publish_to_apple_news_component_style'),
        '#title' => t('Style'),
        '#validated' => TRUE,
      );
    }
    
    if ($this->acceptsTextStyles) {
      $form['tsid'] = array(
        '#type' => 'select',
        '#options' => $this->loadEntityList('publish_to_apple_news_component_text_style'),
        '#title' => t('Text style'),
        '#validated' => TRUE,
      );
    }
    
    if ($this->acceptsBehavior) {
      $form['behavior'] = array(
        '#type' => 'select',
        '#options' => array(
          '' => 'Select...',
          'background_motion' => t('Background motion'),
          'background_parallax' => t('Background parallax'),
          'motion' => t('Motion'),
          'parallax' => t('Parallax'),
          'springy' => t('Springy'),
        ),
        '#title' => t('Behavior'),
        '#description' => t('A behavior defines the physics of a component and its context.'),
      );
    }
    
    $anchor_options = array(
      '' => 'Select...',
      'top' => t('Top'),
      'center' => t('Center'),
      'bottom' => t('Bottom'),
    );
    $form['anchor'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Anchor'),
      '#description' => t('Anchors can be used to align components vertically.'),
      'anchor_identifier' => array(
        '#type' => 'textfield',
        '#title' => t('Target component identifier'),
        '#description' => t('An identifier that refers to another component at the same level.'),
      ),
      'anchor_target_position' => array(
        '#type' => 'select',
        '#options' => $anchor_options,
        '#title' => t('Target anchor position'),
        '#description' => t('Defines the anchor position that should be used to position the component relative to the origin anchor position.'),
      ),
      'anchor_origin_position' => array(
        '#type' => 'select',
        '#options' => $anchor_options,
        '#title' => t('Origin anchor position'),
        '#description' => t('Defines the anchor position at origin. The originating anchor will be positioned as closely as possible to the intended target anchor position.'),
      ),
      'anchor_range_start' => array(
        '#type' => 'textfield',
        '#title' => t('Range start'),
        '#description' => t('The start index of the range of text the component should be anchored to.'),
        '#element_validate' => array('element_validate_number'),
      ),
      'anchor_range_length' => array(
        '#type' => 'textfield',
        '#title' => t('Range length'),
        '#description' => t('The length of the range of text the component should be anchored to.'),
        '#element_validate' => array('element_validate_number'),
      ),
    );

    return $form;
  }

  /**
   * Recursively generates fields and child components forms.
   * This function should only be run from publish_to_apple_news_component_form()
   */
  public function generateForm($form, $form_state) {
    $wrapperId = 'components-' . $this->entityMachineName .
      '-' . $this->entity->parent_cid . '-' . $this->entity->weight . '-form';

    if (!isset($form_state['article_type'])) {
      $form_state['article_type'] = $this->articleType;
      $form_state[$this->entityMachineName] = $this->entity;
      $form_state['entity_type'] = $this->entityMachineName;
    }
    
    $componentForm = $this->form();
    
    // Put required fields at beginning of form
    $required_fields = array();
    foreach ($componentForm as $name => $values) {
      if (!empty($values['#required'])) {
        $required_fields[$name] = $values;
        unset($componentForm[$name]);
      }
    }

    $form['#prefix'] = '<div id="' . $wrapperId . '">';
    $form['#suffix'] = '</div>';
    $form += $required_fields;
    $form += $componentForm;
    
    // Set field values from entity
    if (!empty($this->entity)) {
      foreach ($form as $key => &$element) {
        if (isset($this->entity->$key)) {
          $element['#default_value'] = $this->entity->$key;
        }
      }
    }
    
    // Set anchor default values
    if (!empty($this->entity->anchor_identifier)) {
      $form['anchor']['anchor_identifier']['#default_value'] = $this->entity->anchor_identifier;
    }
    if (!empty($this->entity->anchor_target_position)) {
      $form['anchor']['anchor_target_position']['#default_value'] = $this->entity->anchor_target_position;
    }
    if (!empty($this->entity->anchor_origin_position)) {
      $form['anchor']['anchor_origin_position']['#default_value'] = $this->entity->anchor_origin_position;
    }
    if (!empty($this->entity->anchor_range_start)) {
      $form['anchor']['anchor_range_start']['#default_value'] = $this->entity->anchor_range_start;
    }
    if (!empty($this->entity->anchor_range_length)) {
      $form['anchor']['anchor_range_length']['#default_value'] = $this->entity->anchor_range_length;
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('publish_to_apple_news_component_entity_form_submit'),
      '#ajax' => array(
        'callback' => 'publish_to_apple_news_component_entity_form_callback',
        'wrapper' => $wrapperId,
      ),
      '#weight' => 100,
    );

    if ($this->acceptsComponents && !empty($this->entity->cid) && empty($form_state['storage']['hide_add_button'])) {
      $form['add_components'] = array(
        '#type' => 'button',
        '#value' => t('Add components'),
        '#ajax' => array(
          'callback' => 'publish_to_apple_news_component_form_add_callback',
          'wrapper' => 'components-' . $this->entityMachineName . '-' .
            $this->entity->parent_cid . '-' . $this->entity->weight,
        ),
        '#weight' => 110,
      );
    }
    
    if (!empty($this->entity->cid)) {
      $form['delete'] = array(
        '#markup' => l(t('Delete'), 'admin/structure/publish-to-apple-news/components/' . $this->entityMachineName . '/' . $this->entity->cid . '/delete'),
        '#weight' => 120,
      );
    }

    return array(
      'form' => $form,
      'form_state' => $form_state,
    );
  }
  
  /**
   * Called in the component entity form's submit handler.
   * Use this to save any additional properties from the form to the component.
   */
  public function saveExtraProperties(&$component, &$form_state) {}

  /**
   * Helper function that accepts an entity type and returns a list for use in a form
   */
  private function loadEntityList($entity_type) {
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', $entity_type)
      ->propertyCondition('tid', $this->entity->tid)
      ->execute();
    
    $entity_list = array(
      '' => 'Select...',
    );
    
    if (isset($result[$entity_type])) {
      $ids = array_keys($result[$entity_type]);
      $entities = entity_load($entity_type, $ids);
      
  
      foreach ($entities as $id => $entity) {
        $entity_list[$id] = $entity->identifier;
      }
    }
    
    return $entity_list;
  }
  
  /**
   * Determine if the given text has any tokens remaining.
   * 
   * @param $text
   *  Text that has already been run through token_replace().
   *
   * @return boolean
   *  Whether or not the text has any tokens that haven't been replaced.
   */
  public function anyTokensLeft($text) {
    $tokens = token_scan($text);
    
    return !empty($tokens) ? TRUE : FALSE;
  }
  
  /**
   * Return a description with replacement patterns link to use with fields.
   */
  public function tokensDescription() {
    return t('You may use <a href="#ui-replacement-patterns" rel="modal:open">replacement patterns</a>.');
  }
  
  
  /**
   * For Gallery and Mosaic components - If user specified a field token, get all images from that field.
   * Otherwise, get items user specified.
   * 
   * @param $node
   *  The fully-loaded node.
   *
   * @param &$files
   *  An array of files to append to.
   *
   * @return array
   *  An array of gallery/mosaic items ready to be json_encode'd
   */
  public function getGalleryItems($node, &$files) {
    $json_items = array();
    if (!empty($this->entity->images_field) && substr($this->entity->images_field, 0, 6) == '[node:') {
      $field_name = substr($this->entity->images_field, 6);
      $field_name = str_replace(']', '', $field_name);
      $items = field_get_items('node', $node, $field_name);
      
      if (!empty($items)) {
        foreach ($items as $item) {
          $imageData = publish_to_apple_news_convert_image($item['uri']);
          
          if ($imageData) {
            if (!in_array($imageData['realpath'], $files)) {
              $files[] = $imageData['realpath'];
            }
            
            $json_items[] = array(
              'URL' => $imageData['bundle'],
              'caption' => $item['title'],
            );
          }
        }
      }
    }
    
    if (empty($json_items))
      return FALSE;
    
    return $json_items;
  }
}
