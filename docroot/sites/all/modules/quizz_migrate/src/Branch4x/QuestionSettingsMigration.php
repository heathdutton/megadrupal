<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Destination\QuestionSettingsDestination;
use Drupal\quizz_migrate\Source\QuestionSettingsSource;
use MigrateSQLMap;
use Migration;

class QuestionSettingsMigration extends Migration {

  private $question_type;

  public function __construct($arguments = array()) {
    parent::__construct($arguments);

    $this->question_type = $arguments['bundle'];

    $this->source = new QuestionSettingsSource(array('question_type' => $this->question_type));
    $this->destination = new QuestionSettingsDestination();

    $source_key = $destination_key = QuestionSettingsDestination::getKeySchema();

    $this->map = new MigrateSQLMap("quiz_question_settings__{$this->question_type}", $source_key, $destination_key);
    $this->addSimpleMappings(array('bundle', 'name', 'value'));
  }

}
