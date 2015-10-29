<?php

/**
 * Form builder for the search box in the issue cockpit block.
 */
function project_issue_issue_cockpit_searchbox($form, &$form_state, $node) {
  $machine_name = $node->field_project_machine_name[LANGUAGE_NONE][0]['value'];
  $search_path = "project/issues/$machine_name";
  $adv_search_path = "project/issues/search/$machine_name";
  return array(
    '#action' => url($search_path),
    '#method' => 'get',
    'text' => array(
      '#title' => t('Search'),
      '#title_display' => 'invisible',
      '#type' => 'textfield',
      '#size' => 20,
    ),
    'status' => array(
      '#type' => 'hidden',
      '#value' => 'All',
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#name' => '',
    ),
    'advanced' => array(
      '#markup' => l(t('Advanced search'), $adv_search_path),
    ),
  );
}

/**
 * Theme template preprocess to declare variables for the issue cockpit.
 */
function template_preprocess_project_issue_issue_cockpit(&$vars) {
  $node = $vars['node'];

  $machine_name = $node->field_project_machine_name[LANGUAGE_NONE][0]['value'];

  // Flags that indicate what kind of access to project issues to allow.
  $has_issues = !empty($node->field_project_has_issue_queue[LANGUAGE_NONE][0]['value']);
  $vars['view_issues'] = $has_issues;
  $make_issues = FALSE;
  foreach (project_issue_issue_node_types() as $issue_type) {
    if (node_access('create', $issue_type)) {
      $make_issues = TRUE;
      break;
    }
  }
  $vars['make_issues'] = $has_issues && $make_issues;

  if ($vars['view_issues']) {
    $vars['path'] = "project/issues/$machine_name";
    $vars['form'] = drupal_get_form('project_issue_issue_cockpit_searchbox', $node);

    $vars['categories'] = array();
    $categories = array('All' => t('All issues')) + project_issue_categories();
    $setting = variable_get('project_issue_cockpit_categories', array('All' => 'All', 1 => 'Bug reports'));
    $active_categories = array_intersect_key($categories, array_filter($setting));
    foreach ($active_categories as $category => $name) {
      $vars['categories'][$category] = array(
        'name' => $name,
        'open' => 0,
        'total' => 0,
      );
    }

    $query = db_select('node', 'n');
    $query->innerJoin('field_data_field_project', 'p', "n.nid = p.entity_id AND p.entity_type = 'node'");
    $query->innerJoin('field_data_field_issue_category', 'c', "n.nid = c.entity_id AND c.entity_type = 'node'");
    // @todo Switch to using the "is an open issue" field during #1549428.
    //$query->innerJoin('field_data_field_issue_open', 'o', "n.nid = o.entity_id AND o.entity_type = 'node'");
    $query->innerJoin('field_data_field_issue_status', 's', "n.nid = s.entity_id AND s.entity_type = 'node'");
    $query->condition('n.status', 1);
    $query->condition('p.field_project_target_id', $node->nid);
    $query->addTag('node_access');

    if (isset($vars['categories']['All'])) {
      $temp = clone($query);
      // @todo Switch to using the "is an open issue" field during #1549428.
      //$temp->condition('o.field_issue_open_value', 1);
      $temp->condition('s.field_issue_status_value', project_issue_open_states());
      $vars['categories']['All']['open'] = $temp->countQuery()->execute()->fetchField();
      $temp = clone($query);
      $vars['categories']['All']['total'] = $temp->countQuery()->execute()->fetchField();
    }

    foreach ($active_categories as $category => $name) {
      if ($category == 'All') {
        continue;
      }
      $cquery = clone($query);
      $cquery->condition('c.field_issue_category_value', $category);

      $temp = clone($cquery);
      // @todo Switch to using the "is an open issue" field during #1549428.
      //$temp->condition('o.field_issue_open_value', 1);
      $temp->condition('s.field_issue_status_value', project_issue_open_states());

      $vars['categories'][$category]['open'] = $temp->countQuery()->execute()->fetchField();

      $temp = clone($cquery);
      $vars['categories'][$category]['total'] = $temp->countQuery()->execute()->fetchField();
    }

    $vars['issue_subscribe'] = l(t('Subscribe via e-mail'), "project/issues/subscribe-mail/$machine_name", array('query' => drupal_get_destination()));

    if (module_exists('sampler') && module_exists('d3_sparkline')) {
      $vars['issue_metrics'] = '';
      foreach (array(
        'project_issue_metrics_new_issues',
        'project_issue_metrics_response_rate',
        'project_issue_metrics_first_response',
        'project_issue_metrics_open_bugs',
        'project_issue_metrics_participants',
      ) as $view_name) {
        $view = views_get_view($view_name);
        if ($view && $view->access('default')) {
          $view->set_display('default');
          $view->pre_execute(array($node->nid));
          $view->execute();
          if (!empty($view->result)) {
            $vars['issue_metrics'] .= '<div class="metric-container">';
            $vars['issue_metrics'] .= '<span class="metric-label">' . $view->get_title() . '</span>';
            $vars['issue_metrics'] .= $view->display_handler->preview();
            $vars['issue_metrics'] .= '</div>';
          }
          $view->post_execute();
        }
      }
    }
  }
}
