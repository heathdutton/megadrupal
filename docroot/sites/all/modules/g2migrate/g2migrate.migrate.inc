<?php

/**
 * @file
 * Import photos and other data from Gallery2.
 *
 * Other parts of this module import data from Gallery2 SQL dumps and add data
 * to custom tables. This file uses the Migrate API to describe how to create
 * Drupal objects (nodes, taxonomy terms, etc.) from these data and the actual
 * photos.
 */

/**
 * Implements hook_migrate_api.
 *
 * You must implement hook_migrate_api(), setting the API level to 2, for
 * your migration classes to be recognized by the Migrate module.
 */
function g2migrate_migrate_api() {
  $api = array();
  $api['api'] = 2;
  $api['groups']['g2migrate']['title'] = t('Gallery2');
  $group_name = array('group_name' => 'g2migrate');
  $api['migrations'] = array(
    'Gallery2Album' => array('class_name' => 'Gallery2AlbumMigration') + $group_name,
    'Gallery2Node' => array('class_name' => 'Gallery2NodeMigration') + $group_name,
  );

  if (class_exists('MigrateDestinationRedirect')) {
    $api['migrations']['Gallery2Redirect'] =
      array('class_name' => 'Gallery2RedirectMigration') + $group_name;
  }

  return $api;
}

/**
 * Create taxonomy terms from the album structure.
 */
class Gallery2AlbumMigration extends Migration {
  /**
   * Base path for the Redirect module.
   *
   * @var String
   */
  protected $redirect_base = '';

  protected function computeRedirect($path) {
    return $this->redirect_base . $path;
  }

  public function __construct() {
    parent::__construct();
    $this->description = t('Create taxonomy terms from the Gallery2 album structure.');

    $query = db_select('g2migrate_Album', 'a');
    $query->join('g2migrate_ChildEntity', 'c', 'a.g_id = c.g_id');
    $query->join('g2migrate_Item', 'i', 'a.g_id = i.g_id');
    $query->join('g2migrate_ItemAttributesMap', 'ia', 'a.g_id = ia.g_itemId');
    $query->fields('a', array('g_id', 'path'))
      ->fields('c', array('g_parentid'))
      ->fields('i', array('g_title', 'g_summary', 'g_description'))
      ->fields('ia', array('g_orderWeight'))
      ->orderBy('depth', 'ASC');
    $this->source = new MigrateSourceSQL($query);

    // Import terms from album names in the g2migrate_Album table.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'g_id' => array(
          'description' => 'The primary key from the {g2migrate_Entity} table.',
          'type' => 'int',
          'size' => 'big',
          'not null' => TRUE,
          'alias' => 'a',
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    // Create terms in the selected vocabulary.
    $vocabulary = variable_get('g2migrate_vocabulary', 'g2migrate_album');
    $this->destination = new MigrateDestinationTerm($vocabulary);

    $dnm = array();
    $target_dnm = array('format', 'parent_name');
    foreach (_g2migrate_sources('taxonomy_term') as $key => $field) {
      $target = variable_get("g2migrate_taxonomy_$key", $field['default']);
      if ($target == 'none') {
        $dnm[] = $key;
      }
      else {
        // The order of the arguments is target, source, as in $target = $source
        $this->addFieldMapping($target, $key);
        if ($field['field_type'] == 'text' && !in_array($target, array('name', 'description'))) {
          $target_dnm[] = "$target:language";
        }
      }
    }
    $this->addFieldMapping('weight', 'g_orderWeight');
    $this->addFieldMapping('parent', 'g_parentid')
      ->sourceMigration('Gallery2Album');

    // If the Redirect handler has been defined and the base path has been
    // configured, then create a redirect. See
    // https://drupal.org/comment/7603833#comment-7603833
    if (class_exists('MigrateRedirectEntityHandler') && $this->redirect_base = variable_get('g2migrate_redirect', '')) {
      $this->addFieldMapping('migrate_redirects', 'path')
        ->callbacks(array($this, 'computeRedirect'));
    }

    $this->addUnmigratedSources($dnm);
    $this->addUnmigratedDestinations($target_dnm);

    // We conditionally DNM these fields, so your field mappings will be clean
    // whether or not you have path and or pathauto enabled
    if (module_exists('path')) {
      $this->addFieldMapping('path')
           ->issueGroup(t('DNM'));
      if (module_exists('pathauto')) {
        $this->addFieldMapping('pathauto')
             ->issueGroup(t('DNM'));
      }
    }

  }
}

/**
 * Gallery2 base class.
 *
 * The field mappings will depend on whether we create nodes (using the core
 * Image field) or Media entities. This class handles the source data, common to
 * both.
 */
abstract class Gallery2BaseMigration extends Migration {
  /**
   * Base path for the Redirect module.
   *
   * @var String
   */
  protected $redirect_base = '';

  protected function computeRedirect($path) {
    return $this->redirect_base . $path;
  }

