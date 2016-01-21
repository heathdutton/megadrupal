<?php
/**
 * @file
 * Entity metadata wrapper implementation.
 */

/**
 * Interface for extracting data.
 */
interface PHUpdatesTreeExtractable {
  /**
   * Extracts a data tree.
   *
   * @return array
   */
  public function extractTree();
}

/**
 * Update info wrapper.
 */
class PHUpdatesInfoWrapper extends EntityStructureWrapper implements PHUpdatesTreeExtractable {
  /**
   * Feed tree type.
   */
  protected $feedTreeType;

  /**
   * Sets up type information.
   */
  public function __construct($type, $data = NULL, $info = array()) {
    parent::__construct($type, $data, $info);

    // Determine feed type.
    $this->feedTreeType = ph_updates_extract_data_type($type);
  }

  /**
   * Extracts a data tree.
   *
   * @return PHUpdatesTreeWrapper
   */
  public function extractTree() {
    // Map info wrapper.
    /** @var $tree PHUpdatesTreeWrapper */
    $tree = ph_updates_data_create('tree', $this->feedTreeType);
    $treeData = array();
    $this->mapStructureProperty($treeData, $this, $this->info(), '/', $tree);
    $tree->set($treeData);
    return $tree;
  }

  /**
   * Gets a correctly wrapped property.
   */
  public function get($name) {
    return $this->select($name);
  }

  /**
   * Sets a correctly unwrapped info wrapper.
   */
  public function set($value) {
    if (is_object($value) && $value instanceof self) {
      $value = $value->dataAvailable() ? $value->value() : NULL;
    }
    return parent::set($value);
  }

  /**
   * Selects a property using a deep selector.
   *
   * @param $selector
   *   Selector for the target property as used in Rules, e.g. author:name.
   * @return EntityMetadataWrapper
   *   Property referenced by the selector, appropriated wrapped using the
   *   relevant handler as implemented in Rules.
   * @throws EntityMetadataWrapperException
   */
  public function select($selector) {
    if (!array_key_exists($selector, $this->cache)) {
      $parts = explode(':', str_replace('-', '_', $selector));
      try {
        $property = $this->selectNestedProperty($this, $parts);
        $this->cache[$selector] = $property;
      }
      catch (EntityMetadataWrapperException $e) {
        throw new EntityMetadataWrapperException('Cannot select property "' . $selector . '": ' . $e->getMessage());
      }
    }
    return $this->cache[$selector];
  }

  /**
   * Selects a property.
   */
  protected function selectProperty(EntityMetadataWrapper $wrapper, array $selector) {
    if (!$selector) {
      return $wrapper;
    }
    elseif ($wrapper instanceof PHUpdatesInfoWrapper) {
      return $wrapper->select(implode(':', $selector));
    }
    elseif ($wrapper instanceof EntityStructureWrapper || $wrapper instanceof EntityListWrapper) {
      return $this->selectNestedProperty($wrapper, $selector);
    }
    else {
      throw new EntityMetadataWrapperException('This ' . get_class($wrapper) . ' does not have nested properties to select.');
    }
  }

  /**
   * Selects a property wrapper from a structure or list.
   */
  protected function selectNestedProperty(EntityMetadataWrapper $structure, array $selector) {
    /** @var $structure EntityStructureWrapper|EntityListWrapper */
    $name = array_shift($selector);
    if (isset($name) && $info = $structure->getPropertyInfo($name)) {
      $info += array('parent' => $structure, 'name' => $name, 'langcode' => $structure->langcode);
      $property = rules_wrap_data(NULL, $info, TRUE);
      if ($property->dataAvailable()) {
        // Unwrap internal property if appropriate.
        $raw = $property->raw();
        if (is_object($raw) && $raw instanceof EntityMetadataWrapper) {
          $property = $raw;
        }
      }
      $structure->cache[$name] = $property;
      return $this->selectProperty($property, $selector);
    }
    else {
      throw new EntityMetadataWrapperException('There is no property ' . check_plain($name) . " for this " . get_class($structure) . ".");
    }
  }

