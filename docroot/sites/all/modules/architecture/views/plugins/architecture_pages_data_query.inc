<?php
/**
* @file
*   Views query plugin for Page Manager info.
*/

/**
* Views query plugin for the page manager/ctools info.
*/
class architecture_pm_data_plugin_query extends views_plugin_query {
  /**
   * Overrides the query() method.
   * @inheritdoc
   */
  function query($get_count = FALSE) {

  }

  /**
   * Reproducing add_field() here so the spaghetti doesn't stick together.
   */
  function add_field($table, $field, $alias = '', $params = array()) {
    $alias = views_plugin_query_default::add_field($table, $field, $alias = '', $params = array());
    return $alias;
  }

  /**
   * Overrides the execute() method.
   * @inheritdoc
   */
  function execute(&$view) {
  // Get all the page manager stuff and sort through it.
  $tasks = page_manager_get_tasks();
    foreach ($tasks as $task) {
      // If there are subtasks, ignore the parent task and go to subtasks.
      if(isset($task['subtasks'])) {
        $subtasks = page_manager_get_task_subtasks($task);
        foreach ($subtasks as $subtask){
          // Getting the handlers gets us the panels stuff for the pages.
          $handlers = page_manager_load_task_handlers($subtask);
          foreach($handlers as $handler) {
            $contents = $handler->conf['display']->content;
            foreach($contents as $content) {
              $row = $this->architectureCreatePmRow($subtask);
              architectureAddHandlerFields($row, $handler, $content);
              $view->result[] = $row;
            }
          }
        }
      }
      // If there are no subtasks, then dive into the task.
      else {
        // Getting the handlers gets us the panels stuff for the pages.
        $handlers = page_manager_load_task_handlers($task);
        if($handlers){
          foreach($handlers as $key => $handler) {
            if(!isset($handler->conf)) {
              $row = $this->architectureCreatePmRow($task);
              $row->handler = $handler->name;
              $view->result[] = $row;
            }
            else {
              $contents = $handler->conf['display']->content;
              // This is where the individual panels info lives.
              foreach($contents as $key => $content) {
                $row = $this->architectureCreatePmRow($task);
                architectureAddHandlerFields($row, $handler, $content);
                $view->result[] = $row;
              }
            }
          }
        }
        // If there are no handlers for the task, then just use what we've got.
        else {
          $view->result[] = $this->architectureCreatePmRow($task);
        }
      }
    }
  }

  /**
   * Custom function to streamline putting together a row with common elements.
   * @param array $task
   * @return obj $row
   */
  function architectureCreatePmRow($task) {
    $row = new stdClass();
    $row->storage = isset($task['storage']) ? $task['storage'] : t('In code');
    $row->type = isset($task['task type']) ? $task['task type']: $task['admin type'];
    $row->title = isset($task['title']) ? $task['title'] : $task['admin title'];
    $row->description = isset($task['admin description']) ? $task['admin description'] : NULL;
    // If it's not non-disabled, it's disabled
    $task['disabled'] = isset($task['disabled']) ? $task['disabled'] : 1;
    $row->enabled = !$task['disabled'] ? t('Yes') : t('No');
    $row->path = isset($task['admin path']) ? $task['admin path'] : $task['path'];
    return $row;
  }
}

/**
 * If there's a handler, add all the handler info to the row.
 * @param obj $row
 * @param obj $handler
 * @param obj $content
 */
function architectureAddHandlerFields(&$row, $handler, $content) {
    $row->handler = $handler->name;
    $row->handler_type = $content->type;
    $row->handler_subtype = $content->subtype;
    $row->handler_panel = $content->panel;
    $row->handler_admin_title = isset($content->configuration['admin_title']) ? $content->configuration['admin_title'] : NULL;
    $row->handler_title = isset($content->configuration['title']) ? $content->configuration['title'] : NULL;
    $row->handler_body = isset($content->configuration['body']) ? $content->configuration['body'] : NULL;
}