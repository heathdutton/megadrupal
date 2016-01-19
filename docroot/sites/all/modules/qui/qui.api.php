<?php
/**
 * @file
 * Show what hooks are available to other modules.
 */

/**
 * Expose new formatters to reports.
 *
 * Formatters must be keyed by the machine_name of your formatter.
 *
 * Valid params are:
 * - name: User friendly name of the formatter.
 * - description: Description of what the formatter does.
 * - function: The theme function that will render the results.
 * - headers: Array of headers to set when viewing a formatted report.
 * - download headers: Array of headers to set when downloading the report.
 * - download extension: File extension to set when downloading the file.
 * - plain: Display the results on a plain page, not within the site.
 * - file: Path to function file.
 * - template: Path to template file.
 * - path: Path where function/template is.
 *
 * @return array
 *   Array of formatters.
 */
function hook_qui_formatters_info() {
  return array(
    'csv' => array(
      'name' => t('CSV'),
      'description' => t('Format data in CSV format'),
      'function' => 'qui_formatter_csv',
      'headers' => array(array('Content-Type' => 'text/csv')),
      'plain' => TRUE,
      'file' => '/Function/File',
      'template' => '/Path/To/Template/File',
      'path' => '/Path/To/Function/File',
    ),
  );
}

/**
 * Expose new handlers to the query builder.
 *
 * To include your handler file, make sure you update your module .info file.
 *   files[] = path_to_custom_handler
 */
function hook_qui_query_builder_handlers_info() {
  return array(
    'handler_name' => array(
      'class' => 'Custom_Qui_Handler',
    ),
  );
}

/**
 * Alter report queries before they are run.
 *
 * @param SelectQuery $query
 *   Report query object.
 * @param QuiReport $report
 *   QuiReport Report.
 */
function hook_qui_report_query_alter(SelectQuery &$query, QuiReport $report) {

}

/**
 * Alter the report query for a specific report.
 *
 * @param SelectQuery $query
 *   Report query object.
 * @param QuiReport $report
 *   QuiReport Report.
 */
function hook_qui_report_machine_name_query_alter(SelectQuery &$query, QuiReport $report) {

}

/**
 * Define reports in code.
 *
 * @return array
 *   Array of report entities keyed by machine_name.
 */
function hook_default_qui_report() {
  $nodes = new QuiReport();
  $nodes->query->select_table = 'node';
  $nodes->name = 'Nodes';
  $nodes->tags = array('test1', 'test2');
  $nodes->description = t('Test description please.');
  $nodes->settings->allowed_formats = array('table', 'csv', 'json');
  $nodes->settings->allowed_roles = array(3);
  $nodes->query->conditions[] = array(
    'type' => 'where',
    'column' => 'node.uid',
    'value' => 1,
    'operator' => 'e',
  );
  $items['example1'] = $nodes;
  $items['example2'] = entity_import('qui_report', '{
    "name" : "Nodes",
    "query" : {
      "select_table" : "node",
      "joins" : [
        {"type": "left", "table": "users", "left_col": "users.uid", "right_col": "node.uid"}
      ],
      "conditions" : [
        {"type": "where", "column": "users.uid", "operator": "e", "value": "1"}
      ],
      "columns" : [
        {"column": "node.nid"},
        {"column": "node.uid"},
        {"column": "node.created"},
        {"column": "node.status"},
        {"column": "users.mail"},
        {"column": "users.name"}
      ],
      "range": {"min": "0", "max": "10"},
      "group": [{"column": "node.nid"}],
      "order": [{"type": "column", "column": "node.created", "direction": "DESC"}],
    },
    "settings" : {
      "access_token": "XQb4kr4XefNuRcqMRvHyy72ZRwBkEVYP",
      "cache_time": "60", "allowed_roles": {"3": "3"},
      "allowed_formats": ["table", "csv", "pipe", "json"]
    },
    "description" : "Description for the nodes report.",
    "tags" : [ "tag1", "tag2" ]
  }');
  return $items;
}