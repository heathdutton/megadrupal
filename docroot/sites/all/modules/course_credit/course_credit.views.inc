<?php

module_load_include('inc', 'views', 'handlers/views_handler_relationship');
include_once 'includes/views_join_course_credit.inc';

/**
 * Implements hook_views_handlers() to register all of the basic handlers().
 * views uses.
 */
function course_credit_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'course_credit') . '/includes',
    ),
    'handlers' => array(
      'views_handler_field_course_credit_title' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_filter_course_credit_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'views_handler_field_course_credit_node_active_types' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      'course_credit_credit_awarded_credit_instance_vhr' => array(
        'parent' => 'views_handler_relationship',
      ),
      'course_credit_course_report_credit_awarded_vhr' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}

/**
 * Relate node to awarded credit.
 *
 * Join node.nid to awarded.nid, USER awarded.uid
 *
 * This is for course relationships mostly.
 */
class course_credit_node_awarded_credit_vhr extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    global $user;
    //detect the user from the path to the report (instead of deafualt global $user)
    if ($this->view->argument['uid']) {
      // TODO Convert "user_load" to "user_load_multiple" if "$this->view->args[0]" is other than a uid.
      // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
      // Example: array_shift(user_load_multiple(array(), $this->view->args[0]))
      $account = user_load($this->view->args[0]);
    }
    else {
      $account = $user;
    }
    // Figure out what base table this relationship brings to the party.
    $join = new views_join();
    $join->definition = array(
      'table' => 'course_credit_awarded',
      'field' => 'nid',
      'left_table' => 'node',
      'left_field' => 'nid',
      'extra' => array(
        array(
          'field' => 'uid',
          'numeric' => TRUE,
          'value' => $account->uid,
        ),
      ),
    );

    $join->construct();
    $this->ensure_my_table();

    $alias = $join->definition['table'] . '_' . $join->definition['left_table'];
    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }

}

/**
 * Implements hook_views_data_alter().
 */
function course_credit_views_data_alter(&$data) {
  $data['course_report']['credit_awarded']['relationship'] = array(
    'handler' => 'course_credit_course_report_credit_awarded_vhr',
    'label' => t('Credit awarded'),
    'title' => t('Credit awarded'),
    'help' => t('Credit awarded for this record'),
    'base' => 'course_credit_awarded',
  );

  $data['course_credit_awarded']['ctid_global']['relationship'] = array(
    'relationship field' => 'type',
    'base' => 'course_credit_type',
    'field' => 'type',
    'base field' => 'type',
    'handler' => 'views_handler_relationship',
    'label' => t('Credit type'),
    'title' => t('Credit type'),
    'help' => t('Relate to global credit type'),
  );

  $data['course_credit_awarded']['ctid_instance']['relationship'] = array(
    'handler' => 'course_credit_credit_awarded_credit_instance_vhr',
    'label' => t('Credit instance'),
    'title' => t('Credit instance'),
    'help' => t('Relate awarded credit to per-course credit instance'),
    'base' => 'course_credit',
  );

  $data['course_credit']['ctid']['relationship'] = array(
    'relationship field' => 'type',
    'base' => 'course_credit_type',
    'base field' => 'type',
    'field' => 'type',
    'handler' => 'views_handler_relationship',
    'label' => t('Default credit type'),
    'title' => t('Default credit type'),
    'help' => t('Relate to global credit type.'),
  );

  $data['node']['ctid_instance']['relationship'] = array(
    'relationship field' => 'nid',
    'base' => 'course_credit',
    'field' => 'nid',
    'base field' => 'nid',
    'handler' => 'views_handler_relationship',
    'label' => t('Credit instance'),
    'title' => t('Credit instance'),
    'help' => t('Relate node to credit instances.'),
  );

  $data['node']['credit_awarded']['relationship'] = array(
    'handler' => 'course_credit_node_awarded_credit_vhr',
    'label' => t('Credit awarded'),
    'title' => t('Credit awarded'),
    'help' => t("Relate node to user's awarded credit"),
    'base' => 'course_credit_awarded',
  );

  $data['node']['course_credit_types'] = array(
    'title' => 'Active credit types',
    'help' => 'The credit types active on this node.',
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_field_course_credit_node_active_types',
    ),
  );

  $data['course_credit_type']['title']['field']['handler'] = 'views_handler_field_course_credit_title';
  $data['course_credit']['type']['filter']['handler'] = 'views_handler_filter_course_credit_type';

  return $data;
}
