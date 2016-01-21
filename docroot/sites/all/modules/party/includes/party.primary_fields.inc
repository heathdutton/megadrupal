<?php
/**
 * @file
 * Primary field related functions and callbacks.
 */

/**
 * Helper class for primary fields.
 *
 * Primary fields provide a means to cache data from potentially multiple data
 * sets and sources into a single field on the party. This can then be used for
 * searching, indexing etc.
 *
 * An example of this is the party's label. Parties with different hats may have
 * different data sets, for example an individual and organisation. While the
 * individual's name is stored in a name field on the individual profile, the
 * organisation's name could be stored in a text field on the organisation
 * profile. The label primary fields can be set to pull from both and will run
 * through all the potential sources until a valid (non-NULL) value has been
 * found.
 *
 * In addition, sources can use callbacks to deal with type conversion or
 * rendering. For example, the User's UID has a callback that will convert a UID
 * into the return from format_username().
 *
 * Primary fields are configured from the 'Manage fields' page for a party. Some
 * properties are provided, such as label and email and other may be added,
 * either by other modules or through the Field UI.
 *
 * To add a primary field through the Field UI, add a field of the desired type
 * and use the 'Primary field' widget type. This will then allow you to select
 * sources that match the target primary field's type.
 *
 * Modules can add primary fields using hook_entity_property_info(). Any
 * property with the key 'party primary field' set to TRUE will appear on the
 * party's Manage fields page for configuration. Properties used as primary
 * fields must have a 'setter callback' defined or an Exception will be thrown.
 *
 * Modules wishing to configure primary fields on install should add their
 * settings to the 'party_primary_fields' variable. See
 * PartyPrimaryFields::$fields for the data structure used. Modules can also
 * implement hook_party_primary_fields_fields_alter() to add primary fields from
 * other sources or to override the configured settings for anything using the
 * built in configuration.
 *
 * Sources can be added by declaring a property on the party or a data set via
 * hook_entity_property_info(). It is advised that 'type' is set for data type
 * matching. There will need to be a 'getter callback', otherwise an Exception
 * will be thrown.
 *
 * Callbacks can be added to detected properties using
 * hook_party_primary_fields_sources_alter(). See PartyPrimaryFields::$sources
 * for the data structure.
 *
 * @see hook_party_primary_fields_fields_alter()
 * @see hook_party_primary_fields_sources_alter()
 * @see party_field_extra_fields()
 */
class PartyPrimaryFields {

  /**
   * Cache of primary fields.
   * @var array
   *   An array of primary field information. Outer keys are target properties.
   *   Values are an array of sources each of which are an array of:
   *   - data_set: The data set name the property is on. 'party' is a special case
   *     which allows a party to refer to itself.
   *   - property: The property we are pulling from.
   *   - value: Optionally provide a sub-key of the property to use. The structure
   *     must be described with hook_entity_property_info().
   *   - callback: Optionally the key of a callback to use. The callback must be
   *     defined in the source's callbacks.
   *   - weight: The weight of this source. Lower weights are processed first.
   */
  static protected $fields;

  /**
   * Cache of primary field sources.
   * @var array
   *   An array of all the sources, grouped by data set. Outer keys are the data
   *   set name. Values are an array of:
   *   - All keys from self::$data_sets.
   *   - sources: An array of sources contained in this data set. Keys are
   *     an identifier (normally data_set_name:property[:value] who's values are
   *     arrays of:
   *     - label: The label of the property.
   *     - option label: The label and name of the property, suitable for options.
   *     - type: The data type as known by the entity property info.
   *     - field: Whether this is a field.
   *     - data_set: The data set this property is on.
   *     - property: The property on the data set entity.
   *     - value: The key of the structure of the property. If NULL, the property
   *       is taken as whole.
   *     - callbacks: Array of possible callbacks. Each one should have a unique
   *       key and it's value should be and array of:
   *       - label: Human readable label.
   *       - callback: A callable. It will receive the following arguments:
   *         - value: The value of the source field.
   *         - target: The name of the target property/field.
   *         - info: Metadata about the target property.
   *         - file: A file to include for the callback. This can either be a
   *           a string path relative to DRUPAL_ROOT or an array of (see
   *           module_load_include() arguments for key descriptions):
   *           - type
   *           - module
   *           - name
   *           A recommended location for callbacks is MODULE.party.inc.
   *       - type: The data type this callback returns.
   */
  static protected $sources;

