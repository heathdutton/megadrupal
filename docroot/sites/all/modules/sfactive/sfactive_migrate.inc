<?php

/**
 * Base sf-active migration class.
 */
abstract class SFActiveMigration extends Migration {

  /**
   * List of columns which may require character re-encoding.
   */
  public $blobColumns = array();

  /**
   * List of columns which may require HTML entity decoding.
   */
  public $entityColumns = array();

  /**
   * Assume Windows-1252 encoding for invalid UTF-8 data.
   *
   * @see http://en.wikipedia.org/wiki/Windows-1252
   */
  public $fallbackEncoding = 'Windows-1252';

  public function __construct() {
    parent::__construct();
    $this->fallbackEncoding = variable_get('sfactive_migrate_encoding', $this->fallbackEncoding);
    $this->timezone = variable_get('sfactive_migrate_timezone', variable_get('date_default_timezone', @date_default_timezone_get()));
  }

  public function prepareRow($row) {
    $this->repairBlobColumns($row);
    $this->repairEntityColumns($row);
  }

  /**
   * The name of the MySQL database for the sf-active site.
   */
  public function sfactiveTable($table) {
    global $databases;
    return variable_get('sfactive_migrate_database', $databases['default']['default']['database']) . '.' . $table;
  }

  /**
   * Absolute path to sf-active document root containing uploads and images directories.
   */
  public function sfactiveFiles($files) {
    return variable_get('sfactive_migrate_files', DRUPAL_ROOT) . DIRECTORY_SEPARATOR . $files;
  }

  /**
   * Move attached file to the same path it was stored in on sf-active site.
   */
  public function moveFile($delta, $entity, $row) {
    if (!empty($delta['fid'])) {
      $directory = "public://{$row->year}/{$row->month}";
      if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
        watchdog('sfactive', 'Error creating directory $directory.', array('%directory' => $directory), WATCHDOG_ERROR);
      }
      elseif (!file_move(file_load($delta['fid']), $directory, FILE_EXISTS_RENAME)) {
        watchdog('sfactive', 'Error moving file %uri.', array('%uri' => $file->uri), WATCHDOG_ERROR);
      }
      // Resave file with a more useful timestamp.
      db_update('file_managed')
        ->fields(array('timestamp' => $entity->changed))
        ->condition('fid', $delta['fid'])
        ->execute();
    }
  }

  /**
   * Attempt to repair invalid UTF-8 in feature.summary and webcast.article columns.
   */
  public function repairBlobColumns($row) {
    foreach ($this->blobColumns as $column) {
      if (!drupal_validate_utf8($row->$column)) {
        $row->$column = drupal_convert_to_utf8($row->$column, $this->fallbackEncoding);
        if ($row->$column === FALSE) {
          watchdog('sfactive', 'UTF-8 conversion failed for %column column; all data lost.', array('%column' => $column), WATCHDOG_ERROR);
        }
      }
    }
  }

  /**
   * Decode HTML entities in non-HTML columns.
   */
  public function repairEntityColumns($row) {
    foreach ($this->entityColumns as $column) {
      $row->$column = decode_entities($row->$column);
    }
  }
}

/**
 * Define common attached file handling for webcast and event tables.
 */
abstract class SFActiveLinkedFileMigration extends SFActiveMigration {
  public $blobColumns = array('article');
  public $entityColumns = array('heading', 'author', 'contact', 'link', 'address', 'phone');

  /**
   * Columns which may require HTML entity conversion, depending on format.
   */
  public $markupColumns = array('article', 'summary');

  /**
   * Database column to which text files should be appended.
   */
  public $textFileColumn = 'article';

  /**
   * Decode quote entities in HTML and all HTML entities in plain text.
   */
  public function repairMarkupColumns($row) {
    foreach ($this->markupColumns as $column) {
      if ($row->artmime == 'h') {
        $row->$column = str_replace('&quot;', '"', $row->$column);
      }
      else {
        $original = $row->$column;
        $row->$column = decode_entities($row->$column);
        if (!drupal_validate_utf8($row->$column)) {
          watchdog('sfactive', 'Invalid UTF-8 found after decoding HTML entities; rolling back entity decoding.', NULL, WATCHDOG_WARNING);
          $row->$column = $original;
        }
      }
    }
  }