  /**
   * Maps data for a single property.
   */
  protected function mapProperty(&$treeData, EntityMetadataWrapper $property, array $propertyInfo, $basePath, PHUpdatesTreeWrapper $tree) {
    $propertyInfo += array('tree mapping' => NULL);
    $mapping = $propertyInfo['tree mapping'];

    // Skip unavailable property.
    try {
      // Retrieve raw value to avoid triggering NULL data errors from lists.
      $propertyRaw = $property->raw();
      if (!isset($propertyRaw)) {
        return;
      }

      // Automatically unwrap property if a wrapper is nested.
      while (is_object($propertyRaw) && $propertyRaw instanceof EntityMetadataWrapper) {
        if (!$propertyRaw->dataAvailable() || is_null($raw = $propertyRaw->raw())) {
          return;
        }
        // Update deep references.
        $property = $propertyRaw;
        $propertyRaw = $raw;
      }
    }
    catch (EntityMetadataWrapperException $e) {
      return;
    }

    // Map using value callback.
    if (!empty($propertyInfo['tree value callback'])) {
      // Map value.
      $callback = $propertyInfo['tree value callback'];
      if (is_callable($callback)) {
        $value = $this->callValueCallback($callback, $property, $propertyInfo, $basePath, $tree);
        // Map non-NULL value.
        if (isset($value)) {
          if (!isset($mapping)) {
            $treeData = $value;
          }
          elseif (is_string($mapping)) {
            $treeData[$mapping] = $value;
          }
        }
      }
    }
    // Map property normally.
    else {
      $this->mapNormalProperty($treeData, $property, $propertyInfo, $basePath, $tree);
    }
  }

  /**
   * Calls a value callback.
   */
  protected function callValueCallback($callback, EntityMetadataWrapper $property, array $propertyInfo, $basePath, PHUpdatesTreeWrapper $tree) {
    $callbackArgs = array(
      $property,
      $propertyInfo,
      $basePath,
      $tree,
    );
    $value = call_user_func_array($callback, $callbackArgs);
    return $value;
  }

  /**
   * Maps a normal property.
   *
   * This method effectively simulates method overloading.
   */
  protected function mapNormalProperty(&$treeData, EntityMetadataWrapper $property, array $propertyInfo, $basePath, PHUpdatesTreeWrapper $tree) {
    if ($property instanceof PHUpdatesInfoWrapper) {
      $this->mapInfoProperty($treeData, $property, $propertyInfo, $basePath, $tree);
    }
    elseif ($property instanceof PHUpdatesTreeWrapper) {
      $this->mapTreeProperty($treeData, $property, $propertyInfo, $basePath, $tree);
    }
    elseif ($property instanceof EntityStructureWrapper) {
      $this->mapStructureProperty($treeData, $property, $propertyInfo, $basePath, $tree);
    }
    elseif ($property instanceof EntityListWrapper) {
      $this->mapListProperty($treeData, $property, $propertyInfo, $basePath, $tree);
    }
    else {
      $this->mapGenericProperty($treeData, $property, $propertyInfo, $basePath, $tree);
    }
  }

  /**
   * Maps feed info data.
   */
  protected function mapInfoProperty(&$treeData, PHUpdatesInfoWrapper $property, array $propertyInfo, $basePath, PHUpdatesTreeWrapper $tree) {
    $subtree = $property->extractTree();
    $this->mapTreeProperty($treeData, $subtree, $propertyInfo, $basePath, $tree);
  }

  /**
   * Maps a feed tree.
   */
  protected function mapTreeProperty(&$treeData, PHUpdatesTreeWrapper $property, array $propertyInfo, $basePath, PHUpdatesTreeWrapper $tree) {
    $mapping = $propertyInfo['tree mapping'];
    if (!isset($mapping) || is_string($mapping)) {
      if (isset($mapping)) {
        $treeData[$mapping] = $property->value();
      }
      else {
        $treeData = $property->value();
      }
      $tree->mergeFormatInfo($property->getFormatInfo(), $basePath);
    }
  }

