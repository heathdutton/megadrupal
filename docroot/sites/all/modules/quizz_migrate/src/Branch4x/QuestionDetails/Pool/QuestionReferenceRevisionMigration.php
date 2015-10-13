<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails\Pool;

class QuestionReferenceRevisionMigration extends QuestionReferenceMigration {

  protected $source_table = 'field_revision_field_question_reference';
  protected $dest_table = 'field_revision_field_question_reference';

}
