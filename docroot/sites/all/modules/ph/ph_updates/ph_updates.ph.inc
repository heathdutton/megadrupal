<?php
/**
 * @file
 * Updates feed type definition.
 */

/**
 * Implements hook_ph_updates_info().
 */
function ph_updates_ph_updates_info() {
  return array(
    'project' => array(
      'label' => t('project'),
      'property info' => array(
        'title' => array(
          'type' => 'text',
          'label' => t('Title'),
        ),
        'short_name' => array(
          'type' => 'text',
          'label' => t('Unique short name'),
        ),
        'creator' => array(
          'type' => 'user',
          'label' => t('Creator'),
          'tree mapping' => array(
            'creator' => 'name',
          ),
        ),
        'api_version' => array(
          'type' => 'text',
          'label' => t('API version'),
        ),
        'recommended_major' => array(
          'type' => 'integer',
          'label' => t('Recommended major version'),
          'validation callback' => 'entity_property_validate_integer_positive',
        ),
        'supported_majors' => array(
          'type' => 'list<integer>',
          'label' => t('Supported major versions'),
          'validation callback' => 'entity_property_validate_integer_positive',
          'tree value callback' => 'ph_updates_value_comma_delimited_list',
        ),
        'default_major' => array(
          'type' => 'integer',
          'label' => t('Default major version'),
          'validation callback' => 'entity_property_validate_integer_positive',
        ),
        'project_status' => array(
          'type' => 'boolean',
          'label' => t('Status'),
          'tree value callback' => 'ph_updates_value_boolean_published',
        ),
        'link' => array(
          'type' => 'uri',
          'label' => t('Link'),
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
          '/releases' => array(
            'list item' => 'release',
          ),
        ),
      ),
    ),
    'release' => array(
      'label' => t('release'),
      'property info' => array(
        'name' => array(
          'type' => 'text',
          'label' => t('Name'),
        ),
        'version' => array(
          'type' => 'ph_version',
          'label' => t('Version'),
          'tree mapping' => array(
            'version' => 'version_standard',
            'tag' => 'version_tag',
            'version_major' => 'standardized:major',
            'version_patch' => 'standardized:patch',
            'version_extra' => 'standardized:suffix',
          ),
        ),
        'status' => array(
          'type' => 'boolean',
          'label' => t('Status'),
          'tree value callback' => 'ph_updates_value_boolean_published',
        ),
        'release_link' => array(
          'type' => 'uri',
          'label' => t('Release URL'),
        ),
        'file' => array(
          'type' => 'file',
          'label' => t('Default release archive'),
          'tree mapping' => array(
            'download_link' => 'url',
            'date' => 'timestamp',
            'mdhash' => 'md5hash',
            'filesize' => 'size',
          ),
          'property info alter' => 'ph_updates_release_default_file_property_info_alter',
        ),
        'files' => array(
          'type' => 'list<file>',
          'label' => t('Files'),
          'tree list mapping' => array(
            'url' => 'url',
            'archive_type' => 'archive_extension',
            'md5' => 'md5hash',
            'size' => 'size',
            'filedate' => 'timestamp',
          ),
          'property info alter' => 'ph_updates_release_file_property_info_alter',
        ),
        'terms' => array(
          'type' => 'list<taxonomy_term>',
          'label' => t('Terms'),
          'tree list mapping' => array(
            'name' => 'vocabulary:name',
            'value' => 'name',
          ),
        ),
      ),
      'format info' => array(
        '' => array(
          '/' => array(
            'name' => 'releases',
          ),
          '/files' => array(
            'list item' => 'file',
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
 * Implements hook_ph_updates_project_info_check().
 */
function ph_updates_ph_updates_project_info_check(PHUpdatesInfoWrapper $info, $name, $core_api) {
  if (!$info->short_name->value()) {
    throw new PHUpdatesInfoIncompleteError($info, 'short_name', "No release history was found for the requested project ($name).");
  }
  elseif (!$info->api_version->value()) {
    throw new PHUpdatesInfoIncompleteError($info, 'api_version', "No release history available for $name $core_api.");
  }
}

/**
 * Implements hook_ph_updates_info_alter().
 */
function ph_updates_ph_updates_info_alter(&$info) {
  // Perform alterations for XML format.
  if (variable_get('ph_updates_feed_format') == 'xml') {
    // Define creator namespace.
    $info['project']['format info']['xml'] = array(
      '/' => array(
        'xmlns' => array(
          'dc' => 'http://purl.org/dc/elements/1.1/',
        ),
      ),
      '/creator' => array(
        'prefix' => 'dc',
      ),
    );
  }
}

/**
 * Implements hook_ph_updates_format_info().
 */
function ph_updates_ph_updates_format_info() {
  return array(
    'xml' => array(
      'label' => t('XML'),
      'class' => 'PHUpdatesXMLFormat',
      'mimetype' => 'text/xml',
    ),
  );
}
