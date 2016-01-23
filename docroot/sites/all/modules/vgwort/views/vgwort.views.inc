<?php

/**
 * Implementation of hook_views_data().
 */
function vgwort_views_data() {
  $data['vgwort'] = array(
    'table' => array(
      'base' => array(
        'field' => 'mid',
        'title' => 'Vgwort',
        'help' => 'Hold vgwort marks.',
      ),
      'group' => 'Vgwort',
    ),
    'mid' => array(
      'title' => 'Mid',
      'help' => 'The primary identifier for a pixel.',
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
    'domain' => array(
      'title' => 'Domain',
      'help' => 'Domain to call.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
    ),
    'private' => array(
      'title' => 'Private',
      'help' => 'private id.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
    ),
    'public' => array(
      'title' => 'Public',
      'help' => 'public id.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
    ),
    'entity_id' => array(
      'title' => 'Entity Id',
      'help' => 'public id.',
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
    'entity_type' => array(
      'title' => 'Entity Type',
      'help' => 'entity type.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
    ),
    'status' => array(
      'title' => 'Status',
      'help' => 'Status of the mark',
      'field' => array(
        'handler' => 'vgwort_handler_field_status',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'vgwort_handler_filter_status'
      ),
      'sort' => array(
        'handler' => 'vgwort_handler_sort'
      )
    ),
    'modified' => array(
      'title' => 'Modified',
      'help' => 'Last status change.',
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      )
    ),
    'test' => array(
      'title' => 'Test',
      'help' => 'Is test mark',
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator'
      )
    ),
  );

  $entity_info = entity_get_info();

   foreach ($entity_info as $entity_type => $info) {
     $data['vgwort']['vgwort_related_' . $entity_type] = array(
         'group' => t('VGWort'),
         'title' => t('VGWort to @entity',  array('@entity' => drupal_ucfirst($info['label']))),
         'help' => t('The @entity entity that is associated with the .', array('@entity' => $info['label'])),
         'relationship' => array(
             'handler' => 'views_handler_relationship',
             'label' => t('@entity from VGwort',  array('@entity' => $entity_type)),
             'base' => $info['base table'],
             'base field' => $info['entity keys']['id'],
             'relationship field' => 'entity_id',
             'extra' => array(
                 array('table' => 'vgwort',
                       'field' => 'entity_type',
                       'value' => $entity_type),
             ),
         ),
     );
   }
  return $data ;
}

class vgwort_handler_field_status extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function render($values) {
    $value = $this->get_value($values);
    switch ( $value ) {
      case VGWORT_STATUS_ASSIGNED:
        $value = t('Assigned') ;
      break ;
      case VGWORT_STATUS_NEW:
        $value = t('New') ;
      break ;
      case VGWORT_STATUS_PUBLISHED:
        $value = t('Published') ;
      break ;
      case VGWORT_STATUS_REMOVED:
        $value = t('Removed') ;
      break ;
      case VGWORT_STATUS_ERROR:
        $value = t('Error') ;
      break ;
    }
    return $this->sanitize_value($this->options['prefix'], 'xss')
    . $this->sanitize_value($value)
    . $this->sanitize_value($this->options['suffix'], 'xss');
  }
}

/**
 * Add status handler for views
 */
class vgwort_handler_filter_status extends views_handler_filter_in_operator {

  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $this->value_options = array(
      VGWORT_STATUS_NEW => t('New') ,
      VGWORT_STATUS_ASSIGNED => t('Assigned') ,
      VGWORT_STATUS_PUBLISHED => t('Published') ,
      VGWORT_STATUS_REMOVED => t('Removed') ,
      VGWORT_STATUS_ERROR => t('Error') ,
    );

    return $this->value_options;
  }
}

/**
 * implement hook_views_default_views
 *
 * Add adminstration view
 * @return views
 */
function vgwort_views_default_views() {
  $view = new view();
  $view->name = 'vgwort_administration';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'vgwort';
  $view->human_name = 'VGWort Administration';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'VGWort Administration';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view vgwort report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: VGWort: VGWort to Node */
  $handler->display->display_options['relationships']['vgwort_related_node']['id'] = 'vgwort_related_node';
  $handler->display->display_options['relationships']['vgwort_related_node']['table'] = 'vgwort';
  $handler->display->display_options['relationships']['vgwort_related_node']['field'] = 'vgwort_related_node';
  /* Field: Vgwort: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'vgwort';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'vgwort_related_node';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'vgwort_related_node';
  /* Field: Vgwort: Modified */
  $handler->display->display_options['fields']['modified']['id'] = 'modified';
  $handler->display->display_options['fields']['modified']['table'] = 'vgwort';
  $handler->display->display_options['fields']['modified']['field'] = 'modified';
  /* Field: Vgwort: Private */
  $handler->display->display_options['fields']['private']['id'] = 'private';
  $handler->display->display_options['fields']['private']['table'] = 'vgwort';
  $handler->display->display_options['fields']['private']['field'] = 'private';
  /* Field: Vgwort: Public */
  $handler->display->display_options['fields']['public']['id'] = 'public';
  $handler->display->display_options['fields']['public']['table'] = 'vgwort';
  $handler->display->display_options['fields']['public']['field'] = 'public';
  /* Filter criterion: Vgwort: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'vgwort';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/reports/vgwort/list';

  $views['vgwort_administration'] = $view ;
  return $views ;
}