<?php

/**
 * Common mappings for the Drupal 6 node migrations.
 */
class OaNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    // Add any other data we're pulling into the source row, before the parent
    // constructor.
    $this->sourceFields['summary'] = 'Summary string from D6';

    parent::__construct($arguments);

    // Don't map 'comment' settings - force them to be the default of the
    // destination type. This is important because Open Atrium 2 doesn't use
    // comments for most types.
    $this->addFieldMapping('comment', NULL, FALSE)
      ->defaultValue(variable_get('comment_default_mode_' . $arguments['destination_type'], 0));

    // Don't map from 'status' to 'status' - we want unpublished items to be
    // published in OA2, but marked with the 'trash' flag.
    $this->addFieldMapping('status', NULL, FALSE)->defaultValue(1);

    $fields = $this->destination->fields();

    // Map the original nid to a common field.
    if (module_exists('oa_migrate_legacy') && !empty($fields['field_oa_migrate_legacy_id'])) {
      $this->addFieldMapping('field_oa_migrate_legacy_id', 'nid')
           ->description('We have a common field to save the legacy id (usually just nid).');
    }

    // Map the keywords if the Taxonomy/Field is available.
    if (module_exists('oa_migrate_legacy_keywords') && !empty($fields['field_oa_migrate_legacy_keywords'])) {
      $this->addFieldMapping('field_oa_migrate_legacy_keywords', $arguments['source_keywords_vid'])
           ->sourceMigration($arguments['keywords_migration']);
      $this->addFieldMapping('field_oa_migrate_legacy_keywords:source_type')->defaultValue('tid');
    }

    // Map field_oa_media if it's a valid destination field.
    if (!empty($fields['field_oa_media'])) {
      $this->addFieldMapping('field_oa_media', 'upload')
        ->sourceMigration($arguments['file_migration']);
      $this->addFieldMapping('field_oa_media:file_class')
        ->defaultValue('MigrateFileFid');
      $this->addFieldMapping('field_oa_media:preserve_files')
        ->defaultValue(TRUE);
      $this->addFieldMapping('field_oa_media:description', 'upload:description');
      $this->addFieldMapping('field_oa_media:display', 'upload:list');
      $this->addFieldMapping('field_oa_media:language')
        ->defaultValue(LANGUAGE_NONE);
    }

    // Disable notifications for the length of the import.
    if (function_exists('oa_notification_skip')) {
      oa_notification_skip(TRUE);
    }

  }

  /**
   * Override complete() to do things after the node has been saved.
   */
  public function complete($entity, $row) {
    if (!$row->status) {
      // If the node was unpublished in OA1, then it's archived in OA2.
      if ($flag = flag_get_flag('trash')) {
        $flag->flag('flag', $entity->nid);
      }
    }
  }
}

class OaNodeGroupMigration extends OaNodeMigration {
  /**
   * The top-level Space to put new Spaces under by default.
   */
  protected $parentSpaceDefault;

  /**
   * A map from OA1 Group NIDs to the OA2 Space NID to put the new Space under.
   */
  protected $parentSpaceMap;

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->parentSpaceDefault = !empty($arguments['parent_space_default']) ? $arguments['parent_space_default'] : NULL;
    $this->parentSpaceMap = !empty($arguments['parent_space_map']) ? $arguments['parent_space_map'] : NULL;

    // We get the body NOT from the source node's body, but the og_description.
    $this->addFieldMapping('body', 'og_description', FALSE);
    $this->addFieldMapping('body:format', NULL, FALSE)
         ->defaultValue('panopoly_wysiwyg_text');

