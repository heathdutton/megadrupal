<?php

class ctools_export_ui_example extends ctools_export_ui {
/**
* Provide the table header.
*
* If you've added columns via list_build_row() but are still using a
* table, override this method to set up the table header.
*/
function list_table_header() {
    $header = array();
    if (!empty($this->plugin['export']['admin_title'])) {
	$header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    }
    
    $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));
    
    return $header;
}

}