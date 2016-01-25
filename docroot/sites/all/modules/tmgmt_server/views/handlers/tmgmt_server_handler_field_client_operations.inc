<?php

/**
 * Field handler to display DS Job operations.
 *
 * @ingroup views_field_handlers
 */
class tmgmt_server_handler_field_client_operations extends views_handler_field_entity {

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['uid'] = 'uid';
    $this->additional_fields['cid'] = 'cid';
  }

  function render($values) {

    $uid = $values->{$this->aliases['uid']};
    $cid = $values->{$this->aliases['cid']};

    $element = array();
    $element['#theme'] = 'links';
    $element['#attributes'] = array('class' => array('inline'));

    $element['#links']['manage'] = array(
      'href' => 'user/' . $uid . '/remote-clients/' . $cid,
      'title' => t('manage'),
      'query' => array('destination' => 'user/' . $uid . '/remote-clients'),
    );

    return drupal_render($element);
  }
}