  public function __construct() {
    parent::__construct();
    // Check that webcast table exists.
    if (!db_table_exists($table = $this->sfactiveTable('webcast'))) {
      watchdog('sfactive', 'sf-active database table %table not found; please configure the sf-active database.', array('%table' => $table), WATCHDOG_ERROR);
      drupal_set_message(t('sf-active database table %table not found; please configure the sf-active database.', array('%table' => $table)), 'error');
      drupal_goto('admin/content/migrate/sfactive', array('query' => array('destination' => 'admin/content/migrate')));
    }
    // Check that uploads directory exists.
    if (!is_readable($files = $this->sfactiveFiles('uploads'))) {
      watchdog('sfactive', 'sf-active files directory %files not found or not readable; please configure the sf-active files path.', array('%files' => $files), WATCHDOG_ERROR);
      drupal_set_message(t('sf-active files directory %files not found or not readable; please configure the sf-active files path.', array('%files' => $files)), 'error');
      drupal_goto('admin/content/migrate/sfactive', array('query' => array('destination' => 'admin/content/migrate')));
    }
  }

  /**
   * Returns an array of comments that are parents of themselves.
   */
  public function getCircularComments() {
    static $array;
    if (!isset($array)) {
      $array = db_query('SELECT id, parent_id FROM ' . $this->sfactiveTable('webcast') . ' WHERE id = parent_id')->fetchAllKeyed();
    }
    return $array;
  }

  /**
   * Find the actual location of the linked_file based on created date.
   */
  public function prepareLinkedFile($row) {
    return $this->sfactiveFiles("uploads/{$row->year}/{$row->month}/" . basename($row->linked_file));
  }

  /**
   * Setup the article creation year and month.
   */
  public function prepareDate($row) {
    $date = date_parse($row->created);
    $row->year = $date['year'];
    $row->month = sprintf('%02u', $date['month']);
  }

  public function prepareRow($row) {
    parent::prepareRow($row);
    $this->repairMarkupColumns($row);
    // Artmime represents the format: t = text/plain and h = text/html.
    $row->format = ($row->artmime == 'h') ? 'filtered_html' : 'plain_text';
    // Set up the image and file fields.
    // @todo: Switch to one media field instead?
    if ($row->linked_file) {
      $row->linked_file = $this->prepareLinkedFile($row);
      $extension = pathinfo($row->linked_file, PATHINFO_EXTENSION);
      if (!file_exists($row->linked_file)) {
        watchdog('sfactive', 'Linked file %linked_file does not exist and cannot be migrated.', array('%linked_file' => $row->linked_file), WATCHDOG_ERROR);
      }
      elseif (!is_readable($row->linked_file)) {
        watchdog('sfactive', 'Linked file %linked_file is not readable and cannot be migrated.', array('%linked_file' => $row->linked_file), WATCHDOG_ERROR);
      }
      elseif (in_array($extension, array('gif', 'jpeg', 'jpg', 'png'))) {
        $row->image = $row->linked_file;
      }
      elseif (in_array($extension, array('txt', 'htm', 'html'))) {
        $data = file_get_contents($row->linked_file);
        // The text and HTML files are *sometimes* saved with backslashes.
        // Probably depends if the file was uploaded in $_POST or $_FILES.
        if (is_int(strpos($data, '\"')) || is_int(strpos($data, "\'"))) {
          $data = stripslashes($data);
        }
        if (in_array($extension, array('htm', 'html'))) {
          $doc = new DOMDocument();
          // Let's ignore HTML parse errors :)
          @$doc->loadHTML($data);
          if ($doc->encoding && strcasecmp($doc->encoding, 'utf-8')) {
            watchdog('sfactive', 'Converting uploaded HTML file %linked_file to UTF-8 from %encoding.', array('%linked_file' => $row->linked_file, '%encoding' => $doc->encoding));
            $data = drupal_convert_to_utf8($data, $doc->encoding);
            if ($data === FALSE) {
              watchdog('sfactive', 'UTF-8 conversion failed for %linked_file. Data lost.', array('%linked_file' => $row->linked_file), WATCHDOG_ERROR);
            }
          }
          $row->format = 'filtered_html';
        }
        if (!drupal_validate_utf8($data)) {
          watchdog('sfactive', 'Invalid UTF-8 data found in %linked_file. Attempting to repair; assuming %fallbackEncoding. Some data may be lost.', array('%linked_file' => $row->linked_file, '%fallbackEncoding' => $this->fallbackEncoding), WATCHDOG_WARNING);
          $data = drupal_convert_to_utf8($data, $this->fallbackEncoding);
          if ($data === FALSE) {
            watchdog('sfactive', 'UTF-8 conversion failed for %linked_file. All data lost.', array('%linked_file', $row->linked_file), WATCHDOG_ERROR);
          }
        }
        if ($row->{$this->textFileColumn}) {
          $row->{$this->textFileColumn} .= "\n";
        }
        $row->{$this->textFileColumn} .= $data;
      }
      else {
        $row->file = $row->linked_file;
      }
    }
  }

