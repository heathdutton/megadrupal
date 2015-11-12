<?php
/**
 * @file
 * Class for migrating Image module D6 files for image nodes into Drupal 7.
 */

/**
 * Migration for image data on image nodes, migrating to an imagefield on nodes.
 *
 * This runs after a node migration and a file migration, which import the nodes
 * and the image files respectively. This migration merely sets the data in the
 * image field on the nodes.
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *  - image_node_type: Machine name of the destination node type.
 *  - file_migration - Machine name of the file migration.
 *  - image_node_migration - Machine name of the node migration for the image
 *    node type.
 */
class ImageNodeDrupalMigration extends DrupalMigration {

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    
    $this->imageNodeMigration = $arguments['image_node_migration'];
    $this->dependencies[] = $this->imageNodeMigration;

    $this->fileMigration = $arguments['file_migration'];
    $this->dependencies[] = $this->fileMigration;

    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields,
      NULL, $this->sourceOptions);

    $this->destination = new MigrateDestinationNode($arguments['image_node_type']);

    // Update existing nodes instead of creating new ones.
    $this->systemOfRecord = Migration::DESTINATION;

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source node ID',
                       'alias' => 'n',
                      ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Node id.
    $this->addFieldMapping('nid', 'nid')
      ->sourceMigration($this->imageNodeMigration);

    // Image file id.
    $image_field = $arguments['image_field'];
    $this->addFieldMapping($image_field . ':file_class')
      ->defaultValue('MigrateFileFid');
    $this->addFieldMapping($image_field, 'fid')
      ->sourceMigration($this->fileMigration);
  }

  /**
   * Query for the image data.
   *
   * @return QueryConditionInterface
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('image', 'i')
      ->fields('i')
      ->condition('image_size', '_original');
    return $query;
  }

}
