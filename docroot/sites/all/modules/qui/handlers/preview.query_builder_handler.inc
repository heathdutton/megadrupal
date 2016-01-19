<?php
/**
 * @file
 * Contains the Preview handler.
 */

/**
 * Class Qui_Preview_Handler
 */
class Qui_Preview_Handler extends Qui_Query_Builder_Handler {

  public $name = 'preview';

  /**
   * Enable Preview if the select table is set.
   */
  public function preBuild() {
    if (!empty($this->getHandler('qui_select_table')->defaults)) {
      $this->enabled = TRUE;
    }
  }

  /**
   * Create the preview elements.
   */
  public function build() {
    $this->elements['actions'][$this->name] = array(
      '#type' => 'button',
      '#value' => t('Preview'),
      '#weight' => 2,
      '#name' => 'preview',
      '#button_action' => 'generatePreview',
      '#validate' => array(),
      '#ajax' => array(
        'callback' => '_qui_form_preview_results',
        'wrapper' => 'preview-results',
        'method' => 'replace',
      ),
      '#states' => $this->getHandler('qui_select_table')->select_table_state,
    );
    $this->elements['actions']['#suffix'] = '<div id="preview-results"></div>';
  }

  /**
   * Generate the query preview.
   */
  public function generatePreview() {

    // Call submit handler to build the report object.
    $this->controller->runHandlerMethods('submit');

    $report = $this->report;
    $output = '<div id="preview-results">';
    if (form_get_errors()) {
      $output .= '</div>';
      return $output;
    }

    // Set range for preview.
    $report->query->range['min'] = 0;
    if (!$report->query->row_count) {
      $report->query->range['max'] = variable_get('qui_preview_limit', 5);
    }

    if (variable_get('qui_show_braces')) {
      $query_string = $report->__toString();
    }
    else {
      $query_string = $report->queryString();
    }

    $query_string = '<pre>' . $query_string . '</pre>';

    if ($report->query->row_count) {
      // Remove range limitations for row_count and preview.
      $this->report->range['min'] = NULL;
      $this->report->range['max'] = NULL;
      $output .= t('<h2>Row count: !results</h2>', array('!results' => $report->runQuery()));
    }
    else {
      // Standard display output.
      $output .= $this->previewResults($report, $query_string);
    }

    $output .= '</div>';
    return $output;
  }

  /**
   * Get the preview results for a query.
   */
  private function previewResults($report, $query_string) {
    $output = '';

    try {
      $results = $report->runQuery();
    }
    catch (PDOException $e) {
      // Must immediately return in this catch for the query to be printed.
      $output .= $this->previewError($e->getMessage(), $query_string);
      return $output;
    }
    $result_count = count($results);
    $count = format_plural($result_count,
      'Displaying !count result.',
      'Displaying !count results.',
      array('!count' => $result_count));
    $output .= '<a href="#" id="qui-query-preview-toggle">Show Query</a>';
    $output .= '<div class="qui-query-preview">';
    $output .= $query_string;
    $output .= '</div>';
    $output .= "<h3>$count</h3>";
    $output .= qui_get_formatted_results($report, $results, 'table');
    return $output;
  }

  /**
   * Formats an error message for display in previews.
   *
   * @param string $error
   *   An error message.
   * @param string $query_string
   *   The executed query string.
   *
   * @return string
   *   An HTML string suitable for displaying the preview and error message.
   */
  private function previewError($error, $query_string) {
    $output = '<h2>' . t('Error:') . '</h2>';
    $output .= '<div class="qui-error">' . $error . '</div>';
    $output .= '<h2>' . t('Query attempted:') . '</h2>';
    $output .= '<div class="qui-query-preview">';
    $output .= $query_string;
    $output .= '</div>';
    // End #preview-results div.
    $output .= '</div>';
    return $output;
  }

}