    if (isset($arguments['group_access']) && is_numeric($arguments['group_access'])) {
      // Force to a value specified in the config.
      $this->addFieldMapping('group_access')->defaultValue($arguments['group_access']);
    }
    else {
      // Map the og_private flag to the 'group_access' field.
      $this->addFieldMapping('group_access', 'og_private');
    }
  }

  /**
   * Override query() to join against the OG data.
   */
  protected function query() {
    $query = parent::query();
    $query->innerJoin('og', 'og', 'og.nid = n.nid');
    $query->fields('og', array('og_selective', 'og_description', 'og_theme', 'og_register', 'og_directory', 'og_language', 'og_private'));
    return $query;
  }

  /**
   * Implements Migration::prepare().
   */
  public function prepare($entity, $row) {
    // If the user specified a Space to create the new Space under, then find
    // the correct parent NID and set it.
    $parent_nid = NULL;
    if (!empty($this->parentSpaceMap) && !empty($this->parentSpaceMap[$row->nid])) {
      $parent_nid = $this->parentSpaceMap[$row->nid];
    }
    elseif (!empty($this->parentSpaceDefault)) {
      $parent_nid = $this->parentSpaceDefault;
    }
    if (!empty($parent_nid)) {
      $entity->oa_parent_space[LANGUAGE_NONE][0]['target_id'] = $parent_nid;
    }
  }

  /**
   * Implements Migration::complete().
   */
  public function complete($entity, $row) {
    // Hide the Space menu if requested by the user.
    if (isset($this->arguments['show_space_menu']) && !$this->arguments['show_space_menu']) {
      variable_set('oa_space_menu_' . $entity->nid, FALSE);
    }
  }
}

/**
 * Creating Sections (or Spaces) based on the existing of certain content.
 */
class OaSectionMigration extends DrupalMigration {
  /**
   * Associative array with information about the Section we will create.
   */
  protected $sectionSettings;

  /**
   * Associative array mapping section types to term IDs.
   */
  protected $sectionTypes;

  /**
   * The default language.
   */
  protected $defaultLanguage = LANGUAGE_NONE;

  public function __construct(array $arguments) {
    $this->destinationType = $arguments['destination_type'];
    $this->sourceType = $arguments['source_type'];
    if (!empty($arguments['default_language'])) {
      $this->defaultLanguage = $arguments['default_language'];
    }
    parent::__construct($arguments);

    $user_migration = $arguments['user_migration'];
    $this->dependencies[] = $user_migration;

    $group_migration = $arguments['group_migration'];
    $this->dependencies[] = $group_migration;

    $this->sectionSettings = (array)$arguments['section_settings'];

    $this->sourceFields += array(
      'nid' => t('Group NID'),
      'title' => t('Group title'),
      'uid' => t('Group author'),
      'created' => t('Group created timestamp'),
      'changed' => t('Group changed timestamp'),
      'status' => t('Group status'),
    );

    $this->destination = new MigrateDestinationNode($this->destinationType);

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

    if (!$this->newOnly) {
      $this->highwaterField = array(
        'name' => 'changed',
        'alias' => 'n',
        'type' => 'int',
      );
    }

    //
    // This is what normally would be in a D6-specific child class.
    //

    $query = $this->query();

    $this->source = new MigrateSourceSQL($query, $this->sourceFields, NULL, $this->sourceOptions);

    // TODO: we should do the same status -> archived mapping as nodes.
    $this->addSimpleMappings(array('created', 'changed'));
    $this->addFieldMapping('language', 'language')
         ->defaultValue($this->defaultLanguage);
    $this->addFieldMapping('uid', 'uid')
         ->sourceMigration($user_migration);

    if ($this->destinationType == 'oa_space') {
      // If we want to create a Space (rather than a Section), we need to use
      // a different field for the parent.
      $this->addFieldMapping('oa_parent_space', 'nid')
           ->sourceMigration($group_migration);
    }
    else {
      $this->addFieldMapping('og_group_ref', 'nid')
           ->sourceMigration($group_migration);
    }
  }

  /**
   * Query for the Spaces which contain the source content type.
   *
   * @return QueryConditionInterface
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('node', 'n')
      ->condition('n.type', $this->sourceType);
    $query->innerJoin('og_ancestry', 'oga', 'oga.nid = n.nid');
    $query->innerJoin('node', 'g', 'oga.group_nid = g.nid');
    $query
      ->groupBy('oga.group_nid')
      ->fields('g', array('nid', 'vid', 'language', 'title', 'uid', 'status', 'created', 'changed'))
      ->orderBy('g.changed');

    return $query;
  }

  /**
   * Get all the Section types.
   */
  protected function getSectionTypes() {
    if (empty($this->sectionTypes)) {
      $vocab_name = $this->destinationType == 'oa_space' ? 'space_type' : 'section_type';
      $vocab = taxonomy_vocabulary_machine_name_load($vocab_name);
      $this->sectionTypes = array();
      foreach (taxonomy_get_tree($vocab->vid) as $term) {
        $this->sectionTypes[$term->name] = $term->tid;
      }
    }
    return $this->sectionTypes;
  }

