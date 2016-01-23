<?php

/**
 * @file
 * Entity Lister Example pages.
 */

/**
 * Demo page.
 */
function entity_lister_example_table_page() {

  $array['#attached']['css'][] = array(
    'data' => drupal_get_path('module', 'entity_lister_example') . '/filter.css',
  );

  $filter_form = drupal_get_form('entity_lister_example_filter_form');
  $array['filter-form'] = array(
    '#markup' => drupal_render($filter_form),
  );

  // The type of entity to list.
  $config['type'] = 'user';

  // We don't really have to set a bundle in this case, since there is only one
  // user bundle, but this is how it's done...
  $config['bundle'] = array('user');

  // '0' for no pager, '1' for standard pager, '2' for ajax pager.
  $config['pager'] = '2';

  $config['numitems'] = '20';
  $config['cache'] = '0';

  // '0' for above, '1' for below, '2' for both.
  $config['pager_position'] = '2';

  // See entity_lister_preprocess_entity_lister_list().
  $config['template'] = 'tabular';

  // See entity_lister_example_preprocess_user_profile().
  $config['view_mode'] = 'entity_lister_example_table';

  $table_sort = array(
    'uid' => array(
      'data' => t('User ID'),
      'type' => 'property',
      'specifier' => 'uid',
      'sort' => 'desc',
    ),
    'username' => array(
      'data' => t('Username'),
      'type' => 'property',
      'specifier' => 'name',
    ),
    'email' => array(
      'data' => t('Email'),
      'type' => 'property',
      'specifier' => 'mail',
    ),
    // How to sort on a field.
    /*
    'something' => array(
      'data' => 'Something',
      'type' => 'field',
      'specifier' => array(
        'field' => 'field_something',
        'column' => 'some_column',
      ),
    ),
    */
  );

  // The array to pass to EntityFieldQuery::tableSort().
  $config['table_sort'] = $table_sort;

  $tvars['dir'] = 'desc';
  // The keys in $tvars['headers'] must match those in $table_sort for any
  // headers where 'link' is TRUE.
  $tvars['headers']['uid'] = array('label' => t('User ID'), 'sort' => TRUE);
  $tvars['headers']['username'] = array('label' => t('Username'), 'sort' => TRUE);
  $tvars['headers']['email'] = array('label' => t('Email'), 'sort' => TRUE);
  $tvars['filter_form'] = $filter_form;
  $config['headers'] = theme('entity_lister_headers', $tvars);

  // Email filter.
  if (isset($_GET['mail'])) {
    $config['property_condition'][] = array(
      'column' => 'mail',
      'value' => $_GET['mail'],
      'operator' => 'CONTAINS',
    );
  }

  // The $delta must be unique among all lists appearing
  // on this page, and must be an integer.
  $delta = 0;

  // All params are optional, but in this case we set $config and $delta.
  $obj = new EntityLister($config, $delta);

  // Passing the $page to getList() is only required when using the standard
  // pager. Since this list uses the AJAX pager, we could omit the
  // call, but it's here for demo purposes.
  $page = $obj->getPageNum();

  // Get the themed list output.
  $list = $obj->getList($page);

  // Place the list in a renderable element.
  $array['entity-lister-' . $delta] = $obj->element($list);

  // Add the js and css for a tabular list.
  entity_lister_table_sort($array, $table_sort);

  return $array;

}
