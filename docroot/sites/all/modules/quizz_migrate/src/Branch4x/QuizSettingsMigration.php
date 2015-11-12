<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Destination\QuizSettingsDestination;
use Drupal\quizz_migrate\Source\QuizSettingsSource;
use MigrateSQLMap;
use Migration;

class QuizSettingsMigration extends Migration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments);

    $this->source = new QuizSettingsSource();
    $this->destination = new QuizSettingsDestination();

    $source_key = $destination_key = QuizSettingsDestination::getKeySchema();

    $this->map = new MigrateSQLMap('quiz_settings', $source_key, $destination_key);
    $this->addSimpleMappings(array('bundle', 'name', 'value'));
  }

}
