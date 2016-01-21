<?php

/**
 * UI class for Stylizer.
 */
class raw_sql_query_ui extends ctools_export_ui {

  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$item->name] = empty($item->disabled);
        break;
      case 'title':
        $this->sorts[$item->name] = $item->admin_title;
        break;
      case 'menu_path':
        $this->sorts[$item->name] = $item->menu_path;
        break;
      case 'tablename':
        $this->sorts[$item->name] = $item->tablename;
        break;
      case 'fields':
        $this->sorts[$item->name] = $item->tablename;
        break;
      case 'storage':
        $this->sorts[$item->name] = $item->type . $item->admin_title;
        break;
    }


    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$item->name] = array(
      'data' => array(
        array('data' => check_plain($item->name), 'class' => array('ctools-export-ui-name')),
        array('data' => l($item->menu_path, $item->menu_path, array('attributes' => array('target' => '_blank'))), 'class' => array('ctools-export-ui-menu-path')),
        array('data' => l($item->tablename, "rsq/download/" . $item->tablename), 'class' => array('ctools-export-ui-tablename')),
        array('data' => check_plain(str_replace(',0', '',$item->fields)), 'class' => array('ctools-export-ui-fields')),
        array('data' => check_plain($item->type), 'class' => array('ctools-export-ui-storage')),
        array('data' => $ops, 'class' => array('ctools-export-ui-operations')),
      ),
      'title' => check_plain($item->name),
      'class' => array(!empty($item->disabled) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled'),
    );
  }

  function list_table_header() {
    return array(
      array('data' => t('Name'), 'class' => array('ctools-export-ui-name')),
      array('data' => t('Menu path'), 'class' => array('ctools-export-ui-menu-path')),
      array('data' => t('Download'), 'class' => array('ctools-export-ui-tablename')),
      array('data' => t('Fields'), 'class' => array('ctools-export-ui-fields')),
      array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage')),
      array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations')),
    );
  }


}
