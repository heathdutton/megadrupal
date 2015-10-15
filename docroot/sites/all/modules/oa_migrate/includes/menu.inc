<?php

/**
 * Migrate menu links for books.
 */
class OaMenuNotebookMigration extends DrupalMenuLinks6Migration {
  /**
   * The name of the section migration.
   */
  protected $sectionMigration;

  /**
   * The node type of the notebook nodes.
   */
  protected $sourceType;

  public function __construct(array $arguments) {
    // Get the 'notebook_migration' and make sure the parent knows about it.
    $notebook_migration = $arguments['notebook_migration'];
    if (empty($arguments['node_migrations'])) {
      $arguments['node_migrations'] = array();
    }
    if (!in_array($notebook_migration, $arguments['node_migrations'])) {
      $arguments['node_migrations'][] = $notebook_migration;
    }
    parent::__construct($arguments);

    $this->sectionMigration = $arguments['section_migration'];
    $this->dependencies[] = $this->sectionMigration;

    $this->sourceType = $arguments['source_type'];
  }

  /**
   * Override query() to join against the book and OG data.
   */
  protected function query() {
    $query = parent::query();
    // Only pull in those which are books!
    $query->innerJoin('book', 'b', 'b.mlid = m.mlid');
    $query->fields('b', array('nid'));
    // Also, get the group the node belongs to as well.
    $query->innerJoin('og_ancestry', 'oga', 'oga.nid = b.nid');
    $query->fields('oga', array('group_nid'));
    // And, make sure the type is correct.
    $query->innerJoin('node', 'n', 'n.nid = b.nid');
    $query->condition('n.type', $this->sourceType);
    return $query;
  }

  /**
   * Implements Migration::prepareRow().
   */
  public function prepareRow($row) {
    // Hack to prevent our book menu links from getting ignored. By default,
    // the parent only allows primary, secondary and custom menus.
    $menu_name = $row->menu_name;
    $row->menu_name = 'menu-' . $menu_name;
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $row->menu_name = $menu_name;
  }

  /**
   * Implements Migration::prepare().
   */
  public function prepare($entity, $row) {
    $entity->menu_name = 'og-menu-single';

    // This is a top-level book, so we need to attach it to the 'Notebook'
    // section.
    if (empty($entity->plid)) {
      $section_nid = $this->handleSourceMigration(array($this->sectionMigration), $row->group_nid);
      $section_mlid = db_query("SELECT mlid FROM {menu_links} WHERE link_path = :link_path",
        array(':link_path' => 'node/' . $section_nid))->fetchField();
      if ($section_mlid) {
        $entity->plid = $section_mlid;
      }
    }
  }
}
