<?php

/**
 * Implements hook_rules_event_info().
 *
 * @todo We need a useful way of declaring what variables will be passed to the rule.
 */
function rules_linkevent_rules_event_info() {
  $events = array();

  foreach (rules_linkevent_get_links() as $link) {
    $events['rules_linkevent_' . $link->name] = array(
      'label' => sprintf('%s was accessed', $link->label),
      'group' => t('Links'),
      'variables' => array(
        'path' => array(
          'type' => 'array',
          'label' => t('Path accessed'),
          'description' => t('The path which has just been accessed.'),
        ),
      ),
    );
  }

  return $events;
}