  /**
   * Overrides Migration::prepare().
   */
  public function prepare($entity, $row) {
    $section_settings = $this->sectionSettings;

    // Get the section type and remove it from the settings (we'll be using
    // rest of the settings array to build up the Section node later).
    $section_types = $this->getSectionTypes();
    $section_type_name = !empty($section_settings['section_type']) ? $section_settings['section_type'] : 'Default';
    unset($section_settings['section_type']);
    $section_type_tid = $section_types[$section_type_name];
    $section_type = taxonomy_term_load($section_type_tid);
    $panelizer_default = NULL;

    // Set the section type.
    if ($this->destinationType == 'oa_space') {
      $entity->field_oa_space_type[LANGUAGE_NONE][0]['tid'] = $section_type_tid;
    }
    else {
      $entity->field_oa_section[LANGUAGE_NONE][0]['tid'] = $section_type_tid;
    }

    // Set the panelizer default.
    if (!empty($section_type) && !empty($section_type->field_oa_section_layout[LANGUAGE_NONE][0]['value'])) {
      $entity->panelizer = array(
        'page_manager' => (object)array(
          'name' => $section_type->field_oa_section_layout[LANGUAGE_NONE][0]['value'],
        ),
      );
    }

    // Add all the defaults from $section_settings.
    foreach ($section_settings as $key => $value) {
      $entity->$key = $value;
    }

    // Fix up menu information if added.
    if (!empty($entity->menu)) {
      $entity->menu['enabled'] = TRUE;

      // Get the parent Space NID.
      $space_nid = $this->destinationType == 'oa_space' ?
        $entity->oa_parent_space[LANGUAGE_NONE][0]['target_id'] :
        $entity->og_group_ref[LANGUAGE_NONE][0]['target_id'];

      // Set the parent to the Space menu.
      $entity->menu['menu_name'] = 'og-menu-single';
      $entity->menu['plid'] = og_menu_single_get_link_mlid_or_create('node', $space_nid);

      // Set description if it's not set, to avoid PHP notice.
      if (!isset($entity->menu['description'])) {
        $entity->menu['description'] = '';
      }
    }
  }
}

class OaNodeGroupContentMigration extends OaNodeMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $user_migration = $arguments['user_migration'];
    $this->dependencies[] = $user_migration;

    $group_migration = $arguments['group_migration'];
    $this->dependencies[] = $group_migration;

    $section_migration = $arguments['section_migration'];
    $this->dependencies[] = $section_migration;

    // Map the Space.
    $this->addFieldMapping('og_group_ref', 'group_nid')
         ->sourceMigration($group_migration);

    // Map the Section.
    $this->addFieldMapping('oa_section_ref', 'group_nid')
         ->sourceMigration($section_migration);

    // Rather than taking the node.changed, use the revision.timestamp
    // directly so this works with any revision (not just the last).
    $this->addFieldMapping('changed', 'revision_timestamp', FALSE);

    // Normally, this will pull in the node author, so we need to override.
    $this->addFieldMapping('revision_uid', 'revision_uid', FALSE);
  }

  /**
   * Override query().
   */
  protected function query() {
    $query = parent::query();

    // Join against the OG data, so we know which group content belongs to.
    // Note: This has to be a subquery to limit nodes to a single Group.
    $subquery = Database::getConnection('default', $this->sourceConnection)
      ->select('og_ancestry', 'oga')
      ->fields('oga', array('nid'));
    $subquery->join('node', 'n', 'n.nid=oga.nid');
    $subquery->condition('n.type', $this->sourceType);
    $subquery->groupBy('oga.nid');
    $subquery->addExpression('MIN(oga.group_nid)', 'group_nid');
    $query->join($subquery, 'minoga', 'minoga.nid=n.nid');
    $query->fields('minoga', array('group_nid'));

    // Put the revision timestamp and uid in the fields.
    $query->addField('nr', 'timestamp', 'revision_timestamp');
    $query->addField('nr', 'uid', 'revision_uid');

    // If requested, change the query to refer to the first revision rather
    // than the last.
    if (!empty($this->arguments['first_revision'])) {
      $tables =& $query->getTables();
      $tables['nr']['condition'] = 'n.nid=nr.nid';
      if (isset($tables['f'])) {
        $tables['f']['condition'] = 'nr.vid=f.vid';
      }

      $subquery = Database::getConnection('default', $this->sourceConnection)
        ->select('node', 'n');
      $subquery->join('node_revisions', 'nr', 'n.nid=nr.nid');
      $subquery->condition('n.type', $this->sourceType);
      $subquery->groupBy('nr.nid');
      $subquery->addExpression('MIN(nr.vid)', 'vid');

      $query->join($subquery, 'minrev', 'nr.vid=minrev.vid');
    }

    return $query;
  }
}

