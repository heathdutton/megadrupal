<?php
/**
 * @file
 * Class for migrating Image attach into Drupal 7.
 */

/**
 * Migration for image attach data into entityreference fields.
 *
 * This needs to be run separately for each node type which has attached images
 * (this is because the mapping for the attaching node has to come from the node
 * migration, which is per node type).
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *  - image_node_migration - Machine name of the node migration for the image
 *    node type.
 *  - attaching_node_migration - Machine name of the node migration for the
 *    attaching node type.
 *  - attaching_node_type - Machine name of the node type that attaches the
 *    images.
 *  - attaching_entityreference_field - Machine name of the entityreference
 *    field on the attaching node type which will point to image nodes.
 */
class ImageAttachImageNodeDrupalMigration extends DrupalMigration {

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->imageNodeMigration = $arguments['image_node_migration'];
    $this->dependencies[] = $this->imageNodeMigration;

    $this->attachingNodeMigration = $arguments['attaching_node_migration'];
    $this->dependencies[] = $this->attachingNodeMigration;

    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields,
      NULL, $this->sourceOptions);

    $this->destination = new MigrateDestinationNode($arguments['attaching_node_type']);

    // Update existing nodes instead of creating new ones.
    $this->systemOfRecord = Migration::DESTINATION;

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Source image attach attaching node ID',
          'alias' => 'n',
        ),
        'iid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Source image attach image node ID',
          'alias' => 'i',
        ),
      ),
      array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Source image attach attaching node ID',
          'alias' => 'n',
        ),
        'iid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Source image attach image node ID',
          'alias' => 'i',
        ),
      )
    );

    // Attaching node id.
    $this->addFieldMapping('nid', 'nid')
      ->sourceMigration($this->attachingNodeMigration);

    // Image node id.
    $attaching_entityreference_field = $arguments['attaching_entityreference_field'];
    $this->addFieldMapping($attaching_entityreference_field, 'iid')
      ->sourceMigration($this->imageNodeMigration);
  }

  /**
   * Query for the image data.
   *
   * @return QueryConditionInterface
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('image_attach', 'ia')
      ->fields('ia')
      //->addExpression('GROUP_CONCAT(DISTINCT ia.iid)', 'image_nids')
      ->groupBy('ia.nid');
    return $query;
  }

}
