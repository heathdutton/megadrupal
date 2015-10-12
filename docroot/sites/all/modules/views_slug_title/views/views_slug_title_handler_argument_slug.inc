<?php
/**
 * @file
 * Views Slug Title views argument handler.
 */

/**
 * Views Slug Title argument handler.
 */
class ViewsSlugTitleHandlerArgumentSlug extends views_handler_argument_string {

  /**
   * The really title of the node for the breadcrumb and title page.
   *
   * @return string
   *   return the title of node converted with argument
   */
  public function title() {
    $nid = 0;
    $query = db_select('views_slug_title', 's');
    $query->join('node', 'n', 'n.nid = s.entity_id');
    $query->condition('s.slug', $this->argument);
    $query->fields('n', array('nid'));
    $result = $query->execute();
    while ($record = $result->fetchAssoc()) {
      $nid = $record['nid'];
    }

    if ($node = node_load($nid)) {
      return $node->title;
    }
  }

}