  public function complete($entity, $row) {
    if (!empty($entity->field_image)) {
      foreach ($entity->field_image as $language) {
        foreach ($language as $delta) {
          $this->moveFile($delta, $entity, $row);
        }
      }
    }
    if (!empty($entity->field_file)) {
      foreach ($entity->field_file as $language) {
        foreach ($language as $delta) {
          $this->moveFile($delta, $entity, $row);
        }
      }
    }
  }
}

/**
 * Defines category migration.
 */
class SFActiveCategoryMigration extends SFActiveMigration {
  public function prepareRow($row) {
    parent::prepareRow($row);
    $row->shortname = 'features/' . $row->shortname;
  }

  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate categories from the source database to taxonomy terms');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'category_id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Category ID',
        'alias' => 'category',
      ),
    ), MigrateDestinationTerm::getKeySchema());
    $query = db_select($this->sfactiveTable('category'), 'category')
      ->fields('category', array('category_id', 'description', 'name', 'order_num', 'shortname'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm('category');
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('description', 'description');
    $this->addFieldMapping('weight', 'order_num');
    $this->addFieldMapping('path', 'shortname');
    $this->addUnmigratedDestinations(array('format', 'parent_name', 'parent'));
  }
}

/**
 * Defines user migration.
 */
class SFActiveUserMigration extends SFActiveMigration {
  public function prepareRow($row) {
    parent::prepareRow($row);
    $row->lastlogin .= ' ' . $this->timezone;
  }

  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate sf-active users.');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'user_id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'User ID',
        'alias' => 'user',
      ),
    ), MigrateDestinationUser::getKeySchema());
    $query = db_select($this->sfactiveTable('user'), 'user')
      ->fields('user', array('username', 'password', 'first_name', 'last_name', 'email', 'phone', 'user_id', 'lastlogin'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();
    $this->addFieldMapping('name', 'username');
    $this->addFieldMapping('mail', 'email');
    $this->addFieldMapping('init', 'email');
    $this->addFieldMapping('pass', 'password');
    $this->addFieldMapping('access', 'lastlogin');
    $this->addFieldMapping('login', 'lastlogin');
    $this->addFieldMapping('created', 'lastlogin');
    $this->addFieldMapping('roles')
      ->defaultValue(drupal_map_assoc(array(3)));
    $this->addFieldMapping('field_first_name', 'first_name');
    $this->addFieldMapping('field_last_name', 'last_name');
    $this->addFieldMapping('field_phone', 'phone');
    $this->addFieldMapping('status')
      ->defaultValue(1);
    $this->addUnmigratedDestinations(array('theme', 'signature', 'signature_format', 'timezone', 'language', 'picture', 'path', 'pathauto_perform_alias'));
  }

  /**
   * Update users.pass column to indicate hashed MD5 not hashed password.
   */
  public function complete($entity, $row) {
    db_update('users')
      ->expression('pass', 'CONCAT(:prefix, pass)', array(':prefix' => 'U'))
      ->condition('uid', $entity->uid) 
      ->execute();
  }
}

/**
 * Defines article migration.
 */
