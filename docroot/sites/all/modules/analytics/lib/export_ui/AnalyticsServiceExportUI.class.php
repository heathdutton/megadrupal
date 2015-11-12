<?php

class AnalyticsServiceExportUI extends ctools_export_ui {

  function access($op, $item) {
    // Only can add or import new services if there are available services that
    // can be added.
    if (in_array($op, array('add', 'import')) && !analytics_service_add_service_options()) {
      return FALSE;
    }

    // Disable cloning entirely.
    if ($op == 'clone') {
      return FALSE;
    }

    if (!empty($item->locked)) {
      return FALSE;
    }

    return parent::access($op, $item);
  }

  function build_operations($item) {
    $operations = parent::build_operations($item);
    foreach ($operations as $op => $operation) {
      if ($op == 'enable' || $op == 'disable') {
        continue;
      }
      elseif (!$this->access($op, $item)) {
        unset($operations[$op]);
      }
    }
    return $operations;
  }

  /*function list_table_header() {
    $header = array();
    if (!empty($this->plugin['export']['admin_title'])) {
      $header[] = array(
        'data' => t('Label'),
        'class' => array('ctools-export-ui-title'),
      );
    }

    $header[] = array(
      'data' => t('Machine name'),
      'class' => array('ctools-export-ui-name'),
    );
    $header[] = array(
      'data' => t('Storage'),
      'class' => array('ctools-export-ui-storage'),
    );
    $header[] = array(
      'data' => t('Operations'),
      'class' => array('ctools-export-ui-operations'),
    );

    return $header;
  }*/

  /*function list_build_row($item, &$form_state, $operations) {
    parent::list_build_row($item, $form_state, $operations);
    $name = $item->{$this->plugin['export']['key']};
    $this->rows[$name]['data'][3]['data'] = theme('links', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));
  }*/
}
