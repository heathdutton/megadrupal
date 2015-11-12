<?php

class views_menu_children_argument extends views_handler_argument_numeric {

  /**
   * The operator used for the query: or|and.
   * @var string
   */
  var $operator;

  /**
   * The actual value which is used for querying.
   * @var array
   */
  var $value;

  function option_definition() {
    $options = parent::option_definition();

    $options['target_menu'] = array(
      'default' => ''
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    foreach (menu_load_all() as $menu) {
      $menus[$menu['menu_name']] = $menu['title'];
    }
    asort($menus);

    unset($form['not']);
    unset($form['break_phrase']);

    $form['target_menu'] = array(
      '#type' => 'select',
      '#title' => t('Target menu'),
      '#description' => t('Select the menu to scan for child nodes.'),
      '#default_value' => $this->options['target_menu'],
      '#required' => TRUE,
      '#options' => array_merge(array('' => t('-- Select menu --')), $menus),
    );
  }

  function title() {
    return parent::title();
  }

  /**
   * Override for specific title lookups.
   * @return array
   *    Returns all titles, if it's just one title it's an array with one entry.
   */
  function title_query() {
    return $this->value;
  }

  function query($group_by = FALSE) {
    $this->ensure_my_table();

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    $placeholder = $this->placeholder();
    $null_check = empty($this->options['not']) ? '' : "OR $this->table_alias.$this->real_field IS NULL";

    $target_nid = reset($this->value);
    if (!empty($target_nid)) {
      // Get the node details
      $child_nodes = self::get_child_node_nids($target_nid, $this->options['target_menu']);

      if (count($this->value) >= 1) {
        $operator = empty($this->options['not']) ? 'IN' : 'NOT IN';
        $this->query->add_where_expression(
          0,
          "$this->table_alias.nid $operator($placeholder) $null_check"
          , array($placeholder => $child_nodes));
      }
    }
  }

  /**
   * Gets the direct decedents of a node as structured in the target menu.
   *
   * @param integer $parent_nid
   * @param string $menu_name
   * @return array|null
   */
  static function get_child_node_nids($parent_nid, $menu_name) {
    $nids = array();
    $parent = menu_link_get_preferred('node/' . $parent_nid, $menu_name);
    $parameters = array(
      'active_trail' => array($parent['plid']),
      'only_active_trail' => FALSE,
      'min_depth' => $parent['depth'] + 1,
      'max_depth' => $parent['depth'] + 1,
      'conditions' => array('plid' => $parent['mlid']),
    );

    $children = menu_build_tree($parent['menu_name'], $parameters);
    if (!empty($children)) {
      foreach ($children as $menu_link) {
        $nids[] = str_replace('node/', '', $menu_link['link']['link_path']);
      }
      return $nids;
    }

    return NULL;
  }
}
