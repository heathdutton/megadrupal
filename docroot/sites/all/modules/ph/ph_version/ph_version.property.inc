<?php
/**
 * @file
 * Property implementation for Entity API.
 */

/**
 * Callback function for field property info.
 */
function ph_version_property_info_callback(&$info, $entity_type, $field, $instance, $field_type) {
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$field['field_name']];
  $property['getter callback'] = 'entity_metadata_field_verbatim_get';
  $property['setter callback'] = 'entity_metadata_field_verbatim_set';
  $property['auto creation'] = 'ph_version_defaults';
  $property['property info'] = ph_version_data_property_info($instance['settings']['version_type']);
}

/**
 * Returns version property info.
 */
function ph_version_data_property_info($type = 'contrib') {
  $info = ph_version_data_version_properties($type);
  $info['standardized'] = array(
    'label' => t('Standardized version'),
    'type' => 'ph_version',
    'description' => t('The standardized form of the version property.'),
    'property info' => ph_version_data_version_properties($type),
    'getter callback' => 'ph_version_standardize',
  );
  return $info;
}

/**
 * Returns direct version properties, without any nested versions.
 */
function ph_version_data_version_properties($type = 'contrib') {
  // Provide formatted version strings as properties.
  $info['version_full'] = array(
    'label' => t('Full version string'),
    'type' => 'text',
    'description' => t('Formatted version string containing all available parts, including details about a development snapshot where available (e.g. %example).', array(
      '%example' => $type == 'core' ? '7.0+5-dev' : '7.x-1.0+5-dev',
    )),
    'version format' => 'full',
    'version type' => $type,
    'getter callback' => 'ph_version_property_version_get',
    'setter callback' => 'ph_version_property_version_set',
    'validation callback' => 'ph_version_property_version_validate',
  );
  $info['version_standard'] = array(
    'label' => t('Standard version string'),
    'type' => 'text',
    'description' => t('Formatted standard version string, without development snapshot details, e.g. %example.', array(
      '%example' => $type == 'core' ? '7.x-dev' : '7.x-1.x-dev',
    )),
    'version format' => 'standard',
    'version type' => $type,
    'getter callback' => 'ph_version_property_version_get',
    'setter callback' => 'ph_version_property_version_set',
    'validation callback' => 'ph_version_property_version_validate',
  );
  $info['version_tag'] = array(
    'label' => t('Version tag'),
    'type' => 'text',
    'description' => t('Version control system tag name represented by the version string, without development snapshot details, e.g. %example.', array(
      '%example' => $type == 'core' ? '7.x' : '7.x-1.x',
    )),
    'version format' => 'tag',
    'version type' => $type,
    'getter callback' => 'ph_version_property_version_get',
    'setter callback' => 'ph_version_property_version_set',
    'validation callback' => 'ph_version_property_version_validate',
  );
  $info['core_api'] = array(
    'label' => t('Core API version'),
    'type' => 'text',
    'description' => t('Core API version identifier, e.g. %example.', array(
      '%example' => '7.x',
    )),
    'version format' => 'core_api',
    'getter callback' => 'ph_version_property_version_get',
  );

  // Add version properties.
  $info += ph_version_data_field_property_info($type);

  // Add derived properties.
  $info['suffix'] = array(
    'label' => t('Suffix'),
    'type' => 'text',
    'description' => t('The version suffix. This is the extra release identifier (where exists) for a non-development version or "-dev" for a development version.'),
    'getter callback' => 'ph_version_property_suffix_get',
  );
  $info['is_core'] = array(
    'label' => t('Version is for Drupal core'),
    'type' => 'boolean',
    'description' => t('Whether this version contains information about a Drupal core release.'),
    'getter callback' => 'ph_version_property_version_check',
    'version check' => 'core',
    'version type' => $type,
  );
  $info['is_dev'] = array(
    'label' => t('Version is development snapshot'),
    'type' => 'boolean',
    'description' => t('Whether this version is a development snapshot.'),
    'getter callback' => 'ph_version_property_version_check',
    'version check' => 'dev',
    'version type' => $type,
  );

  return $info;
}