  /**
   * Cache of data set info including our spoofed party.
   * @var array
   *   An array of data set info outer keyed by data set. Key/value pairs are:
   *   - set_name: The name of the data set. 'party' is a special case which
   *     allows a party to refer to itself.
   *   - entity type: The entity type of the data set.
   *   - entity bundle: The bundle of the data set entity.
   *   - label: The label of the data set.
   *   - option label: The label and name of the data set, suitable for options.
   */
  static protected $data_sets;

  /**
   * Clears the field and source cache.
   */
  public static function clearCaches() {
    // Clear our cache.
    cache_clear_all('party:primary_fields:', 'cache', TRUE);

    // Reset our statics.
    self::$fields = NULL;
    self::$sources = NULL;
  }

  /**
   * Get hold of primary field information.
   *
   * Primary fields are used to cache information from multiple potential data set
   * sources into a single property (or field) on the party.
   *
   * The Primary Field widget allows you to create primary fields via the Field UI
   * by adding fields to the party with the Primary Field widget.
   *
   * @param string $target
   *   Optionally provide a target property to retrieve the sources for.
   *
   * @return array
   *   If $target is NULL, outer keys are target properties. See
   *   PartyPrimaryFields::$primary_fields for a description of the structure.
   */
  public static function getFields($target = NULL) {
    if (!isset(self::$fields)) {
      self::buildFields();
    }

    if ($target) {
      return isset(self::$fields[$target]) ? self::$fields[$target] : array();
    }

    return self::$fields;
  }

  /**
   * Get hold of primary field source information.
   *
   * @param string $data_set
   *   Optionally provide a data set retrieve the sources for.
   *
   * @return FALSE||array
   *   Returns source information or FALSE if the requested information is not
   *   available. See PartyPrimaryFields::$sources for the data structure. If
   *   $property is given this will be a single source. Otherwise if $data_set
   *   is given it will be an array of sources. If $data_set is NULL, the entire
   *   source array will be returned.
   */
  public static function getSources($data_set = NULL) {
    if (!isset(self::$sources)) {
      self::buildSources();
    }

    // If no data set is given, return the whole lot.
    if (!isset($data_set)) {
      return self::$sources;
    }

    return isset(self::$sources[$data_set]) ? self::$sources[$data_set]['sources'] : FALSE;
  }

  /**
   * Get hold of primary field sources by type.
   *
   * @param string|array $types
   *   Either a single type or an array of types we want suitable sources for.
   * @param bool $all_types
   *   Indicate whether you want to check against the 'all types' key which
   *   includes callback types Defaults to TRUE.
   *
   * @return array
   *   An array of source information filtered by type. See
   *   PartyPrimaryFields::$sources for the data structure.
   */
  public static function getSourcesByType($types, $all_types = TRUE) {
    // Get our full list of sources.
    $sources = self::getSources();

    // Loop over our sources and remove items we don't want.
    foreach ($sources as &$data_set) {
      foreach ($data_set['sources'] as $key => $info) {
        if (!self::sourceTypeMatch($types, $info, $all_types)) {
          unset($data_set['sources'][$key]);
        }
      }
    }

    // Run our cleanup.
    self::cleanSources($sources);

    return $sources;
  }

  /**
   * Get the info of a particular source.
   *
   * @param string|array $source
   *   Either a data set name or a partial source info array for the required
   *   source. See PartyPrimaryFields::$sources for the data structure.
   *   'data_set' and 'property' are required.
   * @param string $property
   *   Optionally provide a property to retrieve the source info for. $data_set
   *   is required if $property is given. Only used if $source is a data set
   *   string.
   * @param string $value
   *   Optionally provide a value on a property to retrieve the source info for.
   *   $property is required if $value is given. Only used if $source is a data
   *   set string.
   *
   * @return FALSE||array
   *   The full source info for the given partial source or FALSE if not
   *   available.
   */
  public static function getSource($source, $property = NULL, $value = NULL) {
    if (is_string($source)) {
      $source = array(
        'data_set' => $source,
        'property' => $property,
        'value' => $value,
      );
    }

    // Get hold of sources on the data set.
    $sources = self::getSources($source['data_set']);
    if (!$sources) {
      return FALSE;
    }

    // Build our key for returning the source info.
    $key = self::getSourceKey($source);
    return isset($sources[$key]) ? $sources[$key] : FALSE;
  }