class SFActiveArticleMigration extends SFActiveLinkedFileMigration {
  public function prepareRow($row) {
    $this->prepareDate($row);
    parent::prepareRow($row);
    $row->created .= ' ' . $this->timezone;
    $row->modified .= ' ' . $this->timezone;
    $row->promote = in_array($row->display, array('l', 'g')) ? NODE_PROMOTED : NODE_NOT_PROMOTED;
    $row->status = ($row->display == 'f') ? NODE_NOT_PUBLISHED : NODE_PUBLISHED;
    $row->path = 'news/' . $row->year . '/' . $row->month . '/' . $row->id . '.php';
    // Execute a separate query to get the categories, due to slow temporary
    // table and filesort if we use GROUP BY in the main query.
    $row->terms = db_query('SELECT GROUP_CONCAT(catlink.catid) FROM ' . $this->sfactiveTable('catlink') . ' catlink WHERE id = :id', array(':id' => $row->id))->fetchField();
  }

  public function __construct() {
    parent::__construct();
    $this->description = t('Articles');
    $this->dependencies = array('SFActiveCategory');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Article ID',
        'alias' => 'webcast',
      ),
    ), MigrateDestinationNode::getKeySchema());
    $query = db_select($this->sfactiveTable('webcast'), 'webcast')
      ->fields('webcast', array('id', 'heading', 'author', 'article', 'contact', 'link', 'address', 'phone', 'summary', 'modified', 'created', 'linked_file', 'display', 'artmime'));
    if ($this->getCircularComments()) {
      $query->condition(db_or()->condition('parent_id', 0)->condition('webcast.id', $this->getCircularComments()));
    }
    else {
      $query->condition('parent_id', 0);
    }
    // Set up some properties to define later.
    $query->addExpression('NULL', 'promote');
    $query->addExpression('NULL', 'status');
    $query->addExpression('NULL', 'image');
    $query->addExpression('NULL', 'file');
    $query->addExpression('NULL', 'path');
    $query->addExpression('NULL', 'terms');

    $count_query = db_select($this->sfactiveTable('webcast'), 'webcast');
    if ($this->getCircularComments()) {
      $count_query->condition(db_or()->condition('parent_id', 0)->condition('id', $this->getCircularComments()));
    }
    else {
      $count_query->condition('parent_id', 0);
    }
    $count_query->addExpression('COUNT(*)', 'count');

    $this->source = new MigrateSourceSQL($query, array(), $count_query);

    $this->destination = new MigrateDestinationNode('article');
    $this->addFieldMapping('title', 'heading');
    $this->addFieldMapping('is_new')
      ->defaultValue(TRUE);
    $this->addFieldMapping('field_category', 'terms')
      ->sourceMigration('SFActiveCategory')
      ->arguments(array('source_type' => 'tid'))
      ->separator(',');
    $arguments = MigrateTextFieldHandler::arguments(array('source_field' => 'summary'), array('source_field' => 'format'));
    $this->addFieldMapping('body', 'article')
      ->arguments($arguments);
    $this->addFieldMapping('field_image', 'image');
    $this->addFieldMapping('field_file', 'file');
    $this->addFieldMapping('sticky')
      ->defaultValue(NODE_NOT_STICKY);
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('promote', 'promote');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('uid')
      ->defaultValue(0);
    $this->addFieldMapping('revision_uid')
      ->defaultValue(0);
    $this->addFieldMapping('field_author', 'author');
    $this->addFieldMapping('field_contact', 'contact');
    $this->addFieldMapping('field_link', 'link');
    $this->addFieldMapping('field_address', 'address');
    $this->addFieldMapping('field_phone', 'phone');
    $this->addFieldMapping('path', 'path');
    $this->addUnmigratedDestinations(array('revision', 'comment', 'field_tags', 'pathauto_perform_alias', 'language'));
    $this->addUnmigratedSources(array('summary', 'linked_file', 'display', 'artmime'));
  }

  public function complete($entity, $row) {
    parent::complete($entity, $row);
    if ($row->display == 'l') {
      flag('flag', 'local', $entity->nid, user_load(1));
    }
    elseif ($row->display == 'g') {
      flag('flag', 'global', $entity->nid, user_load(1));
    }
    db_merge('sfactive_legacy')
      ->key(array('type' => 'article', 'id' => $row->id))
      ->fields(array('nid' => $entity->nid))
      ->execute();
  }
}

/**
 * Defines comment and attachment migration to comments.
 */
