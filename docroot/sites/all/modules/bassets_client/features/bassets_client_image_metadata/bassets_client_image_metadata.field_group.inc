<?php
/**
 * @file
 * bassets_client_image_metadata.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function bassets_client_image_metadata_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_computed_data|file|image|default';
  $field_group->group_name = 'group_asset_computed_data';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Computed Data',
    'weight' => '4',
    'children' => array(
      0 => 'field_asset_colordepth',
      1 => 'field_computed_height',
      2 => 'field_computed_width',
      3 => 'field_file_image_aspect_ratio',
      4 => 'field_file_mimetype',
      5 => 'field_image_colorspace',
      6 => 'field_image_iscolor',
      7 => 'field_printable_measure',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_asset_computed_data|file|image|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_computed_data|file|image|form';
  $field_group->group_name = 'group_asset_computed_data';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Computed Data',
    'weight' => '7',
    'children' => array(
      0 => 'field_asset_colordepth',
      1 => 'field_computed_height',
      2 => 'field_computed_width',
      3 => 'field_exif_compression',
      4 => 'field_file_image_aspect_ratio',
      5 => 'field_file_mimetype',
      6 => 'field_image_colorspace',
      7 => 'field_image_iscolor',
      8 => 'field_image_orientation',
      9 => 'field_image_print_resolution',
      10 => 'field_printable_measure',
      11 => 'field_unit_of_measurement',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_asset_computed_data|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_copyright|file|image|form';
  $field_group->group_name = 'group_asset_copyright';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Copyrights',
    'weight' => '6',
    'children' => array(
      0 => 'field_asset_copyright',
      1 => 'field_asset_license',
      2 => 'field_asset_license_url',
      3 => 'field_asset_photographer',
      4 => 'field_asset_source',
      5 => 'field_remarks',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_asset_copyright|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_editables|file|image|default';
  $field_group->group_name = 'group_asset_editables';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Editables',
    'weight' => '6',
    'children' => array(
      0 => 'field_asset_caption',
      1 => 'field_asset_source',
      2 => 'field_image_motive_category',
      3 => 'field_image_motive_type',
      4 => 'field_remarks',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_asset_editables|file|image|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_editables|file|image|form';
  $field_group->group_name = 'group_asset_editables';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Editables',
    'weight' => '4',
    'children' => array(
      0 => 'field_file_image_title_text',
      1 => 'field_asset_caption',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_asset_editables|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_exifiptc|file|image|default';
  $field_group->group_name = 'group_asset_exifiptc';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Exif/IPTC',
    'weight' => '5',
    'children' => array(
      0 => 'field_asset_copyright',
      1 => 'field_asset_datetimeoriginal',
      2 => 'field_asset_description',
      3 => 'field_asset_photographer',
      4 => 'field_exif_aperturevalue',
      5 => 'field_exif_compression',
      6 => 'field_exif_exposuretime',
      7 => 'field_exif_flash',
      8 => 'field_exif_focallength',
      9 => 'field_exif_isospeedratings',
      10 => 'field_exif_model',
      11 => 'field_exif_ownername',
      12 => 'field_gps_gpslatitude',
      13 => 'field_gps_gpslatituderef',
      14 => 'field_gps_gpslongitude',
      15 => 'field_gps_gpslongituderef',
      16 => 'field_image_metering_mode',
      17 => 'field_image_orientation',
      18 => 'field_image_subjectdistancerange',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_asset_exifiptc|file|image|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_exposure|file|image|form';
  $field_group->group_name = 'group_asset_exposure';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Exposure Data',
    'weight' => '9',
    'children' => array(
      0 => 'field_asset_datetimeoriginal',
      1 => 'field_exif_aperturevalue',
      2 => 'field_exif_exposuretime',
      3 => 'field_exif_flash',
      4 => 'field_exif_focallength',
      5 => 'field_exif_isospeedratings',
      6 => 'field_exif_model',
      7 => 'field_exif_ownername',
      8 => 'field_image_metering_mode',
      9 => 'field_image_subjectdistancerange',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_asset_exposure|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_gps|file|image|form';
  $field_group->group_name = 'group_asset_gps';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'GPS Data',
    'weight' => '8',
    'children' => array(
      0 => 'field_gps_gpslatitude',
      1 => 'field_gps_gpslatituderef',
      2 => 'field_gps_gpslongitude',
      3 => 'field_gps_gpslongituderef',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_asset_gps|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_supervision|file|image|default';
  $field_group->group_name = 'group_asset_supervision';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Supervision',
    'weight' => '7',
    'children' => array(
      0 => 'field_asset_lock',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_asset_supervision|file|image|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_supervision|file|image|form';
  $field_group->group_name = 'group_asset_supervision';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Supervision',
    'weight' => '10',
    'children' => array(
      0 => 'field_asset_lock',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_asset_supervision|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_asset_tags|file|image|form';
  $field_group->group_name = 'group_asset_tags';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tagging',
    'weight' => '5',
    'children' => array(
      0 => 'field_asset_tags',
      1 => 'field_image_motive_category',
      2 => 'field_image_motive_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_asset_tags|file|image|form'] = $field_group;

  return $export;
}
