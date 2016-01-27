<?php

/**
 * Description
 */
class EntityCollectionRowOptions extends EntityCollectionRow {

  /**
   * Construct an entity collection row object
   *
   * @param type $entity_collection
   */
  public function __construct($entity_collection = NULL, $settings = array()) {
    $this->entity_collection = $entity_collection;

    $this->settings = $this->defaultSettings();
    $this->settings['default_style'] = $settings;
  }

  /**
   * Always use per row styling.
   */
  public function useStylePerRow() {
    return TRUE;
  }

  /**
   * Get options for all styles that can be chosen for a particular row.
   *
   * @param string $entity_types
   */
  public function getOptions($entity_type) {
    $options = $this->getBaseOptions($entity_type);

    $context = $this->getAlterContext();
    drupal_alter('entity_collection_row_options', $options, $context);

    return $options;
  }

  /**
   * Get all available options.
   */
  public function getAllOptions() {
    $options = array();
    foreach ($this->entity_collection->settings['allowed_bundles'] as $type => $bundles) {
      if (count($bundles)) {
        $options[$type] = $this->getBaseOptions($type);
      }
    }

    $context = $this->getAlterContext();
    drupal_alter('entity_collection_all_row_options', $options, $context);

    return $options;
  }

  /**
   *
   */
  public function settingsForm(&$form) {
    $available_options = $this->getAllOptions();

    $settings = $this->settings;

    $form['descr'] = array(
      '#markup' => t('You can set all the default display options per every content type.'),
      '#prefix' => '<p class="form-descr">',
      '#suffix' => '</p>',
    );

    foreach ($available_options as $entity_type => $entity_options) {
      $form[$entity_type] = array(
        '#type' => 'fieldset',
        '#title' => ucwords($entity_type),
        '#description' => t('Options for Entity Type: @type', array('@type' => ucwords($entity_type))),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        // '#access' => user_access(''),
      );
      foreach ($entity_options as $opt_name => $options) {
        $val = empty($settings['default_style'][$entity_type][$opt_name]) ? -1 : $settings['default_style'][$entity_type][$opt_name];
        $form[$entity_type][$opt_name] = $options;
        $form[$entity_type][$opt_name]['#default_value'] = $val;
      }
    }
  }

  /**
   * Act before a row is inserted.
   */
  public function itemPreAdd(EntityCollectionTreeNode$item) {
    if ( !empty($this->settings['default_style']) && empty($item->style) ) {
      $item->style = $this->settings['default_style'][$item->type];
    }
  }

  /**
   * Build a row in an entity collection.
   *
   * @param EntityCollection $collection
   * 	The entity collection in which the content is.
   * @param EntityCollectionTreeNode $item
   * 	The item to render.
   * @param string $langcode
   *  The language code to use.
   */
  public function build(EntityCollection$collection, EntityCollectionTreeNode$item, $langcode = NULL) {
    $style = $this->getDefaultSettings();
    if (!empty($item->style)) {
      $style = array_merge($style, $item->style);
    }
    $classes = $this->getWrapperClasses($style, $item->type);

    if ($item->type == 'entity_collection') {
      $build = $item->content->buildContent($item->style['view_mode'], $langcode);
    }
    else {
      $item->content->collection_style = $item->style;
      $item->content->collection_grid = isset($item->grid_settings) ? $item->grid_settings : array();

      $entities = array($item->entity_id => $item->content);
      $build = entity_view($item->type, $entities, $item->style['view_mode'], $langcode);
    }
    
    $build[$item->type][$item->entity_id]['#attributes']['class'] =
      $this->getWrapperClasses($style, $item->type, FALSE);

    return array(
      '#prefix' => '<div class="' . $classes . '">',
      '#markup' => drupal_render($build),
      '#suffix' => '</div>',
    );
  }

  /********************** Protected Functions **********************/

  /**
   * Loads the view modes for the given entity
   */
  protected function getAllowedViewModes($entity_type) {
    $entity_info = entity_get_info($entity_type);
    $view_modes = array();
    foreach ($entity_info['view modes'] as $mode => $info) {
      $view_modes[$mode] = $info['label'];
    }

    return $view_modes;
  }

  /**
   * Builds an array containing the default options for every row.
   */
  protected function getDefaultSettings() {
    return array(
      'box_decoration' => 0,
      'title_font_weight' => 0,
      'title_font_size' => 0,
      'title_decoration' => 0,
      'view_mode' => 'teaser',
    );
  }

  /********************** Private Functions **********************/

  private function getWrapperClasses($options, $entity_type, $html=TRUE) {
    array_walk($options,
    function (&$item, $key) {
        if (!empty($item) && (bool)$item != FALSE) {
          $text = strtolower(str_ireplace('_', '-', $key));
          $text .= '-' . strtolower(str_ireplace('_', '-', $item));
          $item = $text;
      }
      else {
          $item = FALSE;
      }
    }
    );
    $classes_array = array_filter($options);
    return ($html) ? implode(' ',$classes_array) : array_values($classes_array);
  }

  /**
   *
   */
  private function getBaseOptions($entity_type) {
    $form_items = array();

    $form_items['box_decoration'] = array(
      '#type' => 'select',
      '#title' => t('Box Decoration'),
      '#options' => array(
        0 => t("None"),
        'bg_info' => t("Info"),
        'bg_alert' => t("Alert"),
        'bg_highlight' => t("Highlight"),
      ),
    );

    $form_items['title_font_weight'] = array(
      '#type' => 'select',
      '#title' => t('Title Font weight'),
      '#options' => array(
        0 => t("Normal"),
        'bold' => t("Bold"),
        'light' => t("Light"),
      ),
    );

    $form_items['title_font_size'] = array(
      '#type' => 'select',
      '#title' => t('Title Font size'),
      '#options' => array(
        0 => t("Normal"),
        'xlarge' => t("Extra Large"),
        'large' => t("Large"),
        'small' => t("Small"),
        'xsmall' => t("Extra Small"),
      ),
    );

    $form_items['title_decoration'] = array(
      '#type' => 'select',
      '#title' => t('Title Decoration'),
      '#options' => array(
        0 => t("None"),
        'highlight' => t("Highlight"),
        'underline' => t("Underline"),
      ),
      '#default_value' => -1,
    );
    
    $form_items['view_mode'] = array(
      '#type' => 'select',
      '#title' => t('View Mode'),
      '#options' => $this->getAllowedViewModes($entity_type),
      '#default_value' => -1,
    );

    return $form_items;
  }
  
  /**
   * Returns the current entity collection information to pass to the alter
   *  functions, so it can identify the current Entity Collection.
   */
  private function getAlterContext()
  {
    if (  $this->entity_collection != NULL ) {
      return array(
        'name' => $this->entity_collection->name,
        'row' => $this->entity_collection->row,
        'style' => $this->entity_collection->style,
      );
    }
    
    return NULL;
  }
}
