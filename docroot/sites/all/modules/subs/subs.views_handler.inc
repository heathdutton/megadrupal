<?php
/**
 * @file
 * Views handlers for Subs.
 */

/**
 * A handler to display subscription start and end dates.
 *
 * @ingroup views_field_handlers
 */
class subs_handler_date extends views_handler_field_date {

  function render($values) {
    if ($this->field_alias === 'subs_date_start') {
      if ($values->subs_date_start == 0) {
        return t('None');
      }
    }
    elseif ($this->field_alias === 'subs_date_end') {
      if ($values->subs_date_end == 0) {
        return t('None');
      }
    }

    return parent::render($values);
  }

}

/**
 * A handler to provide a field with a link to view the subscription.
 *
 * @ingroup views_field_handlers
 */
class subs_handler_view extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['sid'] = 'sid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $sid = $values->{$this->aliases['sid']};

    // Load the subscription entity.
    $entity = entity_load('subs', $sid);

    if (!subs_access('view', $entity)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $entity_uri = entity_uri('subs', $entity);
    return l($text, reset($entity_uri));
  }

}

/**
 * A handler to provide a field with a link to edit the subscription.
 *
 * @ingroup views_field_handlers
 */
class subs_handler_edit extends subs_handler_view {

  function render($values) {
    $sid = $values->{$this->aliases['sid']};
    $entity = entity_load('subs', $sid);
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $entity_uri = entity_uri('subs', $entity);
    return l($text, reset($entity_uri) . '/edit');
  }

}

/**
 * A handler to provide a field with a link to delete the subscription.
 *
 * @ingroup views_field_handlers
 */
class subs_handler_delete extends subs_handler_view {

  function render($values) {
    $sid = $values->{$this->aliases['sid']};
    $entity = entity_load('subs', $sid);
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $entity_uri = entity_uri('subs', $entity);
    return l($text, reset($entity_uri) . '/delete');
  }

}

/**
 * A handler to provide a field with operations links for the subscription.
 *
 * @ingroup views_field_handlers
 */
class subs_handler_operations extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['sid'] = 'sid';
    $this->additional_fields['type'] = 'type';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $sid = $values->{$this->aliases['sid']};

    $destination = drupal_get_destination();
    $operations = array();

    // Load the subscription entity.
    $entity = entity_load('subs', $sid);
    $entity_uri = entity_uri('subs', $entity);

    // Add view link if user has permsission.
    if (subs_access('view', $entity)) {
      $operations['view'] = array(
        'title' => t('view'),
        'href' => $entity_uri,
        'destination' => $destination,
      );
    }

    // Add edit link if user has permsission.
    if (subs_access('edit', $entity)) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => reset($entity_uri) . '/edit',
        'destination' => $destination,
      );
    }

    // Add delete link if user has permsission.
    if (subs_access('delete', $entity)) {
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => reset($entity_uri) . '/delete',
        'destination' => $destination,
      );
    }

    return array(
      '#theme' => 'links__subs_operations',
      '#links' => $operations,
      '#attributes' => array('class' => array('links', 'inline')),
    );
  }

}