<?php

/**
 * Field handler for terms.
 *
 * This code is all taken from views_handler_field_term_node_tid.inc
 */

/**
 * Class views_handler_field_term_regcode_tid.
 */
class views_handler_field_term_regcode_tid extends views_handler_field_prerender_list {

  /**
   * Implement option_definition.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_taxonomy'] = array('default' => TRUE);
    $options['limit'] = array('default' => FALSE);
    $options['vids'] = array('default' => array());

    return $options;
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its term page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );
  }

  /**
   * Add this term to the query.
   */
  function query() {
    $this->add_additional_fields();
  }

  /**
   * Attaches taxonomy term data (this is called only once)
   * 
   * @param $values
   *   An array of raw values.
   */
  function pre_render(&$values) {
    $this->aliases['vid'] = 'rid';
    $this->field_alias = $this->aliases['vid'];
    $vids = array();
    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }

    if ($vids) {
      $result = db_query("
      SELECT tn.rid AS node_vid, td.*, v.name as vocabulary
      FROM {taxonomy_term_data} td
      INNER JOIN {regcode_term} tn ON td.tid = tn.tid
      INNER JOIN {taxonomy_vocabulary} v ON v.vid = td.vid
      WHERE tn.rid IN (:rids) AND td.vid = :vid
      ORDER BY td.weight, td.name",
        array(
          ':rids' => $vids,
          ':vid' => variable_get('regcode_vocabulary', 1),
        )
      );

      foreach ($result as $term) {
        $this->items[$term->node_vid][$term->tid]['name'] = check_plain($term->name);
        $this->items[$term->node_vid][$term->tid]['tid'] = $term->tid;
        $this->items[$term->node_vid][$term->tid]['vid'] = $term->vid;
        $this->items[$term->node_vid][$term->tid]['taxonomy_vocabulary'] = check_plain($term->vocabulary);

        if (!empty($this->options['link_to_taxonomy'])) {
          $this->items[$term->node_vid][$term->tid]['make_link'] = TRUE;
          $this->items[$term->node_vid][$term->tid]['path'] = taxonomy_term_uri($term);
        }
      }
    }
  }

  /**
   * Renders the item.
   */
  function render_item($count, $item) {
    return $item['name'];
  }

}
