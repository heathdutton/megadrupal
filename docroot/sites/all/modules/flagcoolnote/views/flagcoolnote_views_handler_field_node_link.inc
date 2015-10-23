<?php

/**
 * @file
 *   Views handler to display links to flagged node.
 */

/**
 * Field handler to present a link to the user.
 */
class flagcoolnote_views_handler_field_node_link extends views_handler_field {
  var $link_type;

  function construct() {
    // We need to set this property before calling the construct() chain
    // as we use it in the option_definintion() call.
    $this->link_type = $this->definition['link_type'];

    parent::construct();
    $this->additional_fields['nid'] = 'nid';

  }

  function allow_advanced_render() {
    return FALSE;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['label'] = array('default' => '', 'translatable' => TRUE);
    $options['text'] = array('default' => $this->link_type, 'translatable' => TRUE);
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

    // Join to the node table to retrieve the node UID.
    $join = new views_join();
    $join->construct('node', $this->table_alias, 'nid', 'nid');
    $this->query->ensure_table('node', $this->relationship, $join);

    $this->add_additional_fields();
  }

  function render($values) {
    $flagged_node = new stdClass();
    $flagged_node->nid = $values->{$this->aliases['nid']};

    switch ($this->link_type) {
      case 'view':
        if (!isset($flagged_node->nid)) {
          return FALSE;
        }
        $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
        $link = l($text, "node/$flagged_node->nid");
        $access = node_access('view', $flagged_node);
        break;
      default:
        $text = '';
        $link = NULL;
        $access = FALSE;
    }

    if (!$access) {
      return;
    }

    return $link;
  }
}
