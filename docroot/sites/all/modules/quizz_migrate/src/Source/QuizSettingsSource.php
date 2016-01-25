<?php

namespace Drupal\quizz_migrate\Source;

use MigrateSource;

class QuizSettingsSource extends MigrateSource {

  private $quiz_type = 'quiz';
  private $item_number = 0;
  private $variables = array();

  public function __construct($options = array()) {
    parent::__construct($options);

    $this->variables = array(
        array('quiz_default_close', 30),
        array('quiz_use_passfail', 1),
        array('quiz_max_result_options', 5),
        array('quiz_remove_partial_quiz_record', 604800), // 7 days
        array('quiz_pager_start', 100),
        array('quiz_pager_siblings', 5),
        array('quiz_has_timer', 0),
        array('quiz_durod', 0),
    );

    foreach ($this->variables as $k => $v) {
      $this->variables[$k] = array($v[0], variable_get($v[0], $v[1]));
    }
  }

  public function fields() {
    return array(
        'bundle' => 'Quiz type (Bundle name)',
        'name'   => 'Variable name',
        'value'  => 'Varible value',
    );
  }

  public function __toString() {
    return "Settings for quiz:{$this->quiz_type}";
  }

  public function computeCount() {
    return count($this->variables);
  }

  public function performRewind() {
    $this->item_number = 0;
  }

  public function getNextRow() {
    if (!isset($this->variables[$this->item_number])) {
      return NULL;
    }

    $var = $this->variables[$this->item_number++];
    return (object) array(
          'migration' => 'quiz_type',
          'bundle'    => $this->quiz_type,
          'name'      => $var[0],
          'value'     => $var[1]
    );
  }

}