  /**
   * Maps data for a structure.
   */
  protected function mapStructureProperty(&$treeData, EntityStructureWrapper $structure, array $structureInfo, $basePath, PHUpdatesTreeWrapper $tree) {
    $structureInfo += array('tree mapping' => NULL);
    $mapping = $structureInfo['tree mapping'];

    // Directly map composite property.
    if (is_array($mapping)) {
      foreach ($mapping as $treeKey => $selector) {
        // Get selected value in Rules style.
        $parts = explode(':', str_replace('-', '_', $selector));
        try {
          $selectedProperty = $structure;
          foreach ($parts as $part) {
            if (!$selectedProperty instanceof EntityStructureWrapper) {
              throw new EntityMetadataWrapperException("This " . get_class($structure) . " has no properties.");
            }
            $selectedProperty = $this->selectNestedProperty($selectedProperty, array($part));
          }
          // Map value.
          $value = $selectedProperty->value();
          $treeData[$treeKey] = $value;
        }
        catch (EntityMetadataWrapperException $e) {
          // Ignore invalid property.
          continue;
        }
      }
    }
    // Map subtree.
    elseif (is_string($mapping)) {
      $treeData[$mapping] = array();
      $basePath = $this->getSubtreeKey($basePath, $mapping);
      $this->mapNestedProperties($treeData[$mapping], $structure, $structureInfo, $basePath, $tree);
    }
    // Map tree directly.
    else {
      $this->mapNestedProperties($treeData, $structure, $structureInfo, $basePath, $tree);
    }
  }

  /**
   * Maps nested properties within a structure.
   */
  protected function mapNestedProperties(&$treeData, EntityMetadataWrapper $parentProperty, array $propertyInfo, $basePath, PHUpdatesTreeWrapper $tree) {
    if ($parentProperty instanceof EntityStructureWrapper) {
      foreach (array_keys($parentProperty->getPropertyInfo()) as $name) {
        /** @var $property EntityMetadataWrapper */
        $property = $this->selectNestedProperty($parentProperty, array($name));
        $propertyInfo = $property->info() + array('tree mapping' => $name);
        $propertyBasePath = $this->getSubtreeKey($basePath, $name);
        $this->mapProperty($treeData, $property, $propertyInfo, $propertyBasePath, $tree);
      }
    }
  }

  /**
   * Maps data for a list property.
   */
  protected function mapListProperty(&$treeData, EntityListWrapper $list, array $listInfo, $basePath, PHUpdatesTreeWrapper $tree) {
    $listInfo += array('tree list mapping' => NULL);
    $mapping = $listInfo['tree mapping'];

    if (!isset($mapping) || is_string($mapping)) {
      if (isset($mapping)) {
        $listTreeData = &$treeData[$mapping];
        $basePath = $this->getSubtreeKey($basePath, $mapping);
      }
      else {
        $listTreeData = &$treeData;
      }
      $listTreeData = array();
      /** @var $listItem EntityMetadataWrapper */
      foreach ($list as $index => $listItem) {
        $itemBasePath = $this->getSubtreeKey($basePath, $index);
        $listItemInfo = $listItem->info();

        // Forward list item value callback.
        if (!empty($listInfo['tree list value callback'])) {
          $listItemInfo['tree value callback'] = $listInfo['tree list value callback'];
        }

        // Map according to list item-level delegated mappings.
        if (isset($listInfo['tree list mapping'])) {
          // Translate list item mapping.
          $listItemInfo['tree mapping'] = $listInfo['tree list mapping'];
        }

        // Map list item.
        $this->mapProperty($listTreeData[$index], $listItem, $listItemInfo, $itemBasePath, $tree);
      }
    }
  }

  /**
   * Maps a generic value property.
   */
  protected function mapGenericProperty(&$treeData, EntityMetadataWrapper $property, array $propertyInfo, $basePath, PHUpdatesTreeWrapper $tree) {
    $mapping = $propertyInfo['tree mapping'];
    if (!isset($mapping)) {
      $treeData = $property->value();
    }
    elseif (is_string($mapping)) {
      $treeData[$mapping] = $property->value();
    }
  }

