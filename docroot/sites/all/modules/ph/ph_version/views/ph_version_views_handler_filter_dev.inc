<?php
/**
 * @file
 * Dev filter implementation.
 */

/**
 * Views filter handler for version:is_dev.
 */
class ph_version_views_handler_filter_dev extends views_handler_filter_boolean_operator {
  /**
   * Assumes able to build group regardless of operators.
   */
  function can_build_group() {
    return $this->is_exposed();
  }

  /**
   * Conditionally returns operators depending on whether building group.
   */
  function operator_options() {
    if ($this->is_a_group()) {
      return array(
        'is' => t('Is'),
      );
    }
    else {
      return array();
    }
  }

  /**
   * Builds operator form.
   */
  function operator_form(&$form, &$form_state) {
    views_handler_filter::operator_form($form, $form_state);
  }

  function accept_exposed_input($input) {
    if (parent::accept_exposed_input($input)) {
      $value = $input[$this->options['expose']['identifier']];
      if ($this->is_a_group() && isset($this->options['group_info']['group_items'][$value])) {
        $value = $this->options['group_info']['group_items'][$value]['value'];
      }
      $this->value = $value;

      return TRUE;
    }

    return FALSE;
  }

  function query() {
    $this->ensure_my_table();

    // Prepare column names.
    $field_name = $this->definition['field_name'];
    $column_patch_name = $field_name . '_patch';
    $column_increment_name = $field_name . '_increment';
    $field_patch = "$this->table_alias.$column_patch_name";
    $field_increment = "$this->table_alias.$column_increment_name";

    if (empty($this->value)) {
      $and = db_and()
        ->isNotNull($field_patch)
        ->isNull($field_increment);
      $this->query->add_where($this->options['group'], $and);
    }
    else {
      $or = db_or()
        ->isNull($field_patch)
        ->isNotNull($field_increment);
      $this->query->add_where($this->options['group'], $or);
    }
  }
}
