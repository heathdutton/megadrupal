<?php

namespace Drupal\quizz_pool;

use Drupal\quizz_question\Entity\QuestionType;

class QuestionTypeInstaller {

  private $field_name = 'field_question_reference';

  public function setup(QuestionType $question_type) {
    $this->doCreateField();
    $this->doCreateFieldInstance($question_type);

    // Override default weight to make body field appear first
    if ($instance = field_read_instance('quiz_question_entity', 'quiz_question_body', $question_type->type)) {
      $instance['widget']['weight'] = -10;
      $instance['widget']['settings']['rows'] = 6;
      field_update_instance($instance);
    }
  }

  public function addNewTarget(QuestionType $question_type) {
    if ($field = field_info_field($this->field_name)) {
      if (empty($field['settings']['handler_settings']['target_bundles'])) {
        $field['settings']['handler_settings']['target_bundles'] = $this->getTargetBundles();
      }
      $field['settings']['handler_settings']['target_bundles'][$question_type->type] = $question_type->type;

      // Ensure question handlers
      foreach (array_keys($field['settings']['handler_settings']['target_bundles']) as $type) {
        if (!$_question_type = quizz_question_type_load($type)) {
          unset($field['settings']['handler_settings']['target_bundles'][$type]);
        }
        elseif (in_array($_question_type->handler, array('multichoice', 'truefalse', 'matching'))) {
          unset($field['settings']['handler_settings']['target_bundles'][$type]);
        }
      }

      field_update_field($field);
    }
  }

  private function doCreateField() {
    if (!field_info_field($this->field_name)) {
      field_create_field(array(
          'active'       => 1,
          'cardinality'  => -1,
          'deleted'      => 0,
          'entity_types' => array(),
          'field_name'   => $this->field_name,
          'foreign keys' => array(
              'quiz_question_entity' => array(
                  'table'   => 'quiz_question_entity',
                  'columns' => array('target_id' => 'qid')
              )
          ),
          'indexes'      => array('target_id' => array(0 => 'target_id')),
          'locked'       => 0,
          'module'       => 'entityreference',
          'translatable' => 0,
          'type'         => 'entityreference',
          'settings'     => array(
              'target_type'      => 'quiz_question_entity',
              'handler'          => 'base',
              'handler_settings' => array(
                  'target_bundles' => $this->getTargetBundles(),
                  'sort'           => array('type' => 'property', 'property' => 'title', 'direction' => 'ASC'),
                  'behaviors'      => array(),
              ),
          ),
      ));
    }
  }

  protected function getTargetBundles() {
    $targets = array();
    foreach (quizz_question_get_types() as $name => $question_type) {
      if (in_array($question_type->handler, array('multichoice', 'truefalse', 'matching'))) {
        $targets[] = $name;
      }
    }
    return $targets;
  }

  private function doCreateFieldInstance(QuestionType $question_type) {
    if (!field_info_instance('quiz_question_entity', $this->field_name, $question_type->type)) {
      field_create_instance(array(
          'field_name'  => $this->field_name,
          'entity_type' => 'quiz_question_entity',
          'bundle'      => $question_type->type,
          'label'       => 'Question reference',
          'description' => 'Question that this pool contains',
          'required'    => TRUE,
          'widget'      => array(
              'type'     => 'entityreference_autocomplete',
              'module'   => 'entityreference',
              'active'   => 1,
              'settings' => array(
                  'match_operator' => 'CONTAINS',
                  'size'           => 60,
                  'path'           => '',
              )
          ),
          'settings'    => array(),
      ));
    }
  }

}
