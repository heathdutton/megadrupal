<?php

/**
 * @file
 * Import photos and other data from Gallery2.
 *
 * Extend g2migrate/galler2.inc by adding migrations that use the Media
 * (7.x-1.x) module.
 */

/**
 * Implements hook_migrate_api.
 *
 * You must implement hook_migrate_api(), setting the API level to 2, for
 * your migration classes to be recognized by the Migrate module.
 */
function g2migrate_media1_migrate_api() {
  $api = array();
  $api['api'] = 2;
  $api['groups']['g2migrate']['title'] = t('Gallery2');
  $group_name = array('group_name' => 'g2migrate');
  $api['migrations'] = array(
    'Gallery2Media1Image' => array('class_name' => 'Gallery2Media1ImageMigration') + $group_name,
    'Gallery2Media1Node' => array('class_name' => 'Gallery2Media1NodeMigration') + $group_name,
  );
  return $api;
}

/**
 * The moment you have been waiting for: import those photos.
 *
 * This class handles photos attached using the Media module.
 *
 * Tested with Media module 7.x-1.2.
 */
class Gallery2Media1ImageMigration extends Gallery2BaseMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Create Media fields from Gallery2 photos.');

    // In the simplest case, just pass the media type.
    $this->destination = new MigrateDestinationMedia('image');

    // The source images are in a local directory - specify the parent.
    $this->addFieldMapping('source_dir')
        ->defaultValue(variable_get('g2migrate_g2data') . '/albums');
    // The 'value' of the media destination is mapped to the source field
    // representing the media itself - in this case, a filename relative to
    // source_dir.
    $this->addFieldMapping('value', 'path');
    // // Fields on the entity can be mapped in the usual way.
    // $this->addFieldMapping('field_image_description', 'description')
    //      ->xpath('description');
    $this->addFieldMapping('media_title', 'g_title');
    $this->addFieldMapping('media_description', 'g_summary');
    $this->addFieldMapping('field_tags', 'g_keywords');
    $this->addFieldMapping('field_tags:create_term')
      ->defaultValue(TRUE);

    // TODO: It might be nice to add these fields to the Media FileField entity.
    // For now, ignore them and add them to node in a dependent migration.
    $this->addUnmigratedSources(array('g_parentid'));

    $this->addUnmigratedDestinations(
      array(
        'timestamp',
        'media_title:format',
        'media_title:language',
        'media_description:format',
        'media_description:language',
        'field_tags:source_type',
        'field_license',
        'destination_dir',
        'destination_file',
        'file_replace',
        'preserve_files',
      )
    );

  }
}

/**
 * The moment you have been waiting for: import those photos.
 *
 * This class creates nodes from photos imported from Gallery2. The only
 * difference from Gallery2NodeMigration is that the photos are stored as Media
 * fields instead of Image fields.
 *
 * TODO: Analyze the difference between this class and Gallery2NodeMigration and
 * remove duplicated code.
 *
 * Tested with Media module 7.x-1.2.
 */
class Gallery2Media1NodeMigration extends Gallery2BaseMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Create nodes and attach photos as Media FileFields.');

    $node_type = variable_get('g2migrate_node_type', 'g2migrate_photo');
    $this->destination = new MigrateDestinationNode($node_type);

    $dnm = array();
    $unmigrated = array(
      'changed', 'translate',
      'revision', 'language', 'revision_uid', 'log', 'tnid',
      'body:language',
      'comment', 'is_new',
    );
    foreach ($sources = _g2migrate_sources() as $key => $data) {
      $field = variable_get("g2migrate_field_$key", $data['default']);
      if ($field == 'none') {
        $dnm[] = $key;
        continue;
      }
      // $sources[$key]['field'] = $field;
      $field_mapping = $this->addFieldMapping($field, $key);
      foreach ($data as $key => $value) {
        switch ($key) {
        case 'description':
        case 'sourceMigration':
        case 'defaultValue':
        case 'separator':
          $field_mapping = $field_mapping->{$key}($value);
          break;
        case 'source_dir':
          $this->addFieldMapping("$field:source_dir")
            ->defaultValue(variable_get('g2migrate_g2data') . "/$value");
          break;
        case 'unmigrated':
          foreach ($value as $subfield) {
            $unmigrated[] = "$field:$subfield";
          }
          break;
        case 'subfields':
          foreach ($value as $subfield => $subvalue) {
            $this->addFieldMapping("$field:$subfield")
              ->defaultValue($subvalue);
          }
          break;
        }
      }
    }

    $this->addUnmigratedDestinations($unmigrated);
    $this->addUnmigratedSources($dnm);

  }
}
