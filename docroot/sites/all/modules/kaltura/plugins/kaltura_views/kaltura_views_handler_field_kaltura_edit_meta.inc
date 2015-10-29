<?php

/**
 * @file
 * Class kaltura_views_handler_field_kaltura_edit_meta.
 */

/**
 * Edit Meta field handler class.
 */
class kaltura_views_handler_field_kaltura_edit_meta extends views_handler_field {
  function pre_render(&$values) {
    ctools_include('modal');
    ctools_include('ajax');
    ctools_modal_add_js();
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    drupal_add_js(array(
      'metadata-modal-style' => array(
          'modalSize' => array(
          'type' => 'scale',
          'width' => .4,
          'height' => .6,
        ),
      ),
    ), 'setting');
    return ctools_modal_text_button('Edit Metadata', 'kaltura/nojs/metadata_editor/' . $value, 'Edit Metadata', 'ctools-modal-metadata-modal-style');
  }
}
