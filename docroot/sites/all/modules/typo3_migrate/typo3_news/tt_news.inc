<?php
/**
 * @file
 * MIgration script for tt_news.
 */

/**
 * Migrate all the tt news category in a drupal taxonomy(NewsCatgeory).
 */
class Typo3NewsCategoryMigration extends BaseMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate News Category Taxonomy');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uid' => array('type' => 'int',
          'length' => 11,
          'not null' => TRUE,
          'description' => 'Category',
        )
      ),
        MigrateDestinationTerm::getKeySchema()
    );

    // initiate connection to the typo3 database and select the table...
    $query = db_select(TYPO3_DATABASE_NAME . '.tt_news_cat', 'cat')
      ->fields('cat', array('uid', 'title', 'parent_category'))
      ->orderBy('tstamp', 'ASC');
    $query->condition('cat.deleted', '0');
    if ($this->queryParams['newsCat']['hidden'] != '') {
      $query->condition('cat.hidden', $this->queryParams['newsCat']['hidden'], $this->queryParams['newsCat']['hidden_operator']);
    }

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($query, array());

    // Set up our destination - terms in the News Category vocabulary
    $this->destination = new MigrateDestinationTerm('news_category');

    $this->addFieldMapping('name', 'title');
    $this->addFieldMapping('description', 'description');

    $this->addFieldMapping('parent', 'parent_category')
          ->sourceMigration('Typo3NewsCategory')
          ->defaultValue(0);

    $this->addFieldMapping('format')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('weight')
          ->issueGroup(t('DNM'));
  }
}


/**
 * Migrate tt news from Typo3 to News content type.
 */

class Typo3NewsMigration extends BaseMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('tt_news migration.');
    // NewsCategory migration needs to run first.
    $this->dependencies = array('Typo3NewsCategory');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'description' => 'tt_news UID',
          'alias' => 'tn',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select(TYPO3_DATABASE_NAME . '.tt_news', 'tn')
              ->fields('tn', $this->queryParams['news']['fields'])
              ->fields('catmm', array('sorting', 'uid_foreign'));

    $query->condition('tn.deleted', '0');
    if ($this->queryParams['news']['pid'] != 'ignore') {
      $query->condition('tn.pid' , $this->queryParams['news']['pid'] , $this->queryParams['news']['pid_operator']);
    }

    $query->leftJoin( TYPO3_DATABASE_NAME . '.tt_news_cat_mm', 'catmm', 'catmm.uid_local = tn.uid');
    $query->leftJoin( TYPO3_DATABASE_NAME . '.tt_news_cat', 'newscat', 'newscat.uid = catmm.uid_foreign');
    // Related news articles
    $query->leftJoin(TYPO3_DATABASE_NAME . '.tt_news_related_mm', 'relatedmm', 'relatedmm.uid_local = tn.uid');
    $query->groupBy('tn.uid');

    $query->addExpression('GROUP_CONCAT(newscat.title)', 'newstags');

    $query->addExpression('GROUP_CONCAT(catmm.uid_foreign)', 'cat_list');
    $query->addExpression('GROUP_CONCAT(distinct relatedmm.uid_foreign)', 'related_list');

    $this->highwaterField = array(
      'name' => 'tstamp', // Column to be used as highwater mark
      'alias' => 'tn',
      'type' => 'int',
    );
    // Note that it is important to process rows in the order of the highwater mark
    $query->orderBy('tn.' . $this->queryParams['news']['orderField']);

    $this->source = new MigrateSourceSQL($query, array());

    // Set up our destination - nodes of type news
    $this->destination = new MigrateDestinationNode('news');

    // Mapped fields
    $this->addFieldMapping('title', 'title')
          ->description(t('title => title'));
    $this->addFieldMapping('field_news_sub_header', 'short')
          ->description(t('short => field_news_sub_header'));
    $this->addFieldMapping('field_news_author', 'author');
    $this->addFieldMapping('field_news_author_email', 'author_email');

    // Copy an image file, write DB record to files table, and save in Field storage.
    // We map the filename (relative to the source_dir below) to the field itself.
    $this->addFieldMapping('field_news_images', 'image')->separator(',');
    // Here we specify the directory containing the source files.
    $this->addFieldMapping('field_news_images:source_dir')
          ->defaultValue(drupal_get_path('module', 'typo3_news') . '/pics');
    // And we map the alt and title values in the database to those on the image.
    $this->addFieldMapping('field_news_images:alt', 'imagealttext')->separator("\n");
    $this->addFieldMapping('field_news_images:title', 'imagetitletext')->separator("\n");
    // Copy an file, write DB record to files table, and save in Field storage.
    // We map the filename (relative to the source_dir below) to the field itself.
    $this->addFieldMapping('field_news_files', 'news_files')->separator(',');
    // Here we specify the directory containing the source files.
    $this->addFieldMapping('field_news_files:source_dir')
          ->defaultValue(drupal_get_path('module', 'typo3_news') . '/files');
    // And we map the alt and title values in the database to those on the image.
    $this->addFieldMapping('field_news_files:alt', '');
    $this->addFieldMapping('field_news_files:title', '');


    // In case of Taxonomy, no need to pass the Source Migration.
    // Only pass title to the taxonmoy
    $this->addFieldMapping('News Category', 'newstags')
        ->separator(',');

    if ($this->queryParams['news']['related_article_field'] != '') {
      $this->addFieldMapping('field_news_related_articles', 'related_list')
          ->sourceMigration('Typo3News')
          ->separator(',');
    }

    // Get userid from be_users
    $this->addFieldMapping('uid', 'cruser_id')
          ->sourceMigration('Typo3BeUser')
          ->defaultValue(1);

    $arguments = MigrateTextFieldHandler::arguments(array('source_field' => 'excerpt'));
    $this->addFieldMapping('body', 'bodytext');
    $this->addFieldMapping('created', 'crdate');
    $this->addFieldMapping('changed', 'tstamp');

    // Pass the default language
    if ($this->queryParams['news']['default_language']) {
      $this->addFieldMapping('language')
          ->defaultValue($this->queryParams['news']['default_language']);
    }
    // Unmapped destination fields
    $this->addFieldMapping('status')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('promote')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('revision')
          ->issueGroup(t('DNM'));
  }

  /**
   *
   * Controll data  and process according to need.
   *  @param stdClass $node
   *  @param stdClass $row
   */
  public function prepare($node, stdClass $row) {
    $node->status  = ($row->hidden) ? '0' : '1';
    $node->teaser = ''; // Not passing data in teaser
    // Replace the typo3 link-tag.
    $node->body[$node->language][0]['value'] = $this->typo3_handle_link_breakage($node->body[$node->language][0]['value']);
    $node->body[$node->language][0]['value'] = $this->typo3_handle_missing_mailto($node->body[$node->language][0]['value']);
    $node->title = str_replace(array('<br>', '<br />', '<br/>'), ' ', $node->title);
    $node->title = strip_tags($node->title);
  }
}