class OaNodeGroupContentRevisionMigration extends DrupalNodeRevision6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $user_migration = $arguments['user_migration'];
    $this->dependencies[] = $user_migration;

    $group_migration = $arguments['group_migration'];
    $this->dependencies[] = $group_migration;

    $section_migration = $arguments['section_migration'];
    $this->dependencies[] = $section_migration;

    // Map the Space.
    $this->addFieldMapping('og_group_ref', 'group_nid')
         ->sourceMigration($group_migration);

    // Map the Section.
    $this->addFieldMapping('oa_section_ref', 'group_nid')
         ->sourceMigration($section_migration);

    // Map timestamp to revision timestamp.
    // @todo: Should this go in DrupalNodeRevision6Migration?
    $this->addFieldMapping('revision_timestamp', 'timestamp');

    $fields = $this->destination->fields();

    // Map the original nid to a common field.
    if (module_exists('oa_migrate_legacy') && !empty($fields['field_oa_migrate_legacy_id'])) {
      $this->addFieldMapping('field_oa_migrate_legacy_id', 'nid')
           ->description('We have a common field to save the legacy id (usually just nid).');
    }

    // Map field_oa_media if it's a valid destination field.
    if (!empty($fields['field_oa_media'])) {
      $this->addFieldMapping('field_oa_media', 'upload')
        ->sourceMigration($arguments['file_migration']);
      $this->addFieldMapping('field_oa_media:file_class')
        ->defaultValue('MigrateFileFid');
      $this->addFieldMapping('field_oa_media:preserve_files')
        ->defaultValue(TRUE);
      $this->addFieldMapping('field_oa_media:description', 'upload:description');
      $this->addFieldMapping('field_oa_media:display', 'upload:list');
      $this->addFieldMapping('field_oa_media:language')
        ->defaultValue(LANGUAGE_NONE);
    }
  }

  /**
   * Override Migration::query().
   */
  protected function query() {
    $query = parent::query();

    // Join against the OG data, so we know which group content belongs to.
    // Note: This has to be a subquery to limit nodes to a single Group.
    $subquery = Database::getConnection('default', $this->sourceConnection)
      ->select('og_ancestry', 'oga')
      ->fields('oga', array('nid'));
    $subquery->join('node', 'n', 'n.nid=oga.nid');
    $subquery->condition('n.type', $this->sourceType);
    $subquery->groupBy('oga.nid');
    $subquery->addExpression('MIN(oga.group_nid)', 'group_nid');
    $query->join($subquery, 'minoga', 'minoga.nid=n.nid');
    $query->fields('minoga', array('group_nid'));

    // Get the first revision number ('min_vid') so we can skip it.
    $subquery = Database::getConnection('default', $this->sourceConnection)
      ->select('node', 'n')
      ->fields('n', array('nid'));
    $subquery->join('node_revisions', 'nr', 'n.nid=nr.nid');
    $subquery->condition('n.type', $this->sourceType);
    $subquery->groupBy('nr.nid');
    $subquery->addExpression('MIN(nr.vid)', 'vid');
    $query->join($subquery, 'minrev', 'n.nid=minrev.nid');
    $query->addField('minrev', 'vid', 'min_vid');

    return $query;
  }

  /**
   * Override Migration::prepareRow().
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Skip the first revision, because we already got it in the node migration.
    if ($row->vid == $row->min_vid) {
      // But we need to record the mapping first.
      if ($dest_nid = $this->handleSourceMigration(array($this->arguments['node_migration']), $row->nid)) {
        $dest_vid = db_select('node', 'n')
          ->fields('n', array('vid'))
          ->condition('n.nid', $dest_nid)
          ->execute()
          ->fetchField();
        $hash = isset($row->migrate_map_hash) ? $row->migrate_map_hash : NULL;
        $this->map->saveIDMapping($row, array($dest_vid), MigrateMap::STATUS_IGNORED,
          MigrateMap::ROLLBACK_PRESERVE, $hash);
        $this->rollbackAction = MigrateMap::ROLLBACK_PRESERVE;
      }

      return FALSE;
    }
  }

  /**
   * Override Migration::prepare().
   */
  public function prepare($entity, $row) {
    // TODO: Why doesn't the normal Migrate mapping for this work?
    if (!empty($row->log)) {
      $entity->log = $row->log;
    }
    // Prevents PHP notice.
    $entity->vid = NULL;
  }
}