class SFActiveCommentMigration extends SFActiveLinkedFileMigration {
  public function prepareRow($row) {
    $this->prepareDate($row);
    parent::prepareRow($row);
    $row->created .= ' ' . $this->timezone;
    $row->modified .= ' ' . $this->timezone;
    $row->status = ($row->display == 'f') ? COMMENT_NOT_PUBLISHED : COMMENT_PUBLISHED;
    $row->subject = truncate_utf8($row->heading, 64, FALSE, TRUE);
    if ($row->subject != $row->heading) {
      if ($row->article) {
        $row->article = "\n" . $row->article;
      }
      $row->article = $row->heading . $row->article;
    }
    $row->contact = truncate_utf8($row->contact, 64, FALSE, TRUE);
    if ($parent = $this->getParentArticle($row->parent_id)) {
      $row->nid = $parent->id;
    }
    else {
      // Assign orphaned comments/attachments to first article.
      watchdog('sfactive', 'Orphaned comment attached to first article.', NULL, WATCHDOG_WARNING);
      $row->nid = $this->getFirstArticle();
    }
    $row->pid = 0;
    $result = db_select($this->sfactiveTable('webcast'), 'webcast')
      ->fields('webcast', array('id', 'parent_id'))
      ->condition('id', $row->parent_id)
      ->execute();
    foreach ($result as $parent) {
      if ($parent->parent_id != 0 && $parent->parent_id != $parent->id) {
        // Parent is a comment, not an article.
        $row->pid = $row->parent_id;
      }
    }
  }

  /**
   * Recursive function to find parent article of comments and attachments.
   */
  public function getParentArticle($parent_id) {
    $result = db_select($this->sfactiveTable('webcast'), 'webcast')
      ->fields('webcast', array('id', 'parent_id'))
      ->condition('id', $parent_id)
      ->execute();
    foreach ($result as $row) {
      if ($row->parent_id == 0 || $row->parent_id == $row->id) {
        return $row;
      }
      else {
        return $this->getParentArticle($row->parent_id);
      }
    }
  }

  /**
   * Get first article for use as parent node of orphan comments.
   */
  public function getFirstArticle() {
    static $article_id;
    if (!isset($article_id)) {
      $query = db_select($this->sfactiveTable('webcast'), 'webcast')
        ->condition('parent_id', 0);
      $query->addExpression('MIN(id)');
      $article_id = $query->execute()->fetchField();
    }
    return $article_id;
  }

  public function __construct() {
    parent::__construct();
    $this->description = 'Article comments and attachments.';
    $this->dependencies = array('SFActiveArticle');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Comment ID',
        'alias' => 'webcast',
      ),
    ), MigrateDestinationComment::getKeySchema());
    $query = db_select($this->sfactiveTable('webcast'), 'webcast')
      ->fields('webcast', array('id', 'parent_id', 'heading', 'author', 'article', 'contact', 'link', 'address', 'phone', 'summary', 'modified', 'created', 'linked_file', 'display', 'artmime'))
      ->condition('parent_id', 0, '<>')
      ->orderBy('parent_id', 'ASC');
    if ($this->getCircularComments()) {
      $query->condition('webcast.id', $this->getCircularComments(), 'NOT IN');
    }
    // Set up some properties to define later.
    $query->addExpression('NULL', 'pid');
    $query->addExpression('NULL', 'nid');
    $query->addExpression('NULL', 'subject');
    $query->addExpression('NULL', 'status');
    $query->addExpression('NULL', 'image');
    $query->addExpression('NULL', 'file');

    $count_query = db_select($this->sfactiveTable('webcast'), 'webcast')
      ->condition('parent_id', 0, '<>');
    if ($this->getCircularComments()) {
      $count_query->condition('id', $this->getCircularComments(), 'NOT IN');
    }
    $count_query->addExpression('COUNT(*)', 'count');

    $this->source = new MigrateSourceSQL($query, array(), $count_query);

    $this->destination = new MigrateDestinationComment('comment_node_article');

    $this->addFieldMapping('pid', 'pid')
      ->sourceMigration('SFActiveComment')
      ->description('Parent comment.');
    $this->addFieldMapping('nid', 'nid')
      ->sourceMigration('SFActiveArticle')
      ->description('Parent article.');
    $this->addFieldMapping('uid', NULL)
      ->defaultValue(0);
    $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('name', 'author');
    $this->addFieldMapping('mail', 'contact');
    $this->addFieldMapping('homepage', 'link');
    // Assume comments and attachments have no summary.
    // This may not be correct.. in which case fix me.
    $arguments = MigrateTextFieldHandler::arguments(NULL, array('source_field' => 'format'));
    $this->addFieldMapping('comment_body', 'article')
      ->arguments($arguments);
    $this->addFieldMapping('field_address', 'address');
    $this->addFieldMapping('field_phone', 'phone');
    $this->addFieldMapping('field_image', 'image');
    $this->addFieldMapping('field_file', 'file');
    $this->addUnmigratedDestinations(array('hostname', 'thread', 'path', 'language'));
    $this->addUnmigratedSources(array('heading', 'summary', 'linked_file', 'display', 'artmime', 'parent_id'));
  }
}

