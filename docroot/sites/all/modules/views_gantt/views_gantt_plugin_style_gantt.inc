<?php

/**
 * @file
 * Contains the list style plugin.
 */

/**
 * Style plugin to render Gantt charts.
 *
 * @ingroup views_style_plugins
 */
class views_gantt_plugin_style_gantt extends views_plugin_style_list {
  private $option_fields;
  private $options_node;

  private $project;
  private $tasks = array();
  private $columns = array();

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['height'] = array('default' => '');
    $options['class'] = array('default' => '');
    $options['wrapper_class'] = array('default' => 'item-list');
    $options['id_field'] = array('default' => '');
    $options['name_field'] = array('default' => '');
    $options['parent_field'] = array('default' => '');
    $options['date_field'] = array('default' => '');
    $options['end_date_field'] = array('default' => '');
    $options['progress_field'] = array('default' => '');
    $options['project_id_field'] = array('default' => '');
    $options['project_date_field'] = array('default' => '');
    $options['parent_id_field'] = array('default' => '');
    $options['predecessor_id_field'] = array('default' => '');
    $options['columns'] = array('default' => '');
    $options['task_type'] = array('default' => '');
    $options['add_task'] = array('default' => 0);
    $options['edit_task'] = array('default' => 0);
    $options['scale'] = array('default' => 2);
    return $options;
  }
  /**
   * Style option form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $fields = array('' => t('<None>'));

    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      if ($label = $handler->label()) {
        $fields[$field] = $label;
      }
      else {
        $fields[$field] = $handler->ui_name();
      }
    }
    $types = array();
    foreach (node_type_get_types() as $key => $value) {
      $key = str_replace('_', '-', $key);
      $types[$key] = $value->name;
    }
    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height of Gantt Chart'),
      '#default_value' => $this->options['height'],
      '#description' => t('Height of Gantt Chart (in px).'),
      '#size' => '5',
    );
    $form['id_field'] = array(
      '#type' => 'select',
      '#title' => t('ID field'),
      '#options' => $fields,
      '#default_value' => $this->options['id_field'],
      '#description' => t('Select the field that contains nid of each record.'),
      '#required' => TRUE,
    );
    $form['name_field'] = array(
      '#type' => 'select',
      '#title' => t('Name field'),
      '#options' => $fields,
      '#default_value' => $this->options['name_field'],
      '#description' => t('Select the field that contains name of each record.'),
      '#required' => TRUE,
    );
    $form['date_field'] = array(
      '#type' => 'select',
      '#title' => t('Date field'),
      '#options' => $fields,
      '#default_value' => $this->options['date_field'],
      '#description' => t('Select the field that contains the start date (timestamp or valid <a href="http://www.php.net/manual/en/datetime.formats.php" target="_blank">date format</a>) of the node in the selected row.'),
      '#required' => TRUE,
    );
    $form['end_date_field'] = array(
      '#type' => 'select',
      '#title' => t('End date field'),
      '#options' => $fields,
      '#default_value' => $this->options['end_date_field'],
      '#description' => t('Select the field that contains the end date (timestamp or valid <a href="http://www.php.net/manual/en/datetime.formats.php" target="_blank">date format</a>) of the node in the selected row.'),
      '#required' => TRUE,
    );
    $form['progress_field'] = array(
      '#type' => 'select',
      '#title' => t('Progress field'),
      '#options' => $fields,
      '#default_value' => $this->options['progress_field'],
      '#description' => t('Select the field that contains the progress of the node in percents.'),
      '#required' => TRUE,
    );
    $form['project_id_field'] = array(
      '#type' => 'select',
      '#title' => t('Project ID field'),
      '#options' => $fields,
      '#default_value' => $this->options['project_id_field'],
      '#description' => t("Select the field that contains nid of the record's project node."),
      '#required' => TRUE,
    );
    $form['task_type'] = array(
      '#type' => 'select',
      '#title' => t('Task type'),
      '#options' => $types,
      '#default_value' => $this->options['task_type'],
      '#description' => t("Select the task node type."),
      '#required' => TRUE,
    );
    $form['project_date_field'] = array(
      '#type' => 'select',
      '#title' => t('Project date field'),
      '#options' => $fields,
      '#default_value' => $this->options['project_date_field'],
      '#description' => t("Select the field that contains the date (timestamp or valid <a href='http://www.php.net/manual/en/datetime.formats.php' target='_blank'>date format</a>) of the record's project node. If not provided, date of the earliest task will be used as project start date."),
    );
    $form['parent_id_field'] = array(
      '#type' => 'select',
      '#title' => t('Parent ID field'),
      '#options' => $fields,
      '#default_value' => $this->options['parent_id_field'],
      '#description' => t("Select the field that contains nid of the record's parent node."),
    );
    $form['predecessor_id_field'] = array(
      '#type' => 'select',
      '#title' => t('Predecessor ID field'),
      '#options' => $fields,
      '#default_value' => $this->options['predecessor_id_field'],
      '#description' => t("Select the field that contains nid of the record's predecessor node."),
    );

    $form['columns'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => t('Additional columns'),
      '#options' => $fields,
      '#default_value' => $this->options['columns'],
    );
    $form['add_task'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add Task'),
      '#default_value' => $this->options['add_task'],
      '#description' => t("Enable posibility 'Add Task' from Gantt chart."),
    );
    $form['edit_task'] = array(
      '#type' => 'checkbox',
      '#title' => t('Edit Task'),
      '#default_value' => $this->options['edit_task'],
      '#description' => t("Enable posibility 'Edit Task' from Gantt chart."),
    );
    $form['scale'] = array(
      '#type' => 'select',
      '#title' => t('Scale level'),
      '#options' => array(
        1 => 'Hours',
        2 => 'Days',
        3 => 'Months',
      ),
      '#default_value' => $this->options['scale'],
      '#description' => t("Select default scale level."),
    );
  }
  /**
   * Render the given style.
   */
  function render() {
    // Check for live preview.
    if (isset($this->view->live_preview)) {
      return t('Gantt Chart not compatible with live preview.');
    }

    // Get columns
    if (!empty($this->options['columns'])) {
      $fields = $this->display->handler->get_handlers('field');
      foreach ($this->options['columns'] as $field) {
        if (!empty($field) && !empty($fields[$field])) {
          $this->columns[$field] = $fields[$field]->label(); 
        }
      }
    }

    $this->options_fields();
    $this->options_node();

    // Get project data.
    if (!$this->get_project()) return;

    // Get array of tasks.
    foreach ($this->view->result as $key => $row) {
      $this->add_task($row, 'view', $key);
    }

    foreach ($this->tasks as $key => $task) {
      $this->load_missing_tasks($key);
      $this->load_missing_values($key);
    }

    // Allow to alter tasks array before next modifications.
    drupal_alter('views_gantt_tasks_prerender', $this->tasks);
    
    // Remove tasks marked for deletion
    foreach ($this->tasks as $key => $value) {
      if ($this->is_delete($key)) {
        unset($this->tasks[$key]);
      }
    }
    
    // Build hierarchical tree of tasks and fix incorrect values.
    $tree = $this->build_tree($this->tasks);
    foreach ($tree as $key => $task) {
      $this->check_date($task);
      $this->check_duration($task);
      $this->calculate_progress($task);
    }

    // Exclude incorrect tasks.
    foreach ($this->tasks as $key => $value) {
      if (!$this->is_correct($key)) {
        unset($this->tasks[$key]);
      }
    }

    $this->mark_modified_tasks();
    $this->views_gantt_before_render();
    return parent::render();
  }

  function build_tree($tasks, $parent_id = '', $level = 0) {
    $branch = array();
    foreach ($tasks as $key => $task) {
      if (!$task['parent_id'] && $key != $this->project['id']) {
        $task['parent_id'] = $this->project['id'];
      }
      if ($task['parent_id'] == $parent_id) {
        $task['id'] = $key;
        $task['level'] = $level;
        $children = $this->build_tree($tasks, $key, $level + 1);
        if ($children) {
          $task['children'] = $children;
        }
        $branch[$key] = $task;
      }
    }

    return $branch;
  }

  function check_date($task, $parent_id = '') {
    if (isset($task['children'])) {
      foreach ($task['children'] as $child) {
        $this->check_date($child, $task['id']);
      }
    }

    $time = $this->get_time($this->tasks[$task['id']]['est']);
    $project_time = $this->get_time($this->project['est']);
  }

  function check_duration($task, $parent_id = '') {
    if (!isset($this->tasks[$task['id']]['duration'])) {
      $this->calculate_duration($task['id']);  
    }

    if (isset($task['children'])) {
      foreach ($task['children'] as $child) {
        $this->check_duration($child, $task['id']);
      }
    }  
  }

  function calculate_progress($task) {
    $progress = &$this->tasks[$task['id']]['percentcompleted'];
    $duration = &$this->tasks[$task['id']]['duration'];

    if (empty($this->tasks[$task['id']]['child_hours_completed'])) {
      $this->tasks[$task['id']]['child_hours_completed'] = 0;
    }
    $child_hours_completed = &$this->tasks[$task['id']]['child_hours_completed'];

    if (isset($task['children'])) {
      foreach ($task['children'] as $child) {
        $child_hours_completed += $this->calculate_progress($child);
      }
      if ($child_hours_completed && $duration) {
        $progress = (string) ceil($child_hours_completed * 100 / $duration);
        $progress = $progress > 100 ? 100 : $progress;

        if (isset($this->tasks[$task['id']]['columns']['progress'])) {
          $this->tasks[$task['id']]['columns']['progress'] = $progress . '%';
        }
      } 
    }

    $hours_completed = $duration * $progress / 100;
    return $hours_completed;
  }

  function calculate_duration($task_id) {
    $start_date = $this->get_time($this->tasks[$task_id]['est']);
    $end_date = $this->get_time($this->tasks[$task_id]['end_date']); 

    // We assumed that 1 day = 8 hours
    $duration = ceil(($end_date - $start_date) / (3600 * 3));  
    $this->tasks[$task_id]['duration'] = $duration; 
  }

  function is_correct($id) {
    $task = $this->tasks[$id];
    $is_correct = $task['duration'] > 0 && $task['est'] && $task['end_date'];
    return $is_correct;
  }  

  function is_delete($id) {
    $task = isset($this->tasks[$id]) ? $this->tasks[$id] : array();
    return isset($task['delete']);
  }

  function get_time($date_string) {
    $date = str_replace(',', '/', $date_string);
    return strtotime($date);
  } 

  function mark_modified_tasks() {
    foreach ($this->tasks as $key => $task) {
      if (isset($task['est_modified']) || isset($task['duration_modified'])) {
        $this->tasks[$key]['name'] .= ' *DATE COLLISION*';
      }

      if (isset($task['start_date_modified'], $task['end_date_modified'])) {
        $this->tasks[$key]['name'] .= ' *START/END DATE MISSING*'; 
      } else if (isset($task['start_date_modified'])) {
        $this->tasks[$key]['name'] .= ' *START DATE MISSING*'; 
      } else if (isset($task['end_date_modified'])) {
        $this->tasks[$key]['name'] .= ' *END DATE MISSING*'; 
      } 
    }    
  }

  /**
   * Returns value of specific field.
   * 
   * @param object $row
   *   Node object from view result array
   * @param string $field_options_name
   *   Option key from views_gantt_plugin_style_gantt::option_definition()
   * 
   * @return string
   *   Field value from node object
   */
  function views_gantt_get_field_value($row, $field_options_name) {
    $field_name = '';
    $field_value = '';
    if (isset($this->options[$field_options_name])) {
      $field_name = $this->options[$field_options_name];
    }

    // If field value is array, we try to get it's raw value,
    // if it's not possible, we get rendered value.
    if ($field_name && isset($this->option_fields[$field_name]) && !empty($this->option_fields[$field_name])) {
      $field_value = $row->{$this->option_fields[$field_name]};
      if (is_array($field_value)) {
        if ($field_options_name == 'end_date_field' && isset($field_value[0]['raw']['value2'])) {
          $field_value = $field_value[0]['raw']['value2'];
        }
        elseif (isset($field_value[0]['raw']['value'])) {
          $field_value = $field_value[0]['raw']['value'];
        }
        elseif (isset($field_value[0]['rendered']['#markup'])) {
          $field_value = $field_value[0]['rendered']['#markup'];
        }
      }

      // If field should provide date, we need to return it's
      // value in specific date format (Y-m-d).
      $date_fields = array('date_field', 'end_date_field', 'project_date_field');
      if (!is_array($field_value) && in_array($field_options_name, $date_fields)) {
        views_gantt_normalize_date_field($field_value);
      }
    }

    return is_array($field_value) && empty($field_value) ? '' : $field_value;
  }

  /**
   * Returns value of specific field from node.
   * 
   * @param object $node
   *   Node object received via node_load()
   * @param string $field_options_name
   *   Option key from views_gantt_plugin_style_gantt::option_definition()
   * 
   * @return string
   *   Field value from node object
   */
  function views_gantt_get_node_field_value($node, $field_options_name) {
    $field_value = '';
    $field_name = '';
    $lang = LANGUAGE_NONE;
    if (isset($this->options_node[$field_options_name])) {
      $field_name = $this->options_node[$field_options_name];
    }

    if ($field_name && isset($node->$field_name) && !empty($node->$field_name)) {
      $field_value = $node->$field_name;
      if (is_array($field_value)) {
        // Get field info.
        $field_info = field_info_field($field_name);

        // If field is translatable, we check if
        // it has an index equals to node language.
        $is_translatable = field_is_translatable('node', $field_info);
        if ($is_translatable && isset($node->{$field_name}[$node->language])) {
          $lang = $node->language;
        }

        $value_keys = array_keys($field_info['columns']);
        $field_value = $node->{$field_name}[$lang][0][$value_keys[0]];

        if ($field_options_name == 'end_date_field' && isset($value_keys[1])) {
          $field_value = $node->{$field_name}[$lang][0][$value_keys[1]];
        }

        // If field type is taxonomy reference.
        if ($value_keys[0] == 'tid') {
          $term = taxonomy_term_load($field_value);
          $field_value = $term->name;
        }
      }

      // If field should provide date, we need to return it's
      // value in specific date format (Y-m-d).
      $date_fields = array('date_field', 'end_date_field', 'project_date_field');
      if (!is_array($field_value) && in_array($field_options_name, $date_fields)) {
        views_gantt_normalize_date_field($field_value);
      }
    }

    return $field_value;
  }  

  /**
   * Gets real names of fields that we put in style settings.
   * If field come from Fields API, we get it's name.
   * Otherwise (if field is 'node_title' for example) we get field alias.
   */
  function options_fields() {
    $this->option_fields = array();
    $view_fields = $this->view->field;
    foreach ($this->view->display_handler->get_handlers('field') as $key => $handler) {
      if (isset($view_fields[$key]) && isset($handler->field_info)) {
        $this->option_fields[$key] = 'field_' . $key;
      }
      else {
        $this->option_fields[$key] = $handler->field_alias;
      }
    }
  }

  /**
   * Some fields in $this->options can have
   * incorrect names (e.g. field_date_1).
   * We need to create new array which contains
   * correct field names to use it in
   * views_gantt_get_node_field_value() function.
   */
  function options_node() {
    $view_fields = $this->view->field;
    foreach ($this->options as $key => $value) {
      if (is_string($value) && isset($view_fields[$value])) {
        $this->options_node[$key] = $view_fields[$value]->field;
      }
    }
  }

  function get_project() {
    $project_id = $this->views_gantt_get_field_value($this->view->result[0], 'project_id_field');
    $node = node_load($project_id);
    if (!$node) {
      $this->project = NULL;
    } else {
      $project_date = $this->views_gantt_get_field_value($this->view->result[0], 'project_date_field');
      $project_end_date = $this->views_gantt_get_field_value($this->view->result[0], 'project_end_date_field');

      $id = $this->add_task($node, 'node');
      $this->project = array('id' => $id) + $this->tasks[$id];

      if (!$project_date) {
        $this->project['est'] = $this->project_date('start');
        $this->tasks[$id]['est'] = $this->project['est'];
      }

      if (!$project_end_date) {
        $this->project['end_date'] = $this->project_date('end');
        $this->tasks[$id]['end_date'] = $this->project['end_date'];
      }

      if (!$this->project['est'] || !$this->project['end_date']) {
        drupal_set_message(t('Gantt Chart requires filled project date.'), 'warning');
        $this->project = NULL;
      }      
    }

    return $this->project;
  }

  function project_date($type = 'start') {
    $date = time();
    if ($this->view->result) {
      foreach ($this->view->result as $value) {
        $task_date = $this->views_gantt_get_field_value($value, 'date_field');
        $task_date = strtotime(str_replace(',', '-', $task_date));

        switch ($type) {
          case 'start':
            $condition = $task_date && $task_date < $date;
            break;
          
          case 'end':
            $condition = $task_date && $task_date > $date;
            break;
        }

        if (!empty($condition)) {
          $date = $task_date;
        }        
      }

      views_gantt_normalize_date_field($date);
      return $date;
    }

    return NULL;
  }

  function load_missing_tasks($id) {
    $task = $this->tasks[$id];
    $fields = array('predecessortasks', 'parent_id');

    foreach ($fields as $field) {
      $check_id = $task[$field];
      $new_task_id = TRUE;
      if ($check_id && !isset($this->tasks[$check_id])) {
        $new_task_id = $this->load_task_from_node($check_id);
        if ($new_task_id) {
          $this->load_missing_tasks($new_task_id);
        }
      }

      if (!$new_task_id || $this->is_delete($check_id)) {
        $this->tasks[$id]['delete'] = TRUE;
      }
    }
  }

  function load_missing_values($id) {
    $task = &$this->tasks[$id];
    // If task has no start_date, try to get parent/project start_date and use it.
    if (!$task['est']) {
      $task['start_date_modified'] = TRUE;
      if ($task['parent_id'] && $this->tasks[$task['parent_id']]['est']) {
        $task['est'] = $this->tasks[$task['parent_id']]['est'];       
      } else {
        $task['est'] = $this->project['est'];
      }
    }

    // If task has no end_date, try to get parent end_date and use it.
    if (!$task['end_date'] || $task['end_date'] == $task['est']) {
      $task['end_date_modified'] = TRUE;
      if ($task['parent_id'] && $this->tasks[$task['parent_id']]['end_date']) {
        $task['end_date'] = $this->tasks[$task['parent_id']]['end_date'];       
      }
    }

    // If task end_date <= task start_date, 
    // try to use node creation date as start_date.
    $start_time = $this->get_time($task['est']);
    $end_time = $this->get_time($task['end_date']);
    if ($end_time <= $start_time) {
      $node = node_load($id);
      if ($node) {
        $task['end_date'] = $task['est'];
        $task['est'] = $node->created;
        views_gantt_normalize_date_field($task['est']);
      }
    }

    // If after all calculation we still have incorrect dates,
    // try to add one day for end date.
    $start_time = $this->get_time($task['est']);
    $end_time = $this->get_time($task['end_date']);
    if ($end_time <= $start_time) {
      $task['end_date'] = $end_time + 3600 * 24;
      views_gantt_normalize_date_field($task['end_date']);
    }    
  }  

  function load_task_from_node($nid) {
    $node = node_load($nid);
    if ($node) {
      return $this->add_task($node, 'node');
    }

    return FALSE;
  } 

  function add_task($data, $type = 'view', $row_key = NULL) {
    switch ($type) {
      case 'view':
        $method = 'views_gantt_get_field_value';
        $task_id = $this->$method($data, 'id_field');
        break;
      
      case 'node':
        $method = 'views_gantt_get_node_field_value';
        $task_id = $data->nid;
        break;

      default:
        return;
    }

    $keys = array(
      'name' => 'name_field',
      'est' => 'date_field',
      'end_date' => 'end_date_field',
      'percentcompleted' => 'progress_field',
      'predecessortasks' => 'predecessor_id_field',
      'parent_id' => 'parent_id_field',
    );
    
    $task = array(
      'columns' => array(),
    );

    // Add fields for chart
    foreach ($keys as $key => $field) {
      $task[$key] = $this->$method($data, $field);
    }

    // Add columns
    foreach ($this->columns as $key => $value) {
      if ($this->options['progress_field'] == $key) {
        $task['columns']['progress'] = (int) $task['percentcompleted'] . '%';
        continue;
      }

      switch ($type) {
        case 'view':
          $this->options[$key] = $key;
          break;
        
        case 'node':
          $this->options_node[$key] = $key;
          break;
      }

      $task['columns'][$key] = $this->$method($data, $key);
    }    

    $this->tasks[$task_id] = $task + array(
      'childtasks' => array(),
    );

    return $task_id;
  }   

  function views_gantt_before_render() {
    // Save view name to variable.
    variable_set('views_gantt_view_name', $this->view->name);

    // Save data in session to use it when we will build XML file for chart.
    $_SESSION['views_gantt']['project'] = $this->project;
    $_SESSION['views_gantt']['tasks'] = $this->tasks;

    $fields = array(
      'project_field' => $this->options['project_id_field'],
      'parent_field' => $this->options['parent_id_field'],
    );
    foreach ($fields as $key => $value) {
      foreach ($this->view->display as $display) {
        $display_options = $display->display_options;
        if (isset($display_options['style_plugin']) && $display_options['style_plugin'] == 'gantt') {
          if (isset($display->display_options['fields'][$value])) {
            $fields[$key] = $display->display_options['fields'][$value]['field'];
          }
          break;
        }
      }
    }
    
    $_SESSION['views_gantt']['project_field'] = $fields['project_field'];
    $_SESSION['views_gantt']['parent_field'] = $fields['parent_field'];
    // IF we have the progress in columns, we need to change key for
    // automatical update when updating task.
    $columns = array();
    foreach ($this->columns as $key => $value) {
      if ($this->options['progress_field'] == $key) {
        $columns['progress'] = $value;
      } else {
        $columns[$key] = $value;
      }
    }
    $this->columns = $columns;
    
    // Add dhtmlxgantt library.
    $library = libraries_load('dhtmlxgantt');

    // Add css to fix chart style.
    drupal_add_css(drupal_get_path('module', 'views_gantt') . "/css/reset.css");
    // Add js.
    drupal_add_js(drupal_get_path('module', 'views_gantt') . "/js/views_gantt.js");
    // Add jquery ui Dialog library.
    drupal_add_library('system', 'ui.dialog');
    drupal_add_library('system', 'jquery.cookie');
    // Add globals variables to use it in js.  
    $settings = array(
      'views_gantt' => array(
        'project_id' => $this->project['id'],
        'exposed_input' => isset($this->view->exposed_input) ? $this->view->exposed_input : NULL,
        'fullscreen_button' => '<a class="gantt-fullscreen">Fullscreen</a>',
        'columns' => $this->columns,
        'task_type' => $this->options['task_type'],
        'add_task' => $this->options['add_task'],
        'edit_task' => $this->options['edit_task'],
        'scale' => $this->options['scale'],
        'ajax_path' => url('views/ajax'),
        'view' => array(
          'href' => url(current_path(), array('query' => drupal_get_query_parameters())),
          'settings' => array(
            'view_name' => $this->view->name,
            'view_display_id' => $this->view->current_display,
            'view_args' => check_plain(implode('/', $this->view->args)),
            'view_path' => check_plain(current_path()),
            'view_base_path' => $this->view->get_path(),
            'view_dom_id' => $this->view->dom_id,
            'pager_element' => isset($this->view->query->pager) ? $this->view->query->pager->get_pager_id() : 0,
          ),
        ),
      ),
    );

    // Send view settings to js.
    drupal_add_js($settings, 'setting');

    // Add js for proper work Views with ajax.
    views_add_js('ajax_view'); 
  }
}
