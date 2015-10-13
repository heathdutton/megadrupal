<?php
/**
 * @file
 * Field handler to present a link node edit.
 */

class pm_handler_field_operation extends views_handler_field_node_link {
  /**
   * Defines views field options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['display_add_icon'] = array('default' => TRUE);
    $options['display_icons'] = array('default' => TRUE);
    return $options;
  }

  /**
   * Defines views field option form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['display_icons'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display icon links'),
      '#description' => t('Display a icon or a text link for edit, delete and add node links.'),
      '#default_value' => $this->options['display_icons'],
    );
    $form['display_add_icon'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display a node add icon in the views header?'),
      '#default_value' => $this->options['display_add_icon'],
    );
  }

  /**
   * Renders field to show icon.
   */
  function render($values) {
    $id = $values->nid;
    $node = new stdClass();
    $node->nid = $values->nid;
    $node->status = 1;
    if ($this->options['display_icons']) {
      $value = "";
      $value .= pm_icon_edit_node($node, $_GET);
      if (!empty($value)) {
        $value .= '&nbsp;';
      }
      $value .= pm_icon_delete_node($node, $_GET);

      return $value;
    }
    else {
      $links = array();
      if ($link = $this->create_link_if_valid(t('edit'), "node/$id/edit")) {
        $links[] = $link;
      }
      if ($link = $this->create_link_if_valid(t('delete'), "node/$id/delete")) {
        $links[] = $link;
      }
      $value = empty($links) ? '' : implode('&nbsp;|&nbsp;', $links);

      $output = '';
      $links = array();
      if ($link = $this->create_link_definition(t('view'), "node/$id")) {
        $links[] = $link;
      }
      $vars = array(
        'title' => t('Quick Links'),
        'links' => $links,
      );
      return theme('ctools_dropdown', $vars);
    }
  }

  /**
   * check if path is valid and create links. returns false otherwise.
   */
  function create_link_if_valid($title, $path) {
    if (drupal_valid_path($path)) {
      return l($title, $path, array('query' => drupal_get_destination()));
    }
    return FALSE;
  }

  /**
   * check if path is valid and create links. returns false otherwise.
   */
  function create_link_definition($title, $path) {
    $link = FALSE;
    if (drupal_valid_path($path)) {
      $link = array(
        'title' => $title,
        'href' => $path
      );
    }
    return $link;
  }
}
