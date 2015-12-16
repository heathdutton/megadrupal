<?php
/**
* @file
*   Views query plugin for Views info.
*/

/**
* Views query plugin for the Views/ctools info.
*/
class architecture_views_data_plugin_query extends views_plugin_query {
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
    // Have to instantiate the thing to call its static method.
    $views_query_plugin_default = new views_plugin_query_default();

    // I don't know what this does, but it complains if these aren't set.
    $views_query_plugin_default->base_table = $table;
    $views_query_plugin_default->base_field = $field;

    // This is the actual thing that needs to get done.
    $alias = $views_query_plugin_default->add_field($table, $field, $alias = '', $params = array());

    return $alias;
  }

  /**
   * Overrides the execute() method.
   * @inheritdoc
   */
  function execute(&$view) {
    // Get all views. This includes those stored in code.
    foreach(views_get_all_views() as $zview) {
      if (@$zview->export_module !== 'architecture') {
        // Examine each display of each view.
        foreach($zview->display as $display) {
          $row = new stdClass();
          // Since we can't render the query here bc of recursion, pass view &
          // display to a field. We'll use a special field handler to get the
          // query for each display.
          $row->view_query = array(
            'view' => $zview,
            'display_id' => $display->id,
          );
          // Access is per display.
          if (isset($display->display_options['access'])) {
            $row->access = $display->display_options['access']['type'];
            $count = 0;
            $type = $display->display_options['access']['type'];
            switch ($type) {
              case 'role':
                $count = 0;
                // Create a list in string form of the array items.
                $row->access .= ': ';
                foreach ($display->display_options['access'][$type] as $rid) {
                  $row->access .= user_role_load($rid)->name;
                  $count++;
                  if ($count < count($display->display_options['access'][$display->display_options['access']['type']])) {  $row->access .= ', '; }
                }
                break;
              case 'perm':
                if (isset($display->display_options['access'][$type])) { $row->access .= ': ' . $display->display_options['access'][$type]; }
                break;
              default:
                break;
            }
          }
          if (isset($display->display_options['fields'])) {
            $row->fields = '';
            $count = 0;
            foreach ($display->display_options['fields'] as $field) {
              $row->fields .= $field['field'];
              $count++;
                  if ($count < count($display->display_options['fields'])) {  $row->fields .= ', '; }
            }
          }
          if (isset($display->display_options['path'])) {
            $row->path = $display->display_options['path'];
          }
          $row->tag = $zview->tag;
          $row->machine_name = $zview->name;
          $row->storage = isset($zview->in_code_only) ?'Code' : 'Database';
          $row->title = $zview->human_name;
          $row->base_table = $zview->base_table;
          $row->description = $zview->description;
          $row->enabled = $zview->disabled ? 'No' : 'Yes';
          $row->display_title = $display->display_title;
          $view->result[] = $row;
        }
      }
    }
  }
}