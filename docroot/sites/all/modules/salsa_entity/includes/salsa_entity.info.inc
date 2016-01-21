<?php

/**
 * @file
 * Metadata controllers for the Salsa entity types.
 */


/**
 * Default Salsa Metadata controller.
 */
class SalsaEntityMetadataController extends EntityDefaultMetadataController {

  static $customColumns;

  public function __construct($entityType) {
    parent::__construct($entityType);
    $this->salsaType = str_replace('salsa_', '', $entityType);
  }

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   */
  public function entityPropertyInfo() {
   // @todo: Add a is connected check to SalsaAPIInterface.
   try {
     return $this->buildEntityPropertyInfo();
   } catch (SalsaQueryException $e) {
     watchdog_exception('salsa_entity', $e);
   }
   return array();
  }

  /**
   * Builds the entity property information based on information from Salsa.
   *
   * @return array
   *   Entity property info.
   */
  protected function buildEntityPropertyInfo() {
    $cid = 'describe2:' . $this->salsaType;
    if (!($salsa_info = salsa_entity_load_cache_item($cid))) {
      $query_info = array(
        '#script' => 'describe2.sjs',
        '#tables' => $this->salsaType,
      );
      if ($salsa_info = salsa_api_query($query_info)) {
        salsa_entity_save_cache_item($cid, $salsa_info);
      }
    }

    $info = array();
    if (is_array($salsa_info)) {
      foreach ($salsa_info[$this->salsaType]['item'] as $item) {
        // Base info
        $label = isset($item['displayName']) ? $item['displayName'] : $item['label'];
        $info[$this->type]['properties'][$item['name']] = array(
          'label' => t($label),
          'type' => $this->_entity_metadata_convert_schema_type($item['type']),
          'salsa_type' => $item['type'],
        );

        // Reference linked linked entities.
        if (strpos($item['name'], '_KEY') !== FALSE) {
          $target_type = substr($item['name'], 0, strpos($item['name'], '_KEY'));
          $info[$this->type]['properties'][$item['name']]['type'] = 'salsa_' . $target_type;
        }

        // If $item is type of enum or set, use callback method to get options.
        if ((strpos($item['type'], 'enum') === 0 || strpos($item['type'], 'set') === 0) && !empty($item['values'])) {
          $info[$this->type]['properties'][$item['name']]['options list'] = 'salsa_entity_property_info_options';
          if (!empty($item['values'])) {
            foreach (explode(',', $item['values']) as $value) {
              $info[$this->type]['properties'][$item['name']]['options'][$value] = t($value);
            }
          }
        }

        // If $item is a custom column, get the options.
        foreach ($this->getCustomColumns() as $column) {
          // Ignore columns without a name.
          if (empty($column->name)) {
            continue;
          }

          if ($item['name'] == $column->name) {
            $info[$this->type]['properties'][$item['name']]['#salsa_custom'] = TRUE;
            if ($item['type'] == 'enum' && !empty($column->options)) {
              $info[$this->type]['properties'][$item['name']]['options'] = array();
              foreach ($column->options as $option) {
                $info[$this->type]['properties'][$item['name']]['options'][$option->value] = t($option->label);
              }
            }
          }
        }
      }
      // Add the url property.
      $info[$this->type]['properties']['url'] = array(
        'label' => t("URL"),
        'description' => t("The URL of the entity."),
        'getter callback' => 'entity_metadata_entity_get_properties',
        'type' => 'uri',
        'computed' => TRUE,
      );
    }

    return $info;
  }

  /**
   * Overrides EntityDefaultMetadataController::_entity_metadata_convert_schema_type().
   */
  public function _entity_metadata_convert_schema_type($type) {
    // @todo: Do we need this configurable?
    $types_map = array(
      'text' => array(
        'varchar',
        'text',
        'blob',
        'enum',
        'set',
      ),
      'integer' => array(
        'int',
        'tinyint',
      ),
      'decimal' => array(
        'float',
        'double',
        'decimal',
      ),
      'date' => array(
        'datetime',
        'date',
        'timestamp',
      ),
      'boolean' => array(
        'bool',
      ),
    );

    // Find the right entity type for submitted salsa type.
    foreach ($types_map as $entity_type => $salsa_types) {
      foreach ($salsa_types as $findme) {
        if (strpos($type, $findme) !== FALSE) {
          return $entity_type;
        }
      }
    }

    // Default type
    return 'text';
  }

  /**
   * Method that loads custom fields.
   */
  private function getCustomColumns() {
    if (self::$customColumns === NULL) {
      $cid = 'custom_column';
      if (!(self::$customColumns = salsa_entity_load_cache_item($cid))) {
        self::$customColumns = salsa_api_get_objects('custom_column');

        if (!empty(self::$customColumns)) {
          $options = salsa_api_get_objects('custom_column_option');
          foreach ($options as $option) {
            if ($option->isDisplayed == 'true') {
              self::$customColumns[$option->custom_column_KEY]->options[$option->custom_column_option_KEY] = $option;
            }
          }
          salsa_entity_save_cache_item($cid, self::$customColumns);
        }
      }
    }

    return self::$customColumns;
  }

}
