<?php

/**
 * @file
 * SSH key field handler for edit links.
 */

class sshkey_handler_field_sshkey_operations extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['key_id'] = 'key_id';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['view_text'] = array('default' => '', 'translatable' => TRUE);
    $options['edit_text'] = array('default' => '', 'translatable' => TRUE);
    $options['delete_text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['view_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display for view links'),
      '#default_value' => $this->options['view_text'],
    );
    $form['edit_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display for edit links'),
      '#default_value' => $this->options['edit_text'],
    );
    $form['delete_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display for delete links'),
      '#default_value' => $this->options['delete_text'],
    );
  }

  function query() {
    $base_table = $this->view->base_table;

    // If the current field is under a relationship you can't be sure that the
    // base table of the view is the base table of the current field.
    // For example a field from a node author on a node view does have users as base table.
    if (!empty($this->relationship)) {
      foreach ($this->view->relationship as $relationship) {
        if ($relationship->alias == $this->relationship) {
          $base_table = $relationship->definition['base'];
        }
      }
    }

    // Get the entity type according to the base table of the field.
    $this->entity_type = $this->definition['entity_tables'][$base_table];
    $entity_info = entity_get_info($this->entity_type);

    $this->additional_fields['entity_id'] = array(
      'table' => $base_table,
      'field' => $entity_info['entity keys']['id'],
    );

    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $key_id = $values->{$this->aliases['key_id']};
    $entity_id = $values->{$this->aliases['entity_id']};
    $key = sshkey_load($key_id);
    $destination = drupal_get_destination();

    $operations = array();
    if (sshkey_access('view', $this->entity_type, $entity_id, $keyy)) {
      $operations['view'] = array(
        'title' => !empty($this->options['view_text']) ? $this->options['view_text'] : t('View'),
        'href' => "ssh-keys/{$this->entity_type}/{$entity_id}/{$key_id}/view",
      );
    }
    if (sshkey_access('edit', $this->entity_type, $entity_id, $key)) {
      $operations['edit'] = array(
        'title' => !empty($this->options['edit_text']) ? $this->options['edit_text'] : t('Edit'),
        'href' => "ssh-keys/{$this->entity_type}/{$entity_id}/{$key_id}/edit",
        'query' => $destination,
      );
    }
    if (sshkey_access('delete', $this->entity_type, $entity_id, $key)) {
      $operations['delete'] = array(
        'title' => !empty($this->options['delete_text']) ? $this->options['delete_text'] : t('Delete'),
        'href' => "ssh-keys/{$this->entity_type}/{$entity_id}/{$key_id}/delete",
        'query' => $destination,
      );
    }

    if (!empty($operations)) {
      return theme('links', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline', 'nowrap'))));
    }
    else {
      return '';
    }
  }
}
