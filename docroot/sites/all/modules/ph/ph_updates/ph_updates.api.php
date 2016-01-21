<?php
/**
 * @file
 * Hook documentation for the Updates Feed module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define data structures used for constructing update feeds.
 *
 * Each data type declared will produce for use in the Entity API an update
 * information type and a feed tree type. The information type definition shall
 * contain the properties for containing data and mappings for extracting data
 * into a tree. The feed tree data type contains format metadata for use with
 * specific formatters.
 *
 * This hook should be implemented in MODULENAME.ph.inc.
 *
 * @return array
 *   An array of data types. Each data type contains the following information:
 *   - label: Translated user-friendly type label.
 *   - property info: Properties of the info type made available for Entity API.
 *     The 'setter callback' for each property automatically defaults to
 *     'entity_property_verbatim_set'. Specify a different callback for
 *     customized setter behavior. Each property can additionally define:
 *     - type: Extra types available for referencing feed data types. Specify
 *       ph_updates_info<TYPE> to reference the generated info type (containing
 *       properties) named TYPE as defined in hook_ph_updates_info(). Specify
 *       ph_updates_tree<TYPE> to reference the generated data tree type
 *       (containing extracted tree data and format info) named TYPE.
 *     - tree mapping: Feed tree mapping. If the mapping is missing, the
 *       property name is used as the tree key to map to. A mapping can be:
 *       - a string specifying the target feed tree key, directly mapping the
 *         property value to the key.
 *       - an array of targets, each key being target feed tree key and each
 *         value being the selector based on the property, e.g. 'value' for the
 *         textual value of a 'text_formatted' property. The selector must
 *         reference only literal or strictly array values, i.e. not containing
 *         any objects. If 'tree value callback' is given, this is ignored.
 *     - tree list mapping: Feed tree mapping for individual items of list
 *       properties. Use the structure of 'tree mapping' for an item. If this is
 *       not defined, the atomic value becomes the list item value directly. If
 *       'tree value callback' is given, this is ignored. Mappings specified in
 *       array form are also ignored if 'tree list value callback' is provided.
 *     - tree value callback: Callback for customizing mapping value. See
 *       ph_updates_value_boolean_published() for an example callback.
 *     - tree list value callback: Callback for customizing list item mapping
 *       value for a list property.
 *   - format info: Metadata used by feed formatter to customize formatting
 *     behavior. Metadata for a format should be contained in an array keyed by
 *     the format name, e.g. 'xml'. Metadata applicable to all formats should be
 *     keyed by an empty string, i.e. ''. For a format, metadata should be keyed
 *     by tree path, starting with a single slash ("/") indicating the base.
 *     This info should be added in hook_ph_updates_info_alter(). Supported
 *     general format info attributes include:
 *     - name: Overall name for this data structure. If this is specified, it
 *       will override the tree key referencing this structure. This is required
 *       for a data structure for the root element, e.g. 'project'.
 *     - list item: Applicable to list properties, a string indicating the key
 *       to use for an individual list item (e.g. for XML). If this is not
 *       defined for a list, the list item name defaults to 'item'.
 *
 * @see ph_updates_xml_hook_ph_updates_info()
 * @see hook_ph_updates_info_alter()
 * @see ph_updates_value_boolean_published()
 */
function hook_ph_updates_info() {
  return array(
    'project' => array(
      'label' => t('Project'),
      'property info' => array(
        'title' => array(
          'type' => 'text',
          'label' => t('Title'),
        ),
        'api_version' => array(
          'type' => 'text',
          'label' => t('API Version'),
        ),
        'creator' => array(
          'type' => 'user',
          'label' => t('Creator'),
          'tree mapping' => array(
            'creator' => 'creator:name',
          ),
        ),
        'project_status' => array(
          'type' => 'boolean',
          'label' => t('Status'),
          'tree value callback' => 'ph_updates_value_boolean_published',
        ),
        'terms' => array(
          'type' => 'list<taxonomy_term>',
          'label' => t('Terms'),
          'tree list mapping' => array(
            'name' => 'vocabulary:name',
            'value' => 'name',
          ),
        ),
        'releases' => array(
          'type' => 'list<ph_updates_tree<release>>',
          'label' => t('Releases'),
        ),
      ),
      'format info' => array(
        '' => array(
          '/' => array(
            'name' => 'project',
          ),
          '/terms' => array(
            'list item' => 'term',
          ),
        ),
      ),
    ),
  );
}

