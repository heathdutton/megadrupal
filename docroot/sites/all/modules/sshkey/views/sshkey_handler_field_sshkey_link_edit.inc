<?php

/**
 * @file
 * SSH key field handler for edit links.
 */

class sshkey_handler_field_sshkey_link_edit extends sshkey_handler_field_sshkey_link {
  function render($values) {
    $key_id = $values->{$this->aliases['key_id']};
    $entity_id = $values->{$this->aliases['entity_id']};

    if (($key = sshkey_load($key_id)) && sshkey_access('edit', $this->entity_type, $entity_id, $key)) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "ssh-keys/{$this->entity_type}/{$entity_id}/{$key_id}/edit";
      $this->options['alter']['query'] = drupal_get_destination();

      $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit');
      return $text;
    }

    return '';
  }
}
