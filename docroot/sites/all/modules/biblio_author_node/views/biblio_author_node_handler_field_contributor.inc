<?php

/**
 * @file
 * Custom contributor rendering.
 */

/**
 * Custom biblio_handler_field_contributor implementation.
 */
class biblio_author_node_handler_field_contributor extends biblio_handler_field_contributor {

  /**
   * Custom render_contriubutors implementation to display authors as links if
   * they are linked to nodes or users.
   */
  function render_contriubutors($contributors) {

    $authors_out = array();

    $authors = parent::render_contriubutors($contributors);
    $authors_array = explode($this->options['separator'], $authors);

    foreach ($authors_array as $index => $name) {

      $author = (object) $contributors[$index];

      if (!empty($author->drupal_nid)) {
        $link = base_path() . drupal_get_path_alias('node/' . $author->drupal_nid);
        $link_pre = '<a href="' . $link . '">';
        $link_post = '</a>';
      }
      elseif (!empty($author->drupal_uid)) {
        $link = base_path() . drupal_get_path_alias('user/' . $author->drupal_uid);
        $link_pre = '<a href="' . $link . '">';
        $link_post = '</a>';
      }
      elseif (!empty($author->cid)) {
        $link = base_path() . variable_get('biblio_base', 'bibliography') . '?f[author]=' . $author->cid;
        $link_pre = '<a href="' . $link . '">';
        $link_post = '</a>';
      }
      else {
        $link_pre = '';
        $link_post = '';
      }

      $authors_out[] = $link_pre . $name . $link_post;
    }

    return implode($this->options['separator'], $authors_out);
  }

}