/**
 * Documentation for supported XML format info.
 *
 * This function is a placeholder for documenting format info supported by the
 * XML format.
 *
 * Supported format info attributes include:
 * - xmlns: A string namespace URI or an array of namespace prefix declarations.
 *   Array values are namespace URIs keyed by namespace prefixes. To both set
 *   the default namespace and declare prefixes, use '' as the key for the
 *   default namespace URI.
 * - prefix: Namespace prefix for the element, defined in array 'xmlns'.
 *
 * Adding this info in hook_ph_updates_info_alter() is recommended.
 *
 * @see hook_ph_updates_info()
 * @see hook_ph_updates_info_alter()
 */
function ph_updates_xml_hook_ph_updates_info() {
  return array(
    'project' => array(
      // 'label' ...
      // 'property info' ...
      'format info' => array(
        'xml' => array(
          '/' => array(
            'xmlns' => array(
              'dc' => 'http://purl.org/dc/elements/1.1/',
            ),
          ),
          '/creator' => array(
            'prefix' => 'dc',
          ),
        ),
      ),
    ),
  );
}

/**
 * Alter data structure definitions.
 *
 * This hook is useful for feed formatters to add metadata for formatting.
 *
 * @see hook_ph_updates_info()
 */
function hook_ph_updates_info_alter(&$info) {
  // Alter $info.
}

/**
 * Provide feed formats for displaying updates feed data.
 *
 * This hook should be implemented in MODULENAME.ph.inc.
 *
 * @return array
 *   An array of feed format definitions, keyed by machine name. Each
 *   format is defined as follows:
 *   - label: Translated user-friendly format name.
 *   - class: Formatter class. This class must extend PHUpdatesFeedFormatter.
 *   - mimetype: (optional) Content type of the formatted feed. If unspecified,
 *     this will default to 'text/plain'.
 */
function hook_ph_updates_format_info() {
  return array(
    'xml' => array(
      'label' => t('Custom'),
      'class' => 'MyCustomFormat',
      'mimetype' => 'text/plain',
    ),
  );
}

/**
 * Builds project update feed info.
 *
 * The release history for a project can be built using this hook. The info is
 * treated as incomplete if:
 * - 'short_name' is not set, which implies the project is not found.
 * - 'api_version' is not set, which implies there are no matching releases.
 *
 * This hook should be implemented in MODULENAME.ph.inc.
 *
 * @param PHUpdatesInfoWrapper $info
 *   Metadata wrapper for the info tree. For a deep property with handler class
 *   (as available in Rules), use PHUpdatesInfoWrapper::select() to wrap the
 *   property with the indicated handler.
 * @param string $name
 *   Name of the request project to build.
 * @param string $core_api
 *   Core API version, e.g. '7.x'.
 */
function hook_ph_updates_project_info_build(PHUpdatesInfoWrapper $info, $name, $core_api) {
  $info->short_name = $name;
  $info->api_version = $core_api;
}

/**
 * Alters project update feed info after it is built.
 *
 * This hook should be implemented in MODULENAME.ph.inc.
 *
 * @see hook_ph_updates_project_info_build()
 */
function hook_ph_updates_project_info_alter(PHUpdatesInfoWrapper $info, $name, $core_api) {
  // Alter $info.
}

/**
 * Checks project update feed info.
 *
 * Modules implementing this hook may throw a subclass of PHUpdatesInfoError to
 * indicate the error in the given info. The error will be formatted separately
 * by the active format.
 *
 * This hook should be implemented in MODULENAME.ph.inc.
 */
function hook_ph_updates_project_info_check(PHUpdatesInfoWrapper $info, $name, $core_api) {
  if (!$info->short_name->value()) {
    throw new PHUpdatesInfoIncompleteError($info, 'short_name');
  }
}

/**
 * @}
 */