  /**
   * Build a key for a given source.
   *
   * @param array $source_info
   *   The source info for the required source. See PartyPrimaryFields::$sources
   *   for the data structure. 'data_set' and 'property' are required.
   * @param bool $ignore_value
   *   Optionally ignore the value key. This can be used to find the parent
   *   property of a source that uses a value key.
   *
   * @return string
   *   A key for the source property.
   */
  public static function getSourceKey($source_info, $ignore_value = FALSE) {
    // Get our information for the key in the right order.
    $key_info = array(
      'data_set' => $source_info['data_set'],
      'property' => $source_info['property'],
    );
    if (!$ignore_value && ($source_info['value'])) {
      $key_info['value'] = $source_info['value'];
    }

    // Build and return our key.
    return implode(':', $key_info);
  }

  /**
   * Retrieve property information for a specific entity/bundle.
   *
   * @param string $entity_type
   *   The entity type we want information for.
   * @param string $bundle
   *   The bundle we want information for.
   * @param string $property
   *   Optionally only return information for a specific proprety.
   *
   * @return FALSE|array
   *   FALSE if the requested information is not available. If $property is
   *   given, the metadata for that property, otherwise an array of property
   *   metadata keyed by property.
   */
  public static function getPropertyInfo($entity_type, $bundle, $property = NULL) {
    $entity = entity_create_stub_entity($entity_type, array(
      0 => NULL,
      2 => $bundle,
    ));
    $wrapper = entity_metadata_wrapper($entity_type, $entity);
    $info = array();
    foreach (array_keys($wrapper->getPropertyInfo()) as $key) {
      $info[$key] = $wrapper->getPropertyInfo($key);
    }

    if ($property) {
      return isset($info[$property]) ? $info[$property] : FALSE;
    }
    else {
      return $info;
    }
  }

  /**
   * Check whether the types match.
   *
   * @param string|array $types
   *   Either a single type or an array of types we want suitable sources for.
   * @param array $source
   *   The source info for the source we are checking. See
   *   PartyPrimaryFields::$sources for the data structure.
   * @param bool $all_types
   *   Indicate whether you want to check against the 'all types' key which
   *   includes callback types. Defaults to TRUE.
   *
   * @return bool
   *   Whether the source matches the given type(s).
   */
  public static function sourceTypeMatch($types, $source, $all_types = TRUE) {
    // Make sure $types is an array.
    $types = (array) $types;

    // If $all_types we want to check the 'all types' key which is an array.
    if ($all_types) {
      return (bool) array_intersect($types, $source['all types']);
    }
    // Otherwise we are just checking the basic 'type'.
    else {
      return in_array($source['type'], $types);
    }
  }

  /**
   * Get data set info including the spoofed party.
   *
   * @param string $data_set
   *   Optionally return the info for a single data set.
   *
   * @return FALSE|array
   *   Data set info array. If $data_set is not given, it is outer keyed by data
   *   set name. If it is set and it is not available, FALSE is returned.
   */
  public static function getDataSetInfo($data_set = NULL) {
    if (!isset(self::$data_sets)) {
      self::$data_sets = array(
        'party' => array(
          'set_name' => 'party',
          'label' => 'Party',
          'entity type' => 'party',
          'entity bundle' => 'party',
        ),
      );
      self::$data_sets += party_get_data_set_info();
    }

    if ($data_set) {
      return isset(self::$data_sets[$data_set]) ? self::$data_sets[$data_set] : FALSE;
    }

    return self::$data_sets;
  }

  /**
   * Execute a callback on a value.
   */
  public static function executeCallback($value, $source, $target) {
    // Check we want to execute callback.
    if (isset($source['callback'])) {
      // Get our source info and check the callback exists.
      $source_info = self::getSource($source);
      if (isset($source_info['callbacks'][$source['callback']])) {
        // Get the callback info.
        $callback = $source_info['callbacks'][$source['callback']];

        // Load an include if necessary.
        if (isset($callback['file'])) {
          if (is_array($callback['file'])) {
            module_load_include($callback['file']['type'], $callback['file']['module'], $callback['file']['name']);
          }
          else {
            require_once DRUPAL_ROOT . '/' . $callback['file'];
          }
        }

        // If the callback is valid, execute it.
        if (is_callable($callback['callback'])) {
          $target_info = self::getPropertyInfo('party', 'party', $target);
          $value = call_user_func($callback['callback'], $value, $target, $target_info, $source_info);
        }
      }
    }

    return $value;
  }

