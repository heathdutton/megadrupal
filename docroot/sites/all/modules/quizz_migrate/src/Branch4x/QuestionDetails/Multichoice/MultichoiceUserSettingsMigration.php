<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails\Multichoice;

use MigrateDestinationTable;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;

class MultichoiceUserSettingsMigration extends Migration {

  protected $source_table = 'quiz_multichoice_user_settings';
  protected $dest_table = 'quizz_multichoice_user_settings';

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();

    $pk_dest = MigrateDestinationTable::getKeySchema($this->dest_table);
    $pk_source = array(
        'uid' => array('alias' => 'question_extra') + $pk_dest['uid'],
    );

    $this->map = new MigrateSQLMap($this->machineName, $pk_source, $pk_dest);
    $this->setupFieldMapping();
  }

  protected function setupMigrateDestination() {
    return new MigrateDestinationTable($this->dest_table);
  }

  protected function setupMigrateSource() {
    $query = db_select($this->source_table, 'question_extra');
    $query
      ->fields('question_extra', array('uid', 'choice_multi', 'choice_random', 'choice_boolean'))
      ->orderBy('question_extra.uid')
    ;
    return new MigrateSourceSQL($query);
  }

  protected function setupFieldMapping() {
    $this->addSimpleMappings(array('uid', 'choice_multi', 'choice_random', 'choice_boolean'));
  }

}
