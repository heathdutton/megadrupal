<?php
/**
 * @file
 * Definition of the API for rows plugins.
 */

/**
 * Interface common to all row style plugins.
 */
interface EntityCollectionRowInterface {

  /**
   * Get options for all styles that can be chosen for a particular row.
   * @param string $entity_types
   */
  public function getOptions($entity_type);

  /**
   * Get all available options.
   */
  public function getAllOptions();

  /**
   * Indicate if we allow one style per row or not.
   */
  public function useStylePerRow();

  /**
   * Act before a row is inserted.
   */
  public function itemPreAdd(EntityCollectionTreeNode $item);

  /**
   * Get a style for a particular entity.
   */
  public function getDefaultStyle($entity_type, $entity);

  /**
   * Build a row in an entity collection.
   * @param EntityCollection $collection
   * 	The entity collection in which the content is.
   * @param EntityCollectionTreeNode $item
   * 	The item to render.
   * @param string $langcode
   *  The language code to use.
   */
  public function build(EntityCollection $collection, EntityCollectionTreeNode $item, $langcode = NULL);
}

/**
 * An abstract class implementing some of the row interface.
 */
abstract class EntityCollectionRow implements EntityCollectionRowInterface {
  protected $entity_collection;

  /**
   * Construct an entity collection row object
   * @param type $entity_collection
   */
  public function __construct($entity_collection = NULL, $settings = array()) {
    $this->entity_collection = $entity_collection;
    if (!isset($settings)) {
      $settings = array();
    }
    $this->settings = $settings + $this->defaultSettings();
  }

  /**
   * Indicate if we allow one style per row or not.
   */
  public function useStylePerRow() {
    return $this->settings['style_per_row'];
  }

  public function defaultSettings() {
    return array(
      'style_per_row' => TRUE,
      'default_style' => array()
    );
  }

  public function getDefaultStyle($entity_type, $entity) {
    if (isset($this->settings['default_style'][$entity_type])) {
      return $this->settings['default_style'][$entity_type];
    }
    return array();
  }

  /**
   * Act when a new row is inserted.
   */
  public function itemPreAdd(EntityCollectionTreeNode $item) {
    // Add a default style if style per row is not enabled.
    if (isset($this->settings['default_style'][$item->type]) && (!$this->settings['style_per_row'] || !isset($item->style))) {
      $item->style['view_mode'] = $this->settings['default_style'][$item->type];
    }
  }

  /**
   * Get the EntityCollectionRow style attached to this collection.
   * @param EntityCollection $collection
   * @return EntityCollectionRowInterface
   *   The row plugin to use in this collection.
   */
  public static function getRow(EntityCollection $collection) {
    ctools_include('plugins');
    if (isset($collection->row)) {
      $plugin = ctools_get_plugins('entity_collection', 'plugins', $collection->row);
      if (isset($plugin)) {
        return new $plugin['entity_collection_row']['class']($collection,
          isset($collection->settings['row_settings']) ? $collection->settings['row_settings'] : NULL);
      }
    }
    return NULL;
  }

  /**
   * Get all row plugins.
   */
  public static function getRows() {
    ctools_include('plugins');
    $plugins = ctools_get_plugins('entity_collection', 'plugins');
    // Get all available storage backends.
    $row_plugins = array();
    foreach ($plugins as $name => $info) {
      if (isset($info['title']) && isset($info['entity_collection_row'])) {
        $row_plugins[$name] = $info;
      }
    }
    return $row_plugins;
  }

  public function settingsForm(&$form) {
    $settings = $this->settings;
    $options = $this->getAllOptions();

    $form['style_per_row'] = array(
      '#title' => t('Style per row'),
      '#type' => 'checkbox',
      '#description' => t('Let the user select a different style for each row.'),
      '#default_value' => $this->settings['style_per_row'],
    );
    $form['default_style'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default style'),
      '#description' => t('The default style will be used for new entries.'),
      '#tree' => TRUE,
    );

    foreach ($options as $entity_type => $type_options) {
      $entity_type_name = ucwords(str_replace('_',' ',$entity_type));
      $form['default_style'][$entity_type] = array(
        '#type' => 'fieldset',
        '#title' => $entity_type_name,
        // '#description' => t('Options for Entity Type: <strong>@type</strong>', array('@type' => $entity_type_name)),
        '#collapsible' => (count($type_options) > 1),
        '#collapsed' => (count($type_options) <= 1),
      );
      foreach ($type_options as $key => $form_item) {
        $form['default_style'][$entity_type][$key] = $form_item;
        $default = isset($settings['default_style'][$entity_type][$key]) ? $settings['default_style'][$entity_type][$key] : FALSE;
        $form['default_style'][$entity_type][$key]['#default_value'] = $default;
      }
    }
  }

  /**
   * Get all row styles formatted for #options
   * @return array
   *   An array of styles.
   */
  public static function getRowOptions() {
    ctools_include('plugins');
    $plugins = EntityCollectionRow::getRows();
    // Get all available storage backends.
    $options = array();
    foreach ($plugins as $name => $info) {
      if (isset($info['title']) && isset($info['entity_collection_row'])) {
        $options[$name] = $info['title'];
      }
    }
    return $options;
  }
}