  /**
   * Set up a source form under the given element.
   *
   * This is typically called on a fieldset.
   *
   * @param array $element
   *   The container or fieldset element or form we are adding the source table
   *   to. In addition to it's normal keys, this element should have:
   *   - #target: The name of the target property.
   *   - #source_types: Optionally provide a filter for source types. If not
   *     provided it will be worked out from the given target property.
   *   - #parents: As this function does not deal with any submission of the
   *     user's input, #parents should be set to indicate where the submitted
   *     data should appear in $form_state['values']. If not set, array() will
   *     be used resulting in $form_state['values'] having 'sources' as a root
   *     key.
   * @param array $form_state
   *   The form state.
   * @param array $form
   *   The whole form.
   */
  public static function sourceForm(&$element, &$form_state, &$form) {
    // Set up our include and submission handler.
    form_load_include($form_state, 'inc', 'party', 'party.admin');
    $form['#submit'][] = 'party_primary_fields_clear_caches';

    // Make sure the element has the required properties.
    $element['#tree'] = TRUE;
    if (!isset($element['#parents'])) {
      $element['#parents'] = array();
    }

    // Get hold of info about the target.
    if (!array_key_exists('#source_types', $element)) {
      $target_info = self::getPropertyInfo('party', 'party', $element['#target']);
      $element['#source_types'] = isset($target_info['type']) ? array($target_info['type']) : NULL;
    }
    elseif (is_string($element['#source_types'])) {
      $element['#source_types'] = (array) $element['#source_types'];
    }

    // Get hold of the current state of the sources.
    if (isset($form_state['values'])) {
      $sources = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
    }
    if (!isset($sources) || !is_array($sources)) {
      $sources = $element['#default_value'];
    }

    // Make sure the sources are sorted by weight.
    uasort($sources, 'drupal_sort_weight');

    // Construct our draggable table.
    $element['table'] = array(
      '#theme' => 'table',
      '#header' => array(
        'property_desc' => t('Property'),
        'data_set_desc' => t('Data set'),
        'callback' => t('Callback'),
        'weight' => t('Weight'),
        'remove' => t('Remove'),
      ),
      '#empty' => t('No sources are selects.'),
      '#attributes' => array(
        'class' => array('field-ui-overview'),
        'id' => 'field-sources',
      ),
      '#parents' => $element['#parents'],
      '#pre_render' => array('party_primary_fields_source_table_pre_render'),
    );
    drupal_add_tabledrag('field-sources', 'order', 'sibling', 'source-weight');

    // Build our rows which will be put into the table after building.
    foreach ($sources as $key => $source) {
      // Get hold of our source info.
      $data_set_info = self::getDataSetInfo($source['data_set']);
      $source_info = self::getSource($source);
      $property_key = self::getSourceKey($source);
      $property_key = substr($property_key, strpos($property_key, ':') + 1);

      $row = array(
        '#element_validate' => array('party_primary_fields_source_table_source_cleanup'),
        'data_set' => array(
          '#type' => 'value',
          '#value' => $source['data_set'],
        ),
        'property' => array(
          '#type' => 'value',
          '#value' => $source['property'],
        ),
        'value' => array(
          '#type' => 'value',
          '#value' => $source['value'],
        ),
        'property_desc' => array('#markup' => format_string('@label <small>[@name]</small>', array(
          '@label' => $source_info['label'],
          '@name' => $property_key,
        ))),
        'data_set_desc' => array('#markup' => format_string('@label <small>[@name]</small>', array(
          '@label' => $data_set_info['label'],
          '@name' => $data_set_info['set_name'],
        ))),
        'weight' => array(
          '#type' => 'weight',
          '#default_value' => $source['weight'],
          '#delta' => 50,
          '#title_display' => 'invisible',
          '#title' => t('Weight for @label', array('@label' => $source_info['label'])),
          '#attributes' => array('class' => array('source-weight')),
        ),
        'callback' => array(),
        'remove' => array(
          '#type' => 'submit',
          '#value' => t('Remove'),
          '#name' => 'remove_source:' . $key,
          '#submit' => array('party_primary_fields_source_table_submit'),
        ),
      );

      // Find any valid callbacks.
      $callbacks = array();
      if (!empty($source_info['callbacks'])) {
        foreach ($source_info['callbacks'] as $key => $info) {
          if (!isset($element['#source_types']) || in_array($info['type'], $element['#source_types'])) {
            $callbacks[$key] = $info['label'];
          }
        }
      }

      // If we have valid callbacks, provide options.
      $row['callback'] = array(
        '#type' => 'select',
        '#title' => t('Callback'),
        '#title_display' => 'invisible',
        '#options' => $callbacks,
        '#default_value' => isset($source['callback']) ? $source['callback'] : NULL,
      );
      // If the source is valid on it's own, provide an empty option.
      if (empty($callbacks) || !isset($element['#source_types']) || in_array($source_info['type'], $element['#source_types'])) {
        $row['callback']['#empty_option'] = t('No callback');
      }

      $element['table'][$key] = $row;
    }

    // Find suitable sources.
    $suitable_sources = PartyPrimaryFields::getSourcesByType($element['#source_types']);

    // Build our options list.
    $options = array();
    foreach ($suitable_sources as $data_set) {
      $set_options = array();

      foreach ($data_set['sources'] as $source) {
        $key = $source['data_set'] . ':' . $source['property'];
        if (isset($source['value'])) {
          $key .= ':' . $source['value'];
        }

        $set_options[$key] = $source['option label'];
      }

      $options[$data_set['option label']] = $set_options;
    }

    $element['add_source'] = array(
      '#type' => 'container',
      '#tree' => FALSE,
      '#weight' => 99,
      '#attributes' => array('class' => array('container-inline')),
      'add_source_property' => array(
        '#type' => 'select',
        '#title' => t('Source property'),
        '#options' => $options,
        '#empty_option' => ' - ' . t('Select property') . ' - ',
      ),
      'add_source_submit' => array(
        '#type' => 'submit',
        '#value' => t('Add source'),
        '#limit_validation_errors' => array(array('add_source_property'), $element['#parents']),
        '#validate' => array('party_primary_fields_source_table_validate'),
        '#submit' => array('party_primary_fields_source_table_submit'),
      ),
    );
  }