/**
 * Defines feature story migration.
 */
class SFActiveFeatureMigration extends SFActiveMigration {
  public $blobColumns = array('summary');
  public $entityColumns = array('title1', 'title2', 'tag');

  public function prepareRow($row) {
    parent::prepareRow($row);
    $row->creation_date .= ' ' . $this->timezone;
    $row->modification_date .= ' ' . $this->timezone;
    if (strtotime($row->display_date) === FALSE) {
      $row->display_date = NULL;
    }
    else {
      $row->display_date .= ' ' . $this->timezone;
    }
    if ($row->image) {
      $row->image_path = urldecode(substr(parse_url($row->image, PHP_URL_PATH), 1));
      $row->image_path = str_replace('local/webcast/uploads/', 'uploads/', $row->image_path);
      if (strpos($row->image_path, 'uploads/') === 0 && substr_count($row->image_path, '/') === 1) {
        $files = file_scan_directory($this->sfactiveFiles('uploads'), '/^' . preg_quote(basename($row->image_path)) . '$/');
        if ($file = array_shift($files)) {
          $row->image_path = $file->uri;
        }
        else {
          $row->image_path = $this->sfactiveFiles($row->image_path);
        }
      }
      else {
        $row->image_path = $this->sfactiveFiles($row->image_path);
      }
      if (!file_exists($row->image_path)) {
        watchdog('sfactive', 'Image %image (searched for %image_path) does not exist and cannot be migrated.', array('%image' => $row->image, '%image_path' => $row->image_path), WATCHDOG_ERROR);
        $row->image_path = NULL;
      }
      elseif (!is_readable($row->image_path)) {
        watchdog('sfactive', 'Image %image (searched for %image_path) is not readable and cannot be migrated.', array('%image' => $row->image, '%image_path' => $row->image_path), WATCHDOG_ERROR);
        $row->image_path = NULL;
      }
      if ($row->image_path) {
        $date = date_parse($row->creation_date);
        $row->year = $date['year'];
        $row->month = sprintf('%02u', $date['month']);
      }
    }
    $row->status = ($row->status == 'h') ? NODE_NOT_PUBLISHED : NODE_PUBLISHED;
  }

