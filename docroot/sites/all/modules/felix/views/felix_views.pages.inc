<?php
/**
 * @file
 * Functions relating to menu callbacks for the Felix Views module.
 */

/**
 * Menu callback for autocomplete node fields.
 */
function felix_views_autocomplete_term($text) {
  $matches = array();
  $terms = db_select('taxonomy_term_data', 'td')
    ->fields('td', array('tid', 'name'))
    ->condition('td.name', db_like($text) . '%', 'LIKE')
    ->orderBy('td.name', 'ASC')
    ->range(0, 10)
    ->execute()
    ->fetchAll();
  foreach ($terms as $term) {
    $matches[$term->tid] = check_plain($term->name);
  }
  drupal_json_output($matches);
}

/**
 * Menu callback for autocomplete node fields.
 */
function felix_views_autocomplete_node($text) {
  $matches = array();
  $nodes = db_select('node', 'n')
    ->fields('n', array('nid', 'type', 'title'))
    ->condition('n.title', db_like($text) . '%', 'LIKE')
    ->orderBy('n.created', 'DESC')
    ->range(0, 10)
    ->execute()
    ->fetchAll();
  foreach ($nodes as $node) {
    $matches[$node->nid] = check_plain($node->title . ' (' . node_type_get_name($node->type) . ')');
  }
  drupal_json_output($matches);
}

/**
 * Get views arguments for a given display.
 *
 * @param object $view
 * @param string $display
 * @return array
 */
function _felix_views_get_view_arguments($view, $display) {
  $arguments = array();

  // If the arguments are overriden and not set for this display.
  if (isset($view->display[$display]->display_options['defaults']['arguments']) and
    $view->display[$display]->display_options['defaults']['arguments'] === FALSE) {
    return array();
  }

  // Add arguments from default display.
  if (!empty($view->display['default']->display_options['arguments'])) {
    $arguments += $view->display['default']->display_options['arguments'];
  }
  // Add arguments from $display.
  if (!empty($view->display[$display]->display_options['arguments'])) {
    $arguments += $view->display[$display]->display_options['arguments'];
  }
  return $arguments;
}

/**
 * Get form element for views argument.
 *
 * @param string $table Storage table
 * @param string $field Storage field
 * @param string $title Administrative title
 * @param mixed $value Default value
 * @param object $view
 * @param object $display
 * @return array
 */
function _felix_views_argument_element($table, $field, $title = NULL, $value = NULL, $view = NULL, $display = NULL) {
  switch ("$table.$field") {
    // Node type argument.
    case 'node.type':
      $info = entity_get_info('node');
      $options = array();
      foreach ($info['bundles'] as $name => $bundle) {
        $options[$name] = $bundle['label'];
      }
      asort($options);
      $options = array_merge(array('' => t('Use filter default')), $options);
      return array(
        '#type' => 'select',
        '#title' => $title ? t($title) : t('Type'),
        '#options' => $options,
        '#default_value' => $value,
      );

    // Node reference.
    case 'node.nid':
      return array(
        '#type' => 'textfield',
        '#title' => $title ? t($title) : t('Node'),
        '#default_value' => $value,
        '#autocomplete_path' => 'felix-blocks-views/autocomplete-node',
      );
      break;

    // Taxonomy term reference.
    case 'taxonomy_index.tid':
      return array(
        '#type' => 'textfield',
        '#title' => $title ? t($title) : t('Taxonomy term'),
        '#default_value' => $value,
        '#autocomplete_path' => 'felix-blocks-views/autocomplete-term',
      );
      break;

    // Subqueues for nodequeue 3.x.
    case 'nodequeue_subqueue.reference':
      $queues = null;
      if ($view && isset($view->display[$display])) {
        $display_options = array_merge($view->display[$display]->display_options, $view->display['default']->display_options);
        $relationship = NULL;
        if (isset($display_options['arguments']['reference'])) {
          $relationship = $display_options['arguments']['reference']['relationship'];
        }
        $queues = NULL;
        if (isset($display_options['relationships'][$relationship])) {
          $queues = $display_options['relationships'][$relationship]['queues'];
        }
        if ($queues) {
          $queues = array_filter(array_values($queues));
        }
      }

      // Build query to fetch possible options.
      $q = db_select('nodequeue_subqueue', 's')
        ->fields('s', array('sqid', 'reference', 'title'));
      if ($queues) {
        // This relationship used by this argument is limited to the given queues.
        $q->condition('s.name', $queues);
      }
      $q->join('nodequeue_queue', 'q', 's.name = q.name');
      $subqueues = $q->fields('q', array('subqueue_title'))
        ->orderBy('q.subqueue_title', 'asc')
        ->orderBy('s.title', 'asc')
        ->range(0, 1000)
        ->execute()
        ->fetchAll();
      $options = array();
      foreach ($subqueues as $subqueue) {
        !empty($subqueue->subqueue_title) or $subqueue->subqueue_title = '%subqueue';
        $options[$subqueue->reference] = strip_tags(t($subqueue->subqueue_title, array('%subqueue' => $subqueue->title)));
      }
      return array(
        '#type' => 'select',
        '#title' => $title ? t($title) : t('Subqueue'),
        '#options' => $options,
        '#default_value' => $value,
      );
      break;

    // Subqueues for nodequeue 2.x.
    case 'nodequeue_nodes.sqid':
      // Get details about this field relationship.
      if ($view && isset($view->display[$display])) {
        $relationship = NULL;
        if (isset($view->display[$display]->display_options['arguments']['sqid'])) {
          $relationship = $view->display[$display]->display_options['arguments']['sqid']['relationship'];
        }
        elseif (isset($view->display['default']->display_options['arguments']['sqid'])) {
          $relationship = $view->display['default']->display_options['arguments']['sqid']['relationship'];
        }
        $queues = NULL;
        if (isset($view->display[$display]->display_options['relationships'][$relationship])) {
          $queues = $view->display[$display]->display_options['relationships'][$relationship]['queues'];
        }
        elseif (isset($view->display['default']->display_options['relationships'][$relationship])) {
          $queues = $view->display['default']->display_options['relationships'][$relationship]['queues'];
        }
        if ($queues) {
          $queues = array_filter(array_values($queues));
        }
      }

      // Build query to fetch possible options.
      $q = db_select('nodequeue_subqueue', 's')
        ->fields('s', array('sqid', 'title'));
      if ($queues) {
        // This relationship used by this argument is limited to the given queues.
        $q->condition('s.name', $queues);
      }
      $q->join('nodequeue_queue', 'q', 's.name = q.name');
      $subqueues = $q->fields('q', array('subqueue_title'))
        ->orderBy('q.subqueue_title', 'asc')
        ->orderBy('s.title', 'asc')
        ->range(0, 1000)
        ->execute()
        ->fetchAll();
      $options = array();
      foreach ($subqueues as $subqueue) {
        !empty($subqueue->subqueue_title) or $subqueue->subqueue_title = '%subqueue';
        $options[$subqueue->sqid] = strip_tags(t($subqueue->subqueue_title, array('%subqueue' => $subqueue->title)));
      }
      return array(
        '#type' => 'select',
        '#title' => $title ? t($title) : t('Subqueue'),
        '#options' => $options,
        '#default_value' => $value,
      );
      break;

    // Unrecognised option. Allow the user to enter the value in a textfield.
    default:
      return array(
        '#type' => 'textfield',
        '#title' => $title ? t($title) : t('Argument %table.%field', array('%table' => $table, '%field' => $field)),
        '#default_value' => $value,
      );
      break;
  }
}
