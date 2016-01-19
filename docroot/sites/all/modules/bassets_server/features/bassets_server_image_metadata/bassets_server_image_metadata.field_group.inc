<?php
/**
 * @file
 * bassets_server_image_metadata.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function bassets_server_image_metadata_field_group_info() {
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
    'weight' => '1',
    'children' => array(
      0 => 'field_asset_colordepth',
      1 => 'field_computed_height',
      2 => 'field_computed_width',
      3 => 'field_exif_colorspace',
      4 => 'field_exif_photometricinterpreta',
      5 => 'field_file_image_aspect_ratio',
      6 => 'field_file_mimetype',
      7 => 'field_image_colorspace',
      8 => 'field_image_iscolor',
      9 => 'field_image_photometricinterpret',
      10 => 'field_printable_measure',
    ),
    'format_type' => 'fieldset',
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
    'weight' => '4',
    'children' => array(
      0 => 'field_asset_colordepth',
      1 => 'field_asset_datetimeoriginal',
      2 => 'field_asset_tags',
      3 => 'field_computed_height',
      4 => 'field_computed_width',
      5 => 'field_exif_aperturevalue',
      6 => 'field_exif_artist',
      7 => 'field_exif_colorspace',
      8 => 'field_exif_compression',
      9 => 'field_exif_copyright',
      10 => 'field_exif_datetimeoriginal',
      11 => 'field_exif_exposuretime',
      12 => 'field_exif_flash',
      13 => 'field_exif_focallength',
      14 => 'field_exif_imagedescription',
      15 => 'field_exif_isospeedratings',
      16 => 'field_exif_meteringmode',
      17 => 'field_exif_model',
      18 => 'field_exif_orientation',
      19 => 'field_exif_ownername',
      20 => 'field_exif_photometricinterpreta',
      21 => 'field_exif_subjectdistancerange',
      22 => 'field_file_image_aspect_ratio',
      23 => 'field_file_mimetype',
      24 => 'field_gps_gpslatitude',
      25 => 'field_gps_gpslatituderef',
      26 => 'field_gps_gpslongitude',
      27 => 'field_gps_gpslongituderef',
      28 => 'field_image_colorspace',
      29 => 'field_image_metering_mode',
      30 => 'field_image_orientation',
      31 => 'field_image_photometricinterpret',
      32 => 'field_image_print_resolution',
      33 => 'field_image_subjectdistancerange',
      34 => 'field_iptc_by_line',
      35 => 'field_iptc_caption',
      36 => 'field_iptc_copyright_notice',
      37 => 'field_iptc_creation_date',
      38 => 'field_iptc_creation_time',
      39 => 'field_iptc_keywords',
      40 => 'field_printable_measure',
      41 => 'field_unit_of_measurement',
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
  $field_group->parent_name = 'group_assets_htab_group';
  $field_group->data = array(
    'label' => 'Copyrights',
    'weight' => '3',
    'children' => array(
      0 => 'field_asset_copyright',
      1 => 'field_asset_license',
      2 => 'field_asset_license_url',
      3 => 'field_asset_photographer',
      4 => 'field_asset_source',
      5 => 'field_license',
      6 => 'field_other_license',
      7 => 'field_remarks',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
        'id' => '',
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
    'weight' => '3',
    'children' => array(
      0 => 'field_file_image_alt_text',
      1 => 'field_asset_caption',
      2 => 'field_asset_license',
      3 => 'field_asset_license_url',
      4 => 'field_asset_source',
      5 => 'field_image_motive_category',
      6 => 'field_image_motive_type',
      7 => 'field_image_tags',
      8 => 'field_remarks',
    ),
    'format_type' => 'fieldset',
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
  $field_group->parent_name = 'group_assets_htab_group';
  $field_group->data = array(
    'label' => 'Editables',
    'weight' => '2',
    'children' => array(
      0 => 'field_file_image_alt_text',
      1 => 'field_file_image_title_text',
      2 => 'field_asset_caption',
      3 => 'field_asset_description',
      4 => 'field_image_iscolor',
      5 => 'group_asset_tags',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
        'id' => '',
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
    'weight' => '2',
    'children' => array(
      0 => 'field_asset_copyright',
      1 => 'field_asset_datetimeoriginal',
      2 => 'field_asset_description',
      3 => 'field_asset_photographer',
      4 => 'field_exif_aperturevalue',
      5 => 'field_exif_artist',
      6 => 'field_exif_compression',
      7 => 'field_exif_copyright',
      8 => 'field_exif_datetimeoriginal',
      9 => 'field_exif_exposuretime',
      10 => 'field_exif_flash',
      11 => 'field_exif_focallength',
      12 => 'field_exif_imagedescription',
      13 => 'field_exif_isospeedratings',
      14 => 'field_exif_model',
      15 => 'field_exif_ownername',
      16 => 'field_exif_subjectdistancerange',
      17 => 'field_gps_gpslatitude',
      18 => 'field_gps_gpslatituderef',
      19 => 'field_gps_gpslongitude',
      20 => 'field_gps_gpslongituderef',
      21 => 'field_image_metering_mode',
      22 => 'field_image_orientation',
      23 => 'field_image_subjectdistancerange',
      24 => 'field_iptc_by_line',
      25 => 'field_iptc_caption',
      26 => 'field_iptc_copyright_notice',
      27 => 'field_iptc_creation_date',
      28 => 'field_iptc_creation_time',
      29 => 'field_iptc_keywords',
    ),
    'format_type' => 'fieldset',
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
    'weight' => '8',
    'children' => array(
      0 => 'field_asset_datetimeoriginal',
      1 => 'field_exif_aperturevalue',
      2 => 'field_exif_datetimeoriginal',
      3 => 'field_exif_exposuretime',
      4 => 'field_exif_flash',
      5 => 'field_exif_focallength',
      6 => 'field_exif_isospeedratings',
      7 => 'field_exif_meteringmode',
      8 => 'field_exif_model',
      9 => 'field_exif_ownername',
      10 => 'field_exif_subjectdistancerange',
      11 => 'field_image_metering_mode',
      12 => 'field_image_subjectdistancerange',
      13 => 'field_iptc_creation_date',
      14 => 'field_iptc_creation_time',
      15 => 'field_asset_tags',
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
    'weight' => '6',
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
    'weight' => '4',
    'children' => array(
      0 => 'field_asset_lock',
    ),
    'format_type' => 'tab',
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
    'weight' => '5',
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
  $field_group->parent_name = 'group_asset_editables';
  $field_group->data = array(
    'label' => 'Tagging',
    'weight' => '17',
    'children' => array(
      0 => 'field_image_motive_category',
      1 => 'field_image_motive_type',
      2 => 'field_image_tags',
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

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_assets_htab_group|file|image|form';
  $field_group->group_name = 'group_assets_htab_group';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Editables/Copyright',
    'weight' => '75',
    'children' => array(
      0 => 'group_asset_copyright',
      1 => 'group_asset_editables',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-assets-htab-group field-group-htabs',
      ),
    ),
  );
  $export['group_assets_htab_group|file|image|form'] = $field_group;

  return $export;
}
