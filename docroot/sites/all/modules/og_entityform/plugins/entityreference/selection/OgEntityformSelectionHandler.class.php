<?php


/**
 * @file
 * OG entityform selection handler.
 */

class OgEntityformSelectionHandler extends OgSelectionHandler {

  /**
   * Overrides OgSelectionHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new OgEntityformSelectionHandler($field, $instance, $entity_type, $entity);
  }

  /**
   * Override EntityReferenceHandler::settingsForm().
   */
  public static function settingsForm($field, $instance) {
    return parent::settingsForm($field, $instance);
  }


  /**
   * Overrides OgSelectionHandler::buildEntityFieldQuery().
   */
  public function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $group_type = $this->field['settings']['target_type'];
    $entity_info = entity_get_info($group_type);
    $query = parent::buildEntityFieldQuery($match, $match_operator);
    $context = og_context($group_type);
    if (!empty($context) && og_user_access($this->field['settings']['target_type'], $context['gid'], 'view og entityforms')) {
      // If I have "view og entityforms" permission, ensure that the group is on the list of options.
      $exists = FALSE;
      foreach ($query->propertyConditions as $i => $condition) {
        if ($condition['column'] == $entity_info['entity keys']['id'] && $condition['operator'] == 'IN') {
          if (!in_array($context['gid'], $condition['value'])) {
            $query->propertyConditions[$i]['value'][] = $context['gid'];
          }
          $exists = TRUE;
        }
      }
      if (!$exists) {
        // I don't have any groups at all. Need to remove the nid=-1 condition
        // and add the specific group that I'm posting to.
        foreach ($query->propertyConditions as $i => $condition) {
          if ($condition['column'] == $entity_info['entity keys']['id'] && $condition['operator'] == '=' && $condition['value'] == -1) {
            unset($query->propertyConditions[$i]);
          }
        }
        $query->propertyCondition('nid', array($context['gid']), 'IN');
      }
    }

    return $query;
  }

  public function entityFieldQueryAlter(SelectQueryInterface $query) {
    parent::entityFieldQueryAlter($query);
  }

}