  public function __construct() {
    parent::__construct();

    // The Album vocabulary holds the album structure from Gallery2. Make sure
    // to import that first, so that now we can attach terms to photos.
    $this->dependencies = array('Gallery2Album');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'g_id' => array(
          'description' => 'size was 11',
          'type' => 'int',
          'size' => 'big',
          'not null' => TRUE,
          'alias' => 'e',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // TODO: What is the difference between g_originationTimestamp in the Item
    // table and g_creationTimestamp in the Entity table?
    $query = db_select('g2migrate_Entity', 'e');
    $query->join('g2migrate_Item', 'i', 'e.g_id = i.g_id');
    $query->join('g2migrate_ChildEntity', 'c', 'e.g_id = c.g_id');
    $query->join('g2migrate_Album', 'a', 'c.g_parentid = a.g_id');
    $query->join('g2migrate_FileSystemEntity', 'f', 'e.g_id = f.g_id');
    $query->join('g2migrate_ItemAttributesMap', 'ia', 'e.g_id = ia.g_itemId');
    $query->condition('g_entityType', 'GalleryPhotoItem')
      ->fields('e', array('g_id'))
      ->fields('i', array('g_description', 'g_keywords', 'g_summary', 'g_title', 'g_originationTimestamp'))
      ->fields('c', array('g_parentid'))
      ->fields('ia', array('g_orderWeight'))
      ->addExpression('CONCAT(a.path, \'/\', f.g_pathComponent)', 'path');
    $this->source = new MigrateSourceSQL($query);

    // TODO: Keep track of owners.
    $this->addFieldMapping('uid')
      ->defaultValue(1);

    // Publish, do not promote to the front page, and do not make sticky.
    $this->addFieldMapping('status')
      ->defaultValue(1);
    $this->addFieldMapping('promote')
      ->defaultValue(0);
    $this->addFieldMapping('sticky')
      ->defaultValue(0);

    if (module_exists('path')) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (module_exists('pathauto')) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(array('totalcount', 'daycount', 'timestamp'));
    }

    // If the Redirect handler has been defined and the base path has been
    // configured, then create a redirect. See
    // https://drupal.org/comment/7603833#comment-7603833
    if (class_exists('MigrateRedirectEntityHandler') && $this->redirect_base = variable_get('g2migrate_redirect', '')) {
      $this->addFieldMapping('migrate_redirects', 'path')
        ->callbacks(array($this, 'computeRedirect'));
    }

  }
}

/**
 * The moment you have been waiting for: import those photos.
 *
 * This class handles photos attached using the core Image field. Use
 * Gallery2Media1NodeMigration instead if you are using the Media module.
 */
class Gallery2NodeMigration extends Gallery2BaseMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Create nodes and attach photos as Image fields from the Gallery2 album.');

    $node_type = variable_get('g2migrate_node_type', 'g2migrate_photo');
    $this->destination = new MigrateDestinationNode($node_type);

    // For each key in $sources, there is a corresponding persistent variable
    // holding the name of the field to use. Other data are stored in the
    // $sources array rather than here to avoid defining two arrays and keeping
    // them synchronized.
    $dnm = array();
    $unmigrated = array(
      'translate', 'changed',
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

    // These fields are supported by the core image field but not by Media.
    $field = variable_get('g2migrate_field_path', 'g2migrate_image');
    // $info = field_info_field($field);
    $this->addFieldMapping("$field:alt", 'g_title');
    $this->addFieldMapping("$field:title", 'g_title');

    // Unmapped source fields
    $this->addUnmigratedSources($dnm);
    // Unmapped destination fields
    $this->addUnmigratedDestinations($unmigrated);

  }
}

/**
 * Gallery2 redirect class.
 *
 * If the Redirect module is available, then provide a migration class to
 * redirect from legacy Gallery2 album pages to those provided by the view.
 * See https://drupal.org/comment/6057284#comment-6057284.
 */
class Gallery2RedirectMigration extends Migration {
  /**
   * Base path for the Redirect module.
   *
   * @var String
   */
  protected $redirect_base = '';

  protected function computeRedirect($path) {
    return $this->redirect_base . $path;
  }

  protected function computeTargetPath($tid) {
    $view_name = variable_get('g2migrate_album_view', 'g2migrate_albums');
    $display = views_get_view($view_name)->display['page_album'];
    return str_replace('/%/', "/$tid/", $display->display_options['path']);
  }

  public function __construct() {
    parent::__construct();
    $this->description = t('Redirect from your legacy Gallery2 album pages to the views of your migrated photos.');
    $this->redirect_base = variable_get('g2migrate_redirect', '');

    // The Album vocabulary holds the album structure from Gallery2.
    $this->dependencies = array('Gallery2Album');

    // $query = db_select('g2migrate_Album', 'a');
    // $query->join('g2migrate_ChildEntity', 'c', 'a.g_id = c.g_id');
    // $query->join('g2migrate_Item', 'i', 'a.g_id = i.g_id');
    // $query->join('g2migrate_ItemAttributesMap', 'ia', 'a.g_id = ia.g_itemId');
    // $query->fields('a', array('g_id', 'path'))
      // ->fields('c', array('g_parentid'))
      // ->fields('i', array('g_title', 'g_summary', 'g_description'))
      // ->fields('ia', array('g_orderWeight'))
      // ->orderBy('depth', 'ASC');
    // $this->source = new MigrateSourceSQL($query);

    $query = db_select('g2migrate_Album', 'a');
    $query->fields('a', array('g_id', 'path'))
      ->orderBy('depth', 'ASC');
    $this->source = new MigrateSourceSQL($query);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'g_id' => array(
          'description' => 'size was 11',
          'type' => 'int',
          'size' => 'big',
          'not null' => TRUE,
          'alias' => 'a',
        ),
      ),
      MigrateDestinationRedirect::getKeySchema()
    );

    $this->destination = new MigrateDestinationRedirect();

    $this->addFieldMapping('source', 'path')
      ->callbacks(array($this, 'computeRedirect'))
      ->description(t('Prepend the legacy base path.'));
    $this->addFieldMapping('redirect', 'g_id')
      ->sourceMigration('Gallery2Album')
      ->callbacks(array($this, 'computeTargetPath'))
      ->description(t('Convert to taxonomy ID and insert into the view path.'));

    $target_dnm = array(
      'rid',
      'hash',
      'type',
      'uid',
      'source_options',
      'redirect_options',
      'language',
      'status_code',
      'count',
      'access',
    );
    $this->addUnmigratedDestinations($target_dnm);
  }

}