  /**
   * Returns a new base path given a subtree key.
   */
  protected function getSubtreeKey($basePath, $key) {
    return rtrim($basePath, '/') . '/' . $key;
  }
}

/**
 * Update data tree wrapper.
 *
 * A update data tree contains feed data and format metadata.
 */
class PHUpdatesTreeWrapper extends EntityMetadataWrapper {
  /**
   * Consolidated format info.
   * @var array
   */
  protected $formatInfo;

  /**
   * Sets up format info.
   */
  public function __construct($type, $data = NULL, $info = array()) {
    parent::__construct($type, $data, $info);

    // Load default format info if not present.
    if (!isset($this->info['format info'])) {
      $cache = rules_get_cache();
      if (isset($cache['data_info'][$type])) {
        $cache['data_info'][$type] += array('format info' => array());
        $this->info['format info'] = $cache['data_info'][$type]['format info'];
      }
    }
    $this->formatInfo = $this->consolidateFormatInfo($this->info['format info']);
  }


  /**
   * Sets a correctly unwrapped tree wrapper.
   */
  public function set($value) {
    if (is_object($value) && $value instanceof self) {
      // Unwrap value and update the format info from value.
      $this->formatInfo = $value->getFormatInfo();
      return parent::set($value->value());
    }
    return parent::set($value);
  }

  /**
   * Updates the format info for this property in the parent wrapper.
   */
  protected function updateParent($value) {
    parent::updateParent($value);
    if (isset($this->info['parent'])) {
      $parentPropertyInfo = &$this->info['parent']->refPropertyInfo();
      $parentPropertyInfo['properties'][$this->info['name']]['format info'] = $this->getFormatInfo();
    }
  }

  /**
   * Retrieves format info.
   *
   * @param $format
   *   Format name. If not specified, info for all formats are returned, keyed
   *   by format name.
   * @return array
   *   Format info.
   */
  public function getFormatInfo($format = '') {
    $info = $this->formatInfo;

    // Filter info by format.
    if ($format) {
      // Return format-specific info.
      return isset($info[$format]) ? $info[$format] : array();
    }

    // Return all info.
    return $info;
  }

  /**
   * Consolidates format info.
   */
  protected function consolidateFormatInfo(array $formatInfo) {
    // Pop general format info.
    $generalInfo = array();
    if (isset($formatInfo[''])) {
      $generalInfo = $formatInfo[''];
      unset($formatInfo['']);
    }

    // Compile info by path.
    $baseInfo = array_fill_keys(array_keys(ph_updates_get_format_info()), $generalInfo);
    $info = $this->getMergedFormatInfo($baseInfo, $formatInfo);

    return $info;
  }

  /**
   * Merges new format info into this tree.
   *
   * @param array $newInfo
   *   Format info to merge.
   * @param $newBasePath
   *   Base path to merge info under.
   */
  public function mergeFormatInfo(array $newInfo, $newBasePath = '/') {
    $this->formatInfo = $this->getMergedFormatInfo($this->formatInfo, $newInfo, $newBasePath);
  }

  /**
   * Returns merged format info.
   */
  protected function getMergedFormatInfo(array $baseInfo, array $newInfo, $newBasePath = '/') {
    // Union new info with base info.
    foreach ($newInfo as $format => $newFormatInfo) {
      // Add missing format (for completeness).
      $baseInfo += array($format => array());
      $baseFormatInfo = &$baseInfo[$format];
      // Merge new info by path.
      foreach ($newFormatInfo as $path => $newPathInfo) {
        // Adjust path using new base.
        if ($newBasePath != '/') {
          $path = '/' . trim(rtrim($newBasePath, '/') . '/' . ltrim($path, '/'), '/');
        }
        // Merge into base format info.
        $baseFormatInfo += array($path => array());
        $baseFormatInfo[$path] = array_merge($baseFormatInfo[$path], $newPathInfo);
      }
    }

    return $baseInfo;
  }
}
