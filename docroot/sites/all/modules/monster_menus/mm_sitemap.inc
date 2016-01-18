<?php
/**
 * @file
 * Generate an XML sitemap based on Monster Menu permissions.
 */

/**
 * Use an iterator to generate the sitemap
 */
function mm_sitemap() {
  class dumpIter extends getTreeIterator {
    protected $fp, $file_path, $tree_path, $max_level, $exclude, $in_home;

    public function __construct($max_level) {
      $this->file_path = 'public://sitemap.xml';
      $this->fp = fopen($this->file_path . '.temp', 'w');
      if ($this->fp) {
        $this->tree_path = array();
        fwrite($this->fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n");
      }
      else {
        watchdog('mm', 'Could not create !path', array('!path' => $this->file_path . '.temp'), WATCHDOG_ERROR);
        exit();
      }
      $this->max_level = $max_level;
      $this->exclude = variable_get('mm_sitemap_exclude_list', array());
    }

    public function iterate($item) {
      global $base_url;

      // Root level.
      if (!$item->level) {
        return 1;     // No error.
      }

      if ($item->level == 1) {
        if ($item->mmtid == mm_home_mmtid()) {
          $this->tree_path = array();
          $this->in_home = 1;
        }
        else {
          $this->in_home = 0;
        }
      }

      // Not publicly readable, hidden, or recycled? Not the normal menu block?
      if ($item->level > $this->max_level + $this->in_home || !$item->perms[MM_PERMS_READ] || $item->hidden || $item->perms[MM_PERMS_IS_RECYCLED] || $item->bid > 1) {
        return -1;    // Skip children.
      }

      // Only traverse /users and the current site home.
      if ($item->level == 1 && $item->mmtid != mm_content_users_mmtid() && $item->mmtid != mm_home_mmtid()) {
        return -1;    // Skip children.
      }

      if ($item->level >= 1 + $this->in_home) {
        $node_name = empty($item->alias) ? $item->mmtid : $item->alias;
        array_splice($this->tree_path, $item->level - 1 - $this->in_home);
        $this->tree_path[] = $node_name;
      }
      $path = join('/', $this->tree_path);

      // Check the exclusions list.
      if (in_array($path, $this->exclude)) {
        return -1;    // Skip children.
      }

      // Figure out if the node or container is newer and use that date.
      // Comment this out if it is too intensive (not required for the xml spec).
      $select = db_select('mm_tree', 't');
      $select->leftJoin('mm_tree_revisions', 'r', 'r.vid = t.vid');
      $select->leftJoin('mm_node2tree', 'm', 't.mmtid = m.mmtid');
      $select->leftJoin('node', 'n', 'm.nid = n.nid');
      $select->condition('t.mmtid', $item->mmtid)
        ->groupBy('t.mmtid')
        ->groupBy('r.mtime');
      $select->addField('r', 'mtime', 't_changed');
      $select->addExpression('MAX(n.changed)', 'n_changed');
      $mod_time = $select->execute()->fetchAssoc();
      $max_mod_time = max($mod_time['t_changed'], $mod_time['n_changed']);

      fwrite($this->fp, "<url>\n");
      fwrite($this->fp, "\t<loc>" . htmlspecialchars($base_url . '/' . $path, ENT_QUOTES) . "</loc>\n");
      if (!is_null($max_mod_time)) {
        fwrite($this->fp, "\t<lastmod>" . date('Y-m-d', $max_mod_time) . "</lastmod>\n");
      }
      // fwrite($this->fp, "\t<priority>".round(1/$item->level, 2)."</priority>\n");
      fwrite($this->fp, "</url>\n");

      return 1;       // No error.
    }

    public function finish() {
      fwrite($this->fp, "</urlset>\n");
      fclose($this->fp);
      $source = $this->file_path . '.temp';   // Necessary for pass-by-reference.
      file_unmanaged_move($source, $this->file_path, FILE_EXISTS_REPLACE);
    }
  }

  $max_level = variable_get('mm_sitemap_max_level', MM_SITEMAP_MAX_LEVEL_DEFAULT);
  if ($max_level >= 0) {
    $iter = new dumpIter($max_level);
    // Use the anonymous user, so permissions tests are valid.
    $params = array(
      MM_GET_TREE_FILTER_NORMAL => TRUE,
      MM_GET_TREE_FILTER_USERS  => TRUE,
      MM_GET_TREE_ITERATOR      => $iter,
      MM_GET_TREE_RETURN_BLOCK  => TRUE,
      MM_GET_TREE_RETURN_PERMS  => TRUE,
      MM_GET_TREE_USER          => user_load(0),
    );
    mm_content_get_tree(1, $params);
    $iter->finish();
  }
  exit();
}

function mm_sitemap_show() {
  $GLOBALS['devel_shutdown'] = FALSE;
  file_transfer('public://sitemap.xml', array('Content-Type' => 'text/xml'));
}