  /**
   * Build the primary field information.
   *
   * @param bool $reset
   *   Whether the cache should be reset.
   */
  protected static function buildFields($reset = FALSE) {
    // If we are resetting, clear our static cache.
    if ($reset) {
      self::$fields = NULL;
    }
    // Otherwise retrieve from the cache.
    else {
      if ($cache = cache_get('party:primary_fields:fields')) {
        self::$fields = $cache->data;
      }
    }

    // If we have no information, build it.
    if (!isset(self::$fields)) {
      // Label and email are provided by custom UIs and stored in a variable which
      // other modules could potentially add to.
      self::$fields = variable_get('party_primary_fields', array());

      // Retrieve from primary field widgets.
      foreach(field_info_instances('party', 'party') as $instance) {
        if ($instance['widget']['type'] == 'party_primary_field') {
          self::$fields[$instance['field_name']] = $instance['widget']['settings']['sources'];
        }
      }

      // Allow other modules to alter it.
      drupal_alter('party_primary_fields_fields', self::$fields);

      // Remove any invalid targets.
      $party_property_info = self::getPropertyInfo('party', 'party');

      foreach (array_keys(self::$fields) as $target) {
        if (!isset($party_property_info[$target])) {
          // Remove and log a watchdog warning.
          watchdog('party', 'The target for the primary field %target does not exist.', array('%target' => $target), WATCHDOG_WARNING);
          unset(self::$fields[$target]);
        }
      }

      // Make sure the sources are valid and sorted by weight.
      $valid_sources = self::getSources();
      foreach (self::$fields as $target => &$sources) {
        // If we don't have any sources, skip it all.
        if (!is_array($sources)) {
          unset(self::$fields[$target]);
          continue;
        }

        // Remove any invalid sources.
        foreach ($sources as $key => &$source) {
          // Make sure our sources have all the keys.
          $source += array(
            'data_set' => NULL,
            'property' => NULL,
            'value' => NULL,
            'callback' => NULL,
            'weight' => 0,
          );

          // Check that the data set exists.
          if (!isset($valid_sources[$source['data_set']])) {
            watchdog('party', 'The source data set %data_set for the primary field %target does not exist.', array(
              '%data_set' => $source['data_set'],
              '%target' => $target,
            ), WATCHDOG_WARNING);
            unset($sources[$key]);
            continue;
          }

          // Check that the property exists.
          $source_key = self::getSourceKey($source, TRUE);
          if (!isset($valid_sources[$source['data_set']]['sources'][$source_key])) {
            watchdog('party', 'The source property %key for the primary field %target does not exist.', array(
              '%key' => $key,
              '%target' => $target,
            ), WATCHDOG_WARNING);
            unset($sources[$key]);
            continue;
          }

          $source_key = self::getSourceKey($source);
          if (!isset($valid_sources[$source['data_set']]['sources'][$source_key])) {
            watchdog('party', 'The source property %key for the primary field %target does not exist. Falling back to %property.', array(
              '%key' => $key,
              '%target' => $target,
              '%property' => $source['property'],
            ), WATCHDOG_WARNING);
            $sources[$key]['value'] = NULL;
            continue;
          }
        }

        // Sort the sources.
        uasort($sources, 'drupal_sort_weight');
      }
    }

    // Store our information to the cache.
    cache_set('party:primary_fields:fields', self::$fields);
  }

