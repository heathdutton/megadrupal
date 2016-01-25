<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Branch4x\AnswerDetails\ClozeDetails;
use Drupal\quizz_migrate\Branch4x\AnswerDetails\DdlinesDetails;
use Drupal\quizz_migrate\Branch4x\AnswerDetails\LongTextDetails;
use Drupal\quizz_migrate\Branch4x\AnswerDetails\MatchingDetails;
use Drupal\quizz_migrate\Branch4x\AnswerDetails\MultichoiceDetails;
use Drupal\quizz_migrate\Branch4x\AnswerDetails\ScaleDetails;
use Drupal\quizz_migrate\Branch4x\AnswerDetails\ShortTextDetails;
use Drupal\quizz_migrate\Branch4x\AnswerDetails\TrueFalseDetails;
use Drupal\quizz_migrate\Branch4x\QuestionDetails\DetailsInterface;
use Migration;
use stdClass;

class AnswerDetailMigration extends Migration {

  /** @var string */
  protected $bundle;

  /** @var DetailsInterface */
  private $details_handler;

  public function __construct($arguments = array()) {
    $this->bundle = $arguments['bundle'];
    $this->machineName = "quiz_answer_details__{$this->bundle}";

    if ($handler = $this->getDetailsHandler()) {
      $this->source = $handler->setupMigrateSource();
      $this->destination = $handler->setupMigrateDestination();
      $this->map = $handler->setupMigrateMap();
      $handler->setupMigrateFieldMapping();
    }

    parent::__construct($arguments);
  }

  public function getMachineName() {
    return $this->machineName;
  }

  public function getDetailsHandler() {
    if (NULL === $this->details_handler) {
      switch ($this->bundle) {
        case 'cloze':
          $this->details_handler = new ClozeDetails($this->bundle, $this);
          break;
        case 'quiz_ddlines':
          $this->details_handler = new DdlinesDetails($this->bundle, $this);
          break;
        case 'long_answer':
          $this->details_handler = new LongTextDetails($this->bundle, $this);
          break;
        case 'matching':
          $this->details_handler = new MatchingDetails($this->bundle, $this);
          break;
        case 'truefalse':
          $this->details_handler = new TrueFalseDetails($this->bundle, $this);
          break;
        case 'short_answer':
          $this->details_handler = new ShortTextDetails($this->bundle, $this);
          break;
        case 'multichoice':
          $this->details_handler = new MultichoiceDetails($this->bundle, $this);
          break;
        case 'scale':
          $this->details_handler = new ScaleDetails($this->bundle, $this);
          $this->dependencies[] = 'quiz_scale_collection';
          $this->dependencies[] = 'quiz_scale_collection_item';
          break;
        case 'pool':
        case 'quiz_directions':
        default:
          break;
      }
    }
    return $this->details_handler;
  }

  public function prepare($entity, $row) {
    return $this->getDetailsHandler()->prepare($entity, $row);
  }

  public function complete($entity, stdClass $row) {
    return $this->getDetailsHandler()->complete($entity, $row);
  }

}
