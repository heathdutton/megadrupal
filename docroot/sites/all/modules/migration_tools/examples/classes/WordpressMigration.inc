<?php

/**
 * @file
 * Contains classes for migrations from Wordpress.
 */

define('MT_LEGACY_WORDPRESS_DIR', DRUPAL_ROOT . '/../../legacy/wordpress_exports');

/**
 * Implementation of WordPressMigration, for blog entries.
 *
 * @package migration_tools
 */
abstract class MTWordpressBlogEntry extends WordPressBlogEntry {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['post_type'] = 'blog_entry';
    $arguments['path_action'] = 2;
    $arguments['generate_redirects'] = 1;
    $arguments['tag_vocabulary'] = 'tags';
    $arguments['category_vocabulary'] = 'blog_categories';
    $arguments['text_format'] = 'wysiwyg';
    $arguments['podcast_field'] = '';
    $arguments['attachment_field'] = 'blog_attachments';

    parent::__construct($arguments);

    $this->addFieldMapping('og_group_ref')->defaultValue($arguments['group_name'])->sourceMigration('Organization');
    $this->addFieldMapping('field_blog_body', 'content');
    $this->addFieldMapping('field_blog_body:format')->defaultValue($arguments['text_format']);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      if ($row->post_type == $this->postType) {
        if (isset($row->status) && $row->status == 'trash') {
          $this->queueMessage(t('Item has status "trash." Not migrating.'));
        }
      }
      return FALSE;
    }

    if (trim($row->content) == '') {
      $row->status = NODE_UNPUBLISHED;
    }
    else {
      $row->status = NODE_PUBLISHED;
    }

    // Trim title.
    if (strlen($row->title) > 255) {
      $row->title = substr(trim($row->title), 0, 255);
      $this->queueMessage(t('Title was too long. It has been truncated to @title.', array('@title' => $row->title)));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($node, $row) {
    parent::prepare($node, $row);

    migration_tools_prepare_workbench_settings($node, $row);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    if (isset($entity->nid) && $entity->nid) {
      migration_tools_complete_workbench_settings($entity, $row);
    }
  }
}

/**
 * Implementation of WordPressMigration, for court decisions.
 *
 * @package migration_tools
 */
abstract class MTWordpressCourtDecision extends WordPressBlogEntry {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['post_type'] = 'court_decision';
    $arguments['path_action'] = 2;
    $arguments['generate_redirects'] = 1;
    $arguments['tag_vocabulary'] = 'tags';
    $arguments['category_vocabulary'] = 'court_decision_topic';
    $arguments['text_format'] = 'wysiwyg';
    $arguments['podcast_field'] = '';
    $arguments['attachment_field'] = 'cd_attachments';

    parent::__construct($arguments);

    $this->addFieldMapping('og_group_ref')->defaultValue($arguments['group_name'])->sourceMigration('Organization');
    $this->addFieldMapping('field_cd_body', 'content');
    $this->addFieldMapping('field_cd_body:format')->defaultValue($arguments['text_format']);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      if ($row->post_type == $this->postType) {
        if (isset($row->status) && $row->status == 'trash') {
          $this->queueMessage(t('Item has status "trash." Not migrating.'));
        }
      }
      return FALSE;
    }

    if (trim($row->content) == '') {
      $row->status = NODE_UNPUBLISHED;
    }
    else {
      $row->status = NODE_PUBLISHED;
    }

    // Trim title.
    if (strlen($row->title) > 255) {
      $row->title = substr(trim($row->title), 0, 255);
      $this->queueMessage(t('Title was too long. It has been truncated to @title.', array('@title' => $row->title)));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($node, $row) {
    parent::prepare($node, $row);
    migration_tools_prepare_workbench_settings($node, $row);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    if (isset($entity->nid) && $entity->nid) {
      migration_tools_complete_workbench_settings($entity, $row);
    }
  }
}

/**
 * Class MTWordpressAuthor.
 */
abstract class MTWordpressAuthor extends WordPressAuthor {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['import_users'] = 0;
    $arguments['default_author_uid'] = 1;

    parent::__construct($arguments);
  }
}

/**
 * Class MTWordpressAttachment.
 */
abstract class MTWordpressAttachment extends WordPressAttachment {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

  }
}

/**
 * Class MTWordpressAttachment.
 */
abstract class MTWordpressCategory extends WordPressCategory {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    if (!isset($arguments['category_vocabulary'])) {
      $arguments['category_vocabulary'] = "topic";
    }
    parent::__construct($arguments);
  }
}

/**
 * Copies and preprocesses raw Wordpress XML file to wordpress:// directory.
 *
 * @param string $filename
 *   The filename of the Wordpress source XML file.
 *
 * @return string
 *   The filepath of the preprocessed file.
 *
 * @throws Exception
 */
function mt_migrate_wordpress_prepare_file($filename) {
  $source_directory = MT_LEGACY_WORDPRESS_DIR;
  $source_filepath = $source_directory . '/' . $filename;
  $dest_directory = 'wordpress://';
  $dest_filepath = $dest_directory . str_replace(' ', '%20', $filename);

  // Preprocess source file and store in sites/default/private/wordpress.
  // The destination directory must exist!
  if (file_exists($source_filepath) && !file_exists($dest_filepath)) {
    if (!variable_get('file_private_path')) {
      throw new Exception('You must specify a private file system path at admin/config/media/file-system');
    }
    WordPressBlog::preprocessFile($source_filepath, $dest_filepath, FALSE);
  }

  // Throw error if the preprocessed file does not exist.
  if (file_exists($dest_filepath)) {
    $arguments['filename'] = $dest_filepath;
  }
  else {
    throw new Exception("Could not find Wordpress source XML file $dest_filepath.");
  }

  return $dest_filepath;
}