  /**
   * Build the primary field source information.
   *
   * @param bool $reset
   *   Whether the cache should be reset.
   */
  protected static function buildSources($reset = FALSE) {
    // If we are resetting, clear our static cache.
    if ($reset) {
      self::$sources = NULL;
    }
    // Otherwise retrieve from the cache.
    else {
      if ($cache = cache_get('party:primary_fields:sources')) {
        self::$sources = $cache->data;
      }
    }

    // If we have no information, build it.
    if (!isset(self::$sources)) {
      $field_map = field_info_field_map();
      self::$sources = array();

      // Go over our data sets finding potential sources.
      foreach (self::getDataSetInfo() as $data_set_name => $data_set) {

        // Build our data set information.
        self::$sources[$data_set_name] = array(
          'set_name' => $data_set['set_name'],
          'entity type' => $data_set['entity type'],
          'entity bundle' => $data_set['entity bundle'],
          'label' => $data_set['label'],
          'option label' => "{$data_set['label']} ({$data_set['set_name']})",
          'sources' => array(),
        );

        // Go over our property info to get the sources.
        $property_info = self::getPropertyInfo($data_set['entity type'], $data_set['entity bundle']);
        foreach ($property_info as $property => $info) {
          // Build our source info.
          $source_info = array(
            'label' => $info['label'],
            'option label' => "{$info['label']} ({$property})",
            'type' => isset($info['type']) ? $info['type'] : NULL,
            'field_type' => NULL,
            'data_set' => $data_set_name,
            'property' => $property,
            'value' => NULL,
            'callbacks' => array(),
            'all types' => array(),
          );

          // If this is a field we store the field type.
          if (!empty($info['field'])) {
            if (isset($field_map[$property])) {
              $source_info['field_type'] = $field_map[$property]['type'];
            }
          }

          // Get the key and add it to the data set sources.
          $key = self::getSourceKey($source_info);
          self::$sources[$data_set_name]['sources'][$key] = $source_info;

          // Process any values within this property.
          if (isset($info['property info'])) {
            foreach ($info['property info'] as $value => $value_info) {
              // Build our source info.
              $source_info = array(
                'label' => "{$info['label']}: {$value_info['label']}",
                'option label' => "{$info['label']}: {$value_info['label']} ({$property}:{$value})",
                'type' => isset($value_info['type']) ? $value_info['type'] : NULL,
                'field_type' => NULL,
                'data_set' => $data_set_name,
                'property' => $property,
                'value' => $value,
                'callbacks' => array(),
                'all types' => array(),
              );

              // Get the key and add it to the data set sources.
              $key = self::getSourceKey($source_info);
              self::$sources[$data_set_name]['sources'][$key] = $source_info;
            }
          }
        }
      }

      // Allow other modules to alter sources.
      drupal_alter('party_primary_fields_sources', self::$sources);

      // Run our cleanup.
      self::cleanSources(self::$sources);
    }

    // Store our information to the cache.
    cache_set('party:primary_fields:sources', self::$sources);
  }

  /**
   * Clean up source information.
   *
   * Calculate the 'all types' key for properties and removing empty data sets.
   */
  protected static function cleanSources(&$sources) {
    foreach ($sources as $data_set_name => &$data_set) {
      // Remove empty data sets.
      if (empty($data_set['sources'])) {
        unset($sources[$data_set_name]);
        continue;
      }

      // Build a list of all types including callbacks.
      foreach ($sources[$data_set_name]['sources'] as &$source) {
        // Make sure our basic type is included.
        $source['all types'][] = $source['type'];

        // Add any callback types.
        foreach ($source['callbacks'] as $callback) {
          $source['all types'][] = $callback['type'];
        }

        // Filter the list and map it.
        $source['all types'] = drupal_map_assoc(array_unique($source['all types']));
      }
    }
  }
}
