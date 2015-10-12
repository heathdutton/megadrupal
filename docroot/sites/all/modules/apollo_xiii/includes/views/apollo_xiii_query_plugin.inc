<?php

/**
 * @file
 * Code for the query plugin.
 */

/**
 * Custom query class to read information from Drupal project issues.
 * Our custom Query handler must be aware
 * of some information about the current view.
 */
class apollo_xiii_query_plugin extends views_plugin_query_default {

  /**
   * Execute the query.
   */
  public function execute(&$view) {
    $projects = str_replace(array("\r\n", "\r", "\n"), ',', $this->options['apollo_xiii_projects']);
    $projects = explode(',', $projects);
    $projects = array_map('trim', $projects);
    $projects = array_filter($projects);
    $query = array();

    $options = array(
      'projects',
      'text',
      'assigned',
      'submitted',
      'participant',
      'status',
      'priorities',
      'categories',
      'version',
    );

    foreach ($options as $option) {
      if (!empty($this->options['apollo_xiii_' . $option])) {
        $query[$option] = $this->options['apollo_xiii_' . $option];
      }
    }

    // Eetrive all data.
    $rows = array();
    if (is_array($projects)) {
      foreach ($projects as $project) {
        $url = 'http://drupal.org/project/issues/search/';
        $url .= $project . '/rss?' . http_build_query($query);
        $request = drupal_http_request($url);
        if ($request->status_message == 'OK' && $request->code == 200) {
          $data = str_replace('<dc:creator>', '<creator>', $request->data);
          $data = str_replace('</dc:creator>', '</creator>', $data);
          $data = simplexml_load_string($data);
          $project_title = $data->channel->title;
          if (strpos($project_title, 'Search issues for') === 0) {
            $project_title = substr($project_title, 18);
          }
          $channel = (object) array(
            'project_name' => $project,
            'project_title' => $project_title,
          );
          foreach ($data->channel->item as $item) {
            $row = array();
            foreach ($view->field as $key => $field) {
              $part = $field->definition['rss_part'];
              $part_field = $field->definition['rss_field'];
              if ($part == 'channel') {
                $row[$key] = $field->inputFilter($channel->{$part_field});
              }
              else {
                $row[$key] = $field->inputFilter($item->{$part_field});
              }
            }

            // Perform conditions.
            $pass_conditions = !count($this->where) || $this->group_operator == 'AND';

            foreach ($this->where as $where) {
              $local_pass_condition = $where['type'] == 'AND';
              foreach ($where['conditions'] as $condition) {
                list(, $field) = explode('.', $condition['field']);
                switch ($condition['operator']) {
                  case '<':
                    if ($row[$field] < $condition['value'] && $where['type'] == 'OR') {
                      $local_pass_condition = TRUE;
                    }
                    elseif ($row[$field] >= $condition['value'] && $where['type'] == 'AND') {
                      $local_pass_condition = FALSE;
                    }
                    break;

                  case '<=':
                    if ($row[$field] <= $condition['value'] && $where['type'] == 'OR') {
                      $local_pass_condition = TRUE;
                    }
                    elseif ($row[$field] > $condition['value'] && $where['type'] == 'AND') {
                      $local_pass_condition = FALSE;
                    }
                    break;

                  case '=':
                  case 'LIKE':
                    if ($row[$field] == $condition['value'] && $where['type'] == 'OR') {
                      $local_pass_condition = TRUE;
                    }
                    elseif ($row[$field] != $condition['value'] && $where['type'] == 'AND') {
                      $local_pass_condition = FALSE;
                    }
                    break;

                  case '!=':
                  case 'NOT LIKE':
                    if ($row[$field] != $condition['value'] && $where['type'] == 'OR') {
                      $local_pass_condition = TRUE;
                    }
                    elseif ($row[$field] == $condition['value'] && $where['type'] == 'AND') {
                      $local_pass_condition = FALSE;
                    }
                    break;

                  case '>=':
                    if ($row[$field] >= $condition['value'] && $where['type'] == 'OR') {
                      $local_pass_condition = TRUE;
                    }
                    elseif ($row[$field] < $condition['value'] && $where['type'] == 'AND') {
                      $local_pass_condition = FALSE;
                    }
                    break;

                  case '>':
                    if ($row[$field] > $condition['value'] && $where['type'] == 'OR') {
                      $local_pass_condition = TRUE;
                    }
                    elseif ($row[$field] <= $condition['value'] && $where['type'] == 'AND') {
                      $local_pass_condition = FALSE;
                    }
                    break;

                  default:
                    watchdog(
                      'error',
                      'Operator %operator is not defined',
                      array('%operator' => $condition['operator']),
                      WATCHDOG_ERROR
                    );
                }
              }
              if ($pass_conditions && $this->group_operator == 'AND' && !$local_pass_condition) {
                $pass_conditions = FALSE;
              }
              if (!$pass_conditions && $this->group_operator == 'OR' && $local_pass_condition) {
                $pass_conditions = TRUE;
              }

            }

            if ($pass_conditions) {
              $rows[] = $row;
            }
          }
        }
      }
    }

    // Perform order by.
    $order_fields = array();
    if (!empty($this->fields)) {
      foreach ($this->orderby as $order) {
        $field = $order['field'];
        $real_field = $this->fields[$field]['field'];
        $order_fields[] = array(
          'field' => $real_field,
          'direction' => $order['direction'],
        );
      }
    }

    $order_rows = array();
    foreach ($rows as $key => $row) {
      foreach ($order_fields as $field) {
        $order_rows[$field['field']][$key] = NULL;
        if (isset($row[$field['field']])) {
          $order_rows[$field['field']][$key] = $row[$field['field']];
        }
      }
    }

    $sort_arg = array();
    foreach ($order_fields as $field) {
      $sort_arg[] = $order_rows[$field['field']];
      $sort_arg[] = $field['direction'] == 'DESC' ? SORT_DESC : SORT_ASC;
    }
    $sort_arg[] = & $rows;

    call_user_func_array('array_multisort', $sort_arg);

    $limit = intval(!empty($this->limit) ? $this->limit : 999999);
    $offset = intval(!empty($this->offset) ? $this->offset : 0);

    foreach (array_slice($rows, $offset, $limit) as $row) {
      $view->result[] = (object) $row;
    }

    // Pager.
    $this->pager->total_items = count($rows);
    $this->pager->update_page_info();
    $this->pager->post_execute($view->result);

    if ($this->pager->use_count_query() || !empty($view->get_total_rows)) {
      $view->total_rows = $this->pager->get_total_items();
    }

  }