class OaNodeNotebookMigration extends OaNodeGroupContentMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
  }
}

class OaNodeRevisionNotebookMigration extends OaNodeGroupContentRevisionMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
  }
}

class OaNodeEventMigration extends OaNodeGroupContentMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_oa_date', 'field_date');

    // TODO: map the taxonomy somehow...
    //$this->addFieldMapping('og_vocabulary', 'taxonomy');
  }
}

class OaNodeDiscussionMigration extends OaNodeGroupContentMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    if (!empty($this->arguments['parent_migration'])) {
      // If a parent_migration is given, then we know that we're migrating the
      // replies, so we have to set the parent.
      $this->addFieldMapping('oa_parent', 'nid')
        ->sourceMigration($this->arguments['parent_migration']);

      // Also, since we're basically migrating comments, we need to change the
      // the map to go cid -> nid, rather than nid -> nid.
      $this->map = new MigrateSQLMap($this->machineName,
        array(
          'cid' => array('type' => 'int',
                         'unsigned' => TRUE,
                         'not null' => TRUE,
                         'description' => 'Source comment ID',
                         'alias' => 'c',
                        ),
        ),
        MigrateDestinationNode::getKeySchema()
      );

      // This also affects the $this->highwaterField.
      if (!$this->newOnly) {
        $this->highwaterField = array(
          'name' => 'timestamp',
          'alias' => 'c',
          'type' => 'int',
        );
      }
    }
  }

  /**
   * Overrides Migration::query().
   */
  protected function query() {
    $query = parent::query();

    if (!empty($this->arguments['parent_migration'])) {
      // If this is meant to be importing replies, then we need to rewrite the
      // query to go over comments.
      $query->join('comments', 'c', 'n.nid = c.nid');

      // Now, we need to remove certain node fields, and replace them with the
      // comment fields instead (because that is the true source of data).
      $fields =& $query->getFields();
      foreach(array('title', 'uid', 'created', 'changed', 'body', 'format', 'revision_uid', 'revision_timestamp') as $name) {
        unset($fields[$name]);
      }
      $query->fields('c', array('cid', 'uid', 'format', 'timestamp'));
      $query->addField('c', 'uid', 'revision_uid');
      $query->addField('c', 'subject', 'title');
      $query->addField('c', 'comment', 'body');
      $query->addField('c', 'timestamp', 'created');
      $query->addField('c', 'timestamp', 'changed');
      $query->addField('c', 'timestamp', 'revision_timestamp');
    }

    return $query;
  }

  /**
   * Overrides Migrate::prepareRow().
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if (!empty($row->cid)) {
      // Clear out the 'upload' data.
      $row->upload = array();
      $row->{'upload:description'} = array();
      $row->{'upload:list'} = array();
      $row->{'upload:weight'} = array();
      
      // Now replace it with data from 'comment_upload'.
      $connection = Database::getConnection('default', $this->arguments['source_connection']);
      if ($connection->schema()->tableExists('comment_upload')) {
        $result = $connection->select('comment_upload', 'u')
          ->fields('u', array('fid', 'description', 'list', 'weight'))
          ->condition('cid', $row->cid)
          ->orderBy('weight')
          ->execute();
        foreach ($result as $upload_row) {
          $row->upload[] = $upload_row->fid;
          $row->{'upload:description'}[] = $upload_row->description;
          $row->{'upload:list'}[] = $upload_row->list;
          $row->{'upload:weight'}[] = $upload_row->weight;
        }
      }
    }
  }

  /**
   * Overrides Migration::prepare().
   */
  public function prepare($entity, $row) {
    if (!empty($row->cid)) {
      // Add the 'cid' to the ID.
      $entity->field_oa_migrate_legacy_id[LANGUAGE_NONE][0]['value'] .= '#comment-' . $row->cid;
    }
  }
}