  public function __construct() {
    parent::__construct();
    $this->description = 'Feature stories.';
    $this->dependencies = array('SFActiveCategory', 'SFActiveUser');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'feature_id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Feature ID',
        'alias' => 'feature',
      ),
    ), MigrateDestinationNode::getKeySchema());
    $query = db_select($this->sfactiveTable('feature'), 'feature')
      ->fields('feature', array('feature_id', 'summary', 'title1', 'title2', 'display_date', 'category_id', 'creation_date', 'creator_id', 'status', 'tag', 'image', 'modification_date', 'modifier_id', 'image_link'))
      ->condition('is_current_version', 1);
    $query->addExpression('NULL', 'image_path');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('feature');
    $this->addFieldMapping('title', 'title2');
    $this->addFieldMapping('is_new')
      ->defaultValue(TRUE);
    $arguments = MigrateTextFieldHandler::arguments(NULL, 'filtered_html');
    $this->addFieldMapping('body', 'summary')
      ->arguments($arguments);
    $this->addFieldMapping('field_category', 'category_id')
      ->sourceMigration('SFActiveCategory')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_subhead', 'title1');
    $this->addFieldMapping('field_image', 'image_path');
    $this->addFieldMapping('field_image:alt', 'tag');
    $this->addFieldMapping('field_image:title', 'tag');
    $this->addFieldMapping('uid', 'creator_id')
      ->sourceMigration('SFActiveUser')
      ->defaultValue(0);
    $this->addFieldMapping('revision_uid', 'modifier_id')
      ->sourceMigration('SFActiveUser')
      ->defaultValue(0);
    $this->addFieldMapping('field_link', 'image_link');
    // Save the display date in the site time zone rather than UTC.
    $this->addFieldMapping('field_date', 'display_date')
      ->arguments(array('timezone' => $this->timezone));
    $this->addFieldMapping('created', 'creation_date');
    $this->addFieldMapping('changed', 'modification_date');
    $this->addFieldMapping('status', 'status');    
    $this->addFieldMapping('sticky')
      ->defaultValue(NODE_NOT_STICKY);
    $this->addFieldMapping('promote')
      ->defaultValue(NODE_PROMOTED);
    $this->addUnmigratedDestinations(array('revision', 'path', 'comment', 'pathauto_perform_alias', 'language'));
    $this->addUnmigratedSources(array('tag', 'image'));
  }

  public function complete($entity, $row) {
    if (!empty($entity->field_image)) {
      foreach ($entity->field_image as $language) {
        foreach ($language as $delta) {
          $this->moveFile($delta, $entity, $row);
        }
      }
    }
    db_merge('sfactive_legacy')
      ->key(array('type' => 'feature', 'id' => $row->feature_id))
      ->fields(array('nid' => $entity->nid))
      ->execute();
  }
}

/**
 * Defines location migration.
 */
class SFActiveLocationMigration extends SFActiveMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate event locations from the source database to taxonomy terms');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'location_id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Location ID',
        'alias' => 'location',
      ),
    ), MigrateDestinationTerm::getKeySchema());
    $query = db_select($this->sfactiveTable('location'), 'location')
      ->fields('location', array('location_id', 'name'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm('location');
    $this->addFieldMapping('name', 'name');
    $this->addUnmigratedDestinations(array('format', 'parent_name', 'description', 'weight', 'parent', 'path'));
  }
}

/**
 * Defines event type migration.
 */
class SFActiveEventTypeMigration extends SFActiveMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate event types from the source database to taxonomy terms');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'event_type_id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Event type ID',
        'alias' => 'event_type',
      ),
    ), MigrateDestinationTerm::getKeySchema());
    $query = db_select($this->sfactiveTable('event_type'), 'event_type')
      ->fields('event_type', array('event_type_id', 'name'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm('event_type');
    $this->addFieldMapping('name', 'name');
    $this->addUnmigratedDestinations(array('format', 'parent_name', 'description', 'weight', 'parent', 'path'));
  }
}

/**
 * Defines event topic migration.
 */
class SFActiveEventTopicMigration extends SFActiveMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate event topics from the source database to taxonomy terms');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'event_topic_id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Event topic ID',
        'alias' => 'event_topic',
      ),
    ), MigrateDestinationTerm::getKeySchema());
    $query = db_select($this->sfactiveTable('event_topic'), 'event_topic')
      ->fields('event_topic', array('event_topic_id', 'name'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm('event_topic');
    $this->addFieldMapping('name', 'name');
    $this->addUnmigratedDestinations(array('format', 'parent_name', 'description', 'weight', 'parent', 'path'));
  }
}

/**
 * Defines calendar event migration.
 */
class SFActiveEventMigration extends SFActiveLinkedFileMigration {
  public $blobColumns = array();
  public $entityColumns = array('location_other', 'event_topic_other', 'title', 'contact_name', 'contact_phone', 'contact_email', 'event_type_other');
  public $textFileColumn = 'description';
  public $markupColumns = array('location_details', 'description');