  /**
   * Get all query plugin options.
   */
  public function option_definition() {
    $options = array();
    $options['apollo_xiii_projects'] = array(
      'default' => '',
    );
    $options['apollo_xiii_text'] = array(
      'default' => '',
    );
    $options['apollo_xiii_assigned'] = array(
      'default' => '',
    );
    $options['apollo_xiii_submitted'] = array(
      'default' => '',
    );
    $options['apollo_xiii_participant'] = array(
      'default' => '',
    );
    $options['apollo_xiii_status'] = array(
      'default' => array(),
    );
    $options['apollo_xiii_priorities'] = array(
      'default' => array(),
    );
    $options['apollo_xiii_categories'] = array(
      'default' => array(),
    );
    $options['apollo_xiii_version'] = array(
      'default' => array(),
    );

    return $options;
  }

  /**
   * Get the form for each options.
   */
  public function options_form(&$form, &$form_state) {
    $form['apollo_xiii_projects'] = array(
      '#title' => t('Projects'),
      '#description' => t('Enter a comma separated list of projects.'),
      '#type' => 'textarea',
      '#default_value' => $this->options['apollo_xiii_projects'],
    );
    $form['apollo_xiii_text'] = array(
      '#title' => t('Search for'),
      '#type' => 'textfield',
      '#default_value' => $this->options['apollo_xiii_text'],
    );
    $form['apollo_xiii_assigned'] = array(
      '#title' => t('Assigned'),
      '#description' => t('Enter a comma separated list of users.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['apollo_xiii_assigned'],
    );
    $form['apollo_xiii_submitted'] = array(
      '#title' => t('Submitted by'),
      '#description' => t('Enter a comma separated list of users.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['apollo_xiii_submitted'],
    );
    $form['apollo_xiii_participant'] = array(
      '#title' => t('Participant'),
      '#description' => t('Enter a comma separated list of users.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['apollo_xiii_participant'],
    );
    $form['apollo_xiii_status'] = array(
      '#title' => t('Status'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => 5,
      '#options' => array(
        'Open' => t('- Open issues -'),
        '1' => t('active'),
        '13' => t('needs work'),
        '8' => t('needs review'),
        '14' => t('reviewed &amp; tested by the community'),
        '15' => t('patch (to be ported)'),
        '2' => t('fixed'),
        '4' => t('postponed'),
        '16' => t('postponed (maintainer needs more info)'),
        '3' => t('closed (duplicate)'),
        '5' => t("closed (won't fix)"),
        '6' => t('closed (works as designed)'),
        '18' => t('closed (cannot reproduce)'),
        '7' => t('closed (fixed)'),
      ),
      '#default_value' => $this->options['apollo_xiii_status'],
    );
    $form['apollo_xiii_priorities'] = array(
      '#title' => t('Priority'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => 5,
      '#options' => array(
        '1' => t('critical'),
        '4' => t('major'),
        '2' => t('normal'),
        '3' => t('minor'),
      ),
      '#default_value' => $this->options['apollo_xiii_priorities'],
    );
    $form['apollo_xiii_categories'] = array(
      '#title' => t('Category'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => 5,
      '#options' => array(
        'bug' => t('bug report'),
        'task' => t('task'),
        'feature' => t('feature request'),
        'support' => t('support request'),
      ),
      '#default_value' => $this->options['apollo_xiii_categories'],
    );
    $form['apollo_xiii_version'] = array(
      '#title' => t('Target Drupal version'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => 5,
      '#options' => array(
        '8.x' => t('!version issues', array('!version' => '8.x')),
        '7.x' => t('!version issues', array('!version' => '7.x')),
        '6.x' => t('!version issues', array('!version' => '6.x')),
        '5.x' => t('!version issues', array('!version' => '5.x')),
        '4.7.x' => t('!version-x.x issues', array('!version' => '4.7.x')),
      ),
      '#default_value' => $this->options['apollo_xiii_version'],
    );

  }

}
