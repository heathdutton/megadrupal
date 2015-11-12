<?php

/**
 * @file
 * Views extend file for Smart Chart module.
 */

/**
 * Implements hook_views_data().
 */
function smart_chart_views_data() {

  // Field handler for item title on Smart Chart.
  $data['node']['smart_item_title'] = array(
    'field' => array(
      'title' => t('Smart Item title'),
      'help' => t('The title of the item on the chart.'),
      'handler' => 'views_handler_field_smart_chart_views_functions_title',
    ),
  );

  // Ensure Smart Chart item type is one of selected content types.
  $data['node']['smart_chart_item_type'] = array(
    'title' => t('Smart Chart item type'),
    'help' => t('Ensure Smart Chart item type is one of selected content types.'),
    'filter' => array(
      'field' => 'status',
      'handler' => 'views_handler_filter_node_smart_chart_item_type',
      'label' => t('Smart Chart item type'),
    ),
  );

  return $data;
}


/**
 * Field handler for item title on Smart Chart.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_smart_chart_views_functions_title extends views_handler_field {

  function construct() {
    parent::construct();
  }

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if (isset($this->view->args) && count($this->view->args) > 1) {
      $href = 'chart/add/nojs/' . $this->view->args[0] . '/' . $this->view->args[1] . '/' . $values->nid;
      $item_add = ctools_modal_text_button($values->node_title, $href, t('Add') . ' ' . $values->node_title, 'ctools-modal-common-modal-style');
      return $item_add;
    }
    else {
      return '[placeholder]';
    }
  }
}

/**
 * Ensure Smart Chart item type is one of selected content types.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_node_smart_chart_item_type extends views_handler_filter {

  function admin_summary() {

  }

  function operator_form(&$form, &$form_state) {

  }

  function can_expose() {
    return FALSE;
  }

  function query() {
    $table = $this->ensure_my_table();

    // Get selected conntent types.
    $file_path = module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');
    $types_selected = sc_get_chart_item_types();

    $this->query->add_where($this->options['group'], $table . '.type', $types_selected, 'in');
  }
}