  public function prepareLinkedFile($row) {
    // Find the portion of the path containing /uploads/Y/m/file.
    // @todo: Update if there is still an sf-active website in 2100 :p
    if (preg_match('@uploads/([0-9]{4})/([0-9]{2})/.*@', $row->linked_file, $matches)) {
      $row->year = $matches[1];
      $row->month = $matches[2];
      return $this->sfactiveFiles($matches[0]);
    }
    watchdog('sfactive', 'Unrecognized linked file %linked_file. File not migrated.', array('%linked_file' => $row->linked_file), WATCHDOG_ERROR);
  }

  public function prepareRow($row) {
    parent::prepareRow($row);
    if ($row->start_date == '0000-00-00 00:00:00') {
      // Set a minimum start date if missing.
      $row->start_date = '2000-01-01 00:00:00';
    }
    if ($row->end_date === NULL) {
      // Set a minimum end date if missing.
      $row->end_date = $row->start_date;
    }
    $row->event_date = drupal_json_encode(array(
      'from' => $row->start_date . ' ' . $this->timezone,
      'to' => $row->end_date . ' ' . $this->timezone,
    ));
  }

  public function __construct() {
    parent::__construct();
    $this->description = 'Calendar events.';
    $this->dependencies = array('SFActiveLocation', 'SFActiveEventType', 'SFActiveEventTopic');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'event_id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Event ID',
        'alias' => 'event',
      ),
    ), MigrateDestinationNode::getKeySchema());
    $query = db_select($this->sfactiveTable('event'), 'event')
      ->fields('event', array('event_id', 'start_date', 'location_id', 'location_other', 'event_topic_id', 'event_topic_other', 'event_type_id', 'title', 'location_details', 'contact_name', 'contact_phone', 'contact_email', 'description', 'event_type_other', 'linked_file', 'artmime'));
    // Add some additional properties to define later.
    $query->addExpression('NULL', 'image');
    $query->addExpression('NULL', 'file');
    $query->addExpression('NULL', 'event_date');
    $query->addExpression('DATE_ADD(event.start_date, INTERVAL event.duration HOUR)', 'end_date');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('event');
    $this->addFieldMapping('is_new')
      ->defaultValue(TRUE);
    $this->addFieldMapping('uid')
      ->defaultValue(0);
    $this->addFieldMapping('revision_uid')
      ->defaultValue(0);
    $this->addFieldMapping('status')
      ->defaultValue(NODE_PUBLISHED);
    $this->addFieldMapping('promote')
      ->defaultValue(NODE_NOT_PROMOTED);
    $this->addFieldMapping('sticky')
      ->defaultValue(NODE_NOT_STICKY);
    $this->addFieldMapping('title', 'title');
    $arguments = MigrateTextFieldHandler::arguments(NULL, array('source_field' => 'format'));
    $this->addFieldMapping('body', 'description')
      ->arguments($arguments);
    $this->addFieldMapping('field_image', 'image');
    $this->addFieldMapping('field_file', 'file');
    $this->addFieldMapping('field_phone', 'contact_phone');
    $this->addFieldMapping('field_author', 'contact_name');
    $this->addFieldMapping('field_contact', 'contact_email');
    $this->addFieldMapping('field_event_date', 'event_date');
    $this->addFieldMapping('field_location_other', 'location_other');
    $this->addFieldMapping('field_event_topic_other', 'event_topic_other');
    $this->addFieldMapping('field_event_type_other', 'event_type_other');
    $arguments = MigrateTextFieldHandler::arguments(NULL, array('source_field' => 'format'));
    $this->addFieldMapping('field_location_details', 'location_details')
      ->arguments($arguments);
    $this->addFieldMapping('field_location', 'location_id')
      ->sourceMigration('SFActiveLocation')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_event_topic', 'event_topic_id')
      ->sourceMigration('SFActiveEventTopic')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_event_type', 'event_type_id')
      ->sourceMigration('SFActiveEventType')
      ->arguments(array('source_type' => 'tid'));
    $this->addUnmigratedDestinations(array('revision', 'path', 'comment', 'pathauto_perform_alias', 'created', 'changed', 'language'));
    $this->addUnmigratedSources(array('end_date', 'linked_file', 'artmime'));
  }

  public function complete($entity, $row) {
    parent::complete($entity, $row);
    db_merge('sfactive_legacy')
      ->key(array('type' => 'event', 'id' => $row->event_id))
      ->fields(array('nid' => $entity->nid))
      ->execute();
  }
}
