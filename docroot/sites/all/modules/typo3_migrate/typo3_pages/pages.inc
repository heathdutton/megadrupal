<?php
/**
 * @file
 * Set up for the pages migration.
 */

/**
 *
 * Migrate Pages from Typo3.
 * Read all the corresponding tt_content elements and merge them into a single page
 *
 */
class Typo3PagesMigration extends BaseMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Pages and tt_content migration.');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'description' => 'pages UID',
          'alias' => 'pg',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select(TYPO3_DATABASE_NAME . '.pages', 'pg')
              ->fields('pg', array('uid' , 'pid', 'sorting', 'crdate', 'cruser_id', 'title', 'doktype', 'hidden', 'tstamp'));

    // Place all the conditions to refine the source data
    $query->condition('pg.deleted', '0');
    $query->condition('pg.pid', '-1', '<>');
    $query->condition('pg.hidden', $this->queryParams['pages']['hidden'], $this->queryParams['pages']['hidden_operator']);

    $query->condition(db_or()->condition('pg.endtime', 0)->condition('pg.endtime', time(), '>'));

    if ($this->queryParams['pages']['dokType']) {
      $query->condition('pg.doktype', $this->queryParams['pages']['dokType'], $this->queryParams['pages']['dokType_operator']);
    }

    // Pickup all the pages content element from tt_content
    $query->leftJoin( TYPO3_DATABASE_NAME . '.tt_content', 'tt', 'tt.pid = pg.uid');
    $query->condition('tt.deleted', '0');
    $query->condition('tt.hidden', '0');

    $query->condition(db_or()->condition('tt.endtime', 0)->condition('tt.endtime', time(), '>'));
    //do not import the custom php pages - they should be handled manually

    if ($this->queryParams['pages']['cType']) {
      $query->condition('tt.CType' , $this->queryParams['pages']['cType'] , $this->queryParams['pages']['cType_operator']);
    }

    $query->groupBy('pg.uid');

    if ($this->queryParams['pages']['expBody']) {
      $query->addExpression($this->queryParams['pages']['expBody'], 'sourcebody');
    }

    $this->highwaterField = array(
      'name' => 'tstamp', // Column to be used as highwater mark
      'alias' => 'pg',
      'type' => 'int',
    );
    // Note that it is important to process rows in the order of the highwater mark
    $query->orderBy('pg.' . $this->queryParams['pages']['orderField']);

    $this->source = new MigrateSourceSQL($query, array());

    // Set up our destination - nodes of type typo3 pages
    $this->destination = new MigrateDestinationNode($this->queryParams['pages']['destination_type']);

    // Mapped fields
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('status', 'hidden');
    if ($this->queryParams['pages']['path_perform_alias'] != '') {
      $this->addFieldMapping('pathauto_perform_alias')
            ->defaultValue($this->queryParams['pages']['path_perform_alias']);
    }
    // Get userid from be_users
    $this->addFieldMapping('uid', 'cruser_id')
          ->sourceMigration('Typo3BeUser')
          ->defaultValue(1);
    $arguments = MigrateTextFieldHandler::arguments(array('source_field' => 'excerpt'));
    $this->addFieldMapping('body', 'sourcebody')
          ->defaultValue('');

    $this->addFieldMapping('created', 'crdate');

    // Unmapped destination fields
    $this->addFieldMapping('name')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('promote')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('revision')
          ->issueGroup(t('DNM'));

  }

  /**
   *
   * Process information here before migration.
   * @param stdClass $node
   * @param stdClass $row
   */
  public function prepare($node, stdClass $row) {
    $node->status  = ($row->hidden) ? '0' : '1';
    // Remove teaser part
    if (!($this->queryParams['pages']['teaser'])) {
      $node->teaser = '';
    }
    $node->body[$node->language][0]['value'] = $this->typo3_handle_link_breakage($node->body[$node->language][0]['value']);
    $node->body[$node->language][0]['value'] = $this->typo3_handle_missing_mailto($node->body[$node->language][0]['value']);
  }

}

