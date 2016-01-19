<?php
/**
 * @file
 * Provides a visitor context plugin for Entity data.
 */

class EntityContext extends PersonalizeContextBase {
  /**
   * Implements PersonalizeContextInterface::create().
   */
  public static function create(PersonalizeAgentInterface $agent = NULL, $selected_context = array()) {
    return new self($agent, $selected_context);
  }

  /**
   * Implements PersonalizeContextInterface::getOptions().
   */
  public static function getOptions() {
    $options = array();
    $info = entity_get_info();

    $result = db_select('entity_context', 'ec')
      ->fields('ec', array('id', 'context'))
      ->orderBy('id', 'ASC')
      ->execute();

    foreach ($result as $row) {
      $options[$row->context] = array(
        'name' => entity_context_get_nice_context_name($row->context),
        'group' => t('Entity Context'),
      );
    }

    return $options;
  }
}
