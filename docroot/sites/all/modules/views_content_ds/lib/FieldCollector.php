<?php


class views_content_ds_FieldCollector {

  protected $fields = array();
  protected $entityTypesMap = array();

  function __construct() {
    foreach (entity_get_info() as $entity_type => $info) {
      $primary = $info['entity keys']['id'];
      $this->entityTypesMap["entity:$entity_type.$primary"] = $entity_type;
    }
  }

  function fields() {
    return $this->fields;
  }

  function viewsPane($arg_options, $display) {
    $c = str_replace('-', '_', $arg_options['context']);
    $entity_type = @$this->entityTypesMap[$c];
    if (isset($entity_type)) {
      $this->_viewsPane($entity_type, $display);
    }
    elseif ($c === 'entity:group.entity_id') {
      $this->_viewsPane('node', $display);
    }
  }

  protected function _viewsPane($entity_type, $display) {
    $fieldname = $this->_field($entity_type, $display->handler->view);
    $this->_formatter($entity_type, $fieldname, $display);
  }

  protected function _field($entity_type, $view) {
    $view_id = $view->name;
    $view_nicename = t($view->human_name);
    $fieldname = "views_content_ds__$view_id";
    $title = t('Views: !field_name', array('!field_name' => $view_nicename));
    if (!isset($this->fields[$entity_type][$fieldname])) {
      $this->fields[$entity_type][$fieldname] = array(
        'title' => $title,
        'field_type' => DS_FIELD_TYPE_FUNCTION,
        'function' => '_views_content_ds_field_print',
        // These two settings have no meaning for ds api,
        // we just need them in the callback.
        '_view_id' => $view_id,
        // We could restrict to specific bundles and view modes, if we wanted.
        'ui_limit' => array('*|*'),
      );
    }
    return $fieldname;
  }

  protected function _formatter($entity_type, $fieldname, $display) {
    $formatter_name = $display->id;
    $formatter_title = t($display->display_title);
    $this->fields[$entity_type][$fieldname]['properties']['formatters'][$formatter_name] = $formatter_title;
  }
}
