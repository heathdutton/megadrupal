<?php

/**
 * @file
 * Fortissimmo attachment file Views handler
 */
class views_handler_field_fortissimmo_file extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);
    $field = $options['field'];
    $prefix = str_replace('_file', '', $field);
    $this->additional_fields = array(
      $prefix . '_typevalue' => array('table' => $view->base_table, 'field' => $prefix . '_typevalue'),
      $prefix . '_comment' => array('table' => $view->base_table, 'field' => $prefix . '_comment'),
    );
  }

  function render($values) {
    $options = $this->options;
    $field = $options['field'];
    $prefix = str_replace('_file', '', $field);

    if (isset($this->aliases[$prefix . '_typevalue'])) {
      $filetype = $values->{$this->aliases[$prefix . '_typevalue']};
    }
    if (isset($this->aliases[$prefix . '_comment'])) {
      $comment = $values->{$this->aliases[$prefix . '_comment']};
    }

    $output = '';
    $filename = $values->{$this->field_alias};
    if ($filename) {
      $fileuri = fortissimmo_create_asset_url($filename, 'files');
      $fileurl = file_create_url($fileuri);
      if ($filetype) {
        $label = $filetype;
      }
      else {
        $label = $filename;
      }

      $output .= l($label, $fileurl);

      if ($comment) {
        $output .= ' - ' . $comment;
      }
    }

    return $output;
  }
}