/**
 * Returns basic field property info.
 */
function ph_version_data_field_property_info($type = 'contrib') {
  $labels = ph_version_component_options();
  $info = array();

  // Add basic field properties.
  $properties['core'] = array(
    'type' => 'integer',
    'description' => t('The core base version, without the patch level.'),
    'validation callback' => 'entity_property_validate_integer_positive',
  );
  $properties['core_minor'] = array(
    'type' => 'integer',
    'description' => t('The minor core version (non-patch), for Drupal 4.7.x and earlier.'),
    'validation callback' => 'entity_property_validate_integer_non_negative',
  );
  if (!$type || $type == 'contrib') {
    $properties['major'] = array(
      'type' => 'integer',
      'description' => t('The major version number of a contribution.'),
      'validation callback' => 'entity_property_validate_integer_positive',
    );
  }
  $properties['patch'] = array(
    'type' => 'integer',
    'description' => t('The patch level of the indicated major version or core version.'),
    'validation callback' => 'entity_property_validate_integer_non_negative',
  );
  $properties['extra'] = array(
    'type' => 'text',
    'description' => t('The extra release identifier indicating release status, e.g. beta1, rc2.'),
  );
  $properties['increment'] = array(
    'type' => 'integer',
    'description' => t('The commit increment since the indicated patch/extra versioned release.'),
    'validation callback' => 'entity_property_validate_integer_non_negative',
  );
  foreach ($properties as $name => $property) {
    $info[$name] = $property + array(
      'label' => isset($labels[$name]) ? $labels[$name] : $name,
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    );
  }

  return $info;
}

/**
 * Converts entity property data to a version info structure.
 */
function ph_version_property_to_version($data) {
  // Individually obtain version parts to support wrapped structures.
  $version = ph_version_defaults();
  foreach (array_keys($version) as $name) {
    // Collect value.
    if (!is_object($data[$name])) {
      $version[$name] = $data[$name];
    }
    elseif ($data[$name] instanceof EntityMetadataWrapper) {
      $version[$name] = $data[$name]->value();
    }
  }
  return $version;
}

/**
 * Formats a full version string.
 */
function ph_version_property_version_get($data, array $options, $name, $type, $info) {
  // Prepare version property info.
  $info += array(
    'version format' => '',
    'version type' => '',
  );
  $version_format = $info['version format'] ? $info['version format'] : 'full';
  $version_type = $info['version type'];

  // Obtain version array.
  $version = ph_version_property_to_version($data);

  // Format version string.
  return ph_version_format($version, $version_format, $version_type);
}

/**
 * Sets a version string.
 */
function ph_version_property_version_set(&$data, $name, $value, $langcode, $type) {
  $data = ph_version_extract_info($value);
}

/**
 * Derives the version suffix.
 */
function ph_version_property_suffix_get($data, array $options, $name, $type, $info) {
  // Obtain version array.
  $version = ph_version_property_to_version($data);

  // Return suffix.
  return ph_version_is_dev($version) ? 'dev' : $version['extra'];
}

/**
 * Validates a full version string according to its format.
 */
function ph_version_property_version_validate($value, $info) {
  if (!empty($info['version format'])) {
    return ph_version_valid_format($value, $info['version format']);
  }
  return FALSE;
}

/**
 * Checks a meta-property of a version.
 */
function ph_version_property_version_check($data, array $options, $name, $type, $info) {
  // Prepare version property info.
  $info += array(
    'version check' => '',
    'version type' => '',
  );

  if ($info['version check']) {
    $version = ph_version_property_to_version($data);

    switch ($info['version check']) {
      case 'core':
        return $info['version type'] == 'core' || ph_version_is_core($version);
      case 'dev':
        return ph_version_is_dev($version);
    }
  }
}
