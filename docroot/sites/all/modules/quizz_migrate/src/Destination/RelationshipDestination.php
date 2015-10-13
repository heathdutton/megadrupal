<?php

namespace Drupal\quizz_migrate\Destination;

use Drupal\quizz_migrate\BaseEntityDestination;

class RelationshipDestination extends BaseEntityDestination {

  protected $entity_type = 'quiz_relationship';
  protected $base_table = 'quiz_relationship';
  protected static $pk_name = 'qr_id';

}
