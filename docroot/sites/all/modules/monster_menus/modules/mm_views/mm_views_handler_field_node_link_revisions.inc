<?php

/**
 * @file
 * Views interface for Monster Menus
 */

/**
 * Field handler to present a link to a node's revision list.
 */
class mm_views_handler_field_node_link_revisions extends views_handler_field_node_link {
  function query() {
    parent::query();
    $alias = $this->query->add_table('mm_node_revisions');
    $this->query->add_field(NULL, "COUNT($alias.nid)", 'count_revisions');
    $this->query->add_groupby("$alias.nid");
    $this->query->add_having(0, 1, 1);    // Due to a bug in views, without this the GROUP BY isn't added
  }

  function render_link($node, $values) {
    if ($values->count_revisions <= 1) {
      return;
    }

    if (!user_access('view revisions') && !user_access('bypass node access')) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "node/$node->nid/revisions";

    return !empty($this->options['text']) ? $this->options['text'] : t('revisions');
  }
}

