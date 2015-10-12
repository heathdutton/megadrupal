<?php
// $Id: mm_renderer.inc 5381 2011-05-03 17:05:32Z dwilga $

/**
 * @file
 * Class used by mm_content_render_tree().
 */

class mmRenderer {
  public function __construct() {
    // To avoid having to sort, this array must be in increasing order
    $this->state_to_css = array(     // Do not translate these with t()
      MM_GET_TREE_STATE_COLLAPSED => 'collapsed',
      MM_GET_TREE_STATE_DENIED =>    'denied',
      MM_GET_TREE_STATE_EXPANDED =>  'expanded',
      MM_GET_TREE_STATE_HERE =>      'here',
      MM_GET_TREE_STATE_HIDDEN =>    'hidden-entry',
      MM_GET_TREE_STATE_LEAF =>      'leaf',
      MM_GET_TREE_STATE_NOT_WORLD => 'not-world',
      MM_GET_TREE_STATE_RECYCLE =>   'recycle-bin',
    );
    $this->state_cache = array();
    $keys = array_keys($this->state_to_css);
    $this->state_max = array_pop($keys);
  }

  public function preprocess($tree) {
    // Speed up future calls to mm_content_get() by pre-fetching everything all
    // at once (well, up to 50 at once.)
    foreach (array_chunk($tree, 50) as $chunk) {
      $mmtids = array();
      foreach ($chunk as $item) {
        $mmtids[] = $item->mmtid;
      }
      mm_content_get($mmtids, MM_GET_PARENTS);
    }
  }

  public function state_class($leaf) {
    if (!isset($this->state_cache[$leaf->state])) {
      $state = $leaf->state;
      $state_css = array();
      $i = 0;
      $bit = 0;
      while ($state && $bit != $this->state_max) {
        $bit = 1 << $i++;
        if ($state & $bit && isset($this->state_to_css[$bit])) {
          $state_css[] = $this->state_to_css[$bit];
          $state ^= $bit;
        }
      }
      $this->state_cache[$leaf->state] = join(' ', $state_css);
    }
    return $this->state_cache[$leaf->state];
  }

  public function prefix_all() {
    return "<ul class=\"menu\">";
  }

  public function suffix_all() {
    return '</ul>';
  }

  public function prefix_leaf($leaf, $item, $link_item) {
    return '<li class="' . $this->state_class($leaf) . '">';
  }

  public function link_leaf($leaf, $item, $link_item) {
//    if( $leaf->nodecount === '0' ) $item['title'] .= t(' [no pages]');
    $atts = !empty($item['description']) ? array('title' => $item['description']) : array();

    if ($link_item['path'] != $_GET['q'] &&
        ($leaf->state & (MM_GET_TREE_STATE_EXPANDED|MM_GET_TREE_STATE_HERE)) == (MM_GET_TREE_STATE_EXPANDED|MM_GET_TREE_STATE_HERE))
      $atts['class'] = array('active');

    if ($leaf->state & MM_GET_TREE_STATE_HIDDEN) $item['title'] .= ' ' . t('(hidden)');

    if ($leaf->state & MM_GET_TREE_STATE_DENIED) $atts['rel'] = 'nofollow';

    return l($item['title'], $link_item['path'], array('attributes' => $atts, 'query' => isset($item['query']) ? $item['query'] : array()));
  }

  public function suffix_leaf($leaf, $item, $link_item) {
    return "</li>\n";
  }

  public function prefix_parent($leaf, $item) {
    return "\n<ul class=\"menu\">";
  }

  public function suffix_parent($leaf, $item) {
    return "</ul>\n";
  }

  public function is_visible($leaf) {
    if ($leaf->name == MM_ENTRY_NAME_RECYCLE) {
      return mm_content_user_can_recycle($leaf->mmtid, MM_PERMS_READ);
    }

    if (isset($leaf->perms) && ($leaf->perms[MM_PERMS_WRITE] || $leaf->perms[MM_PERMS_SUB] || $leaf->perms[MM_PERMS_APPLY])) {
      return TRUE;
    }

    return !variable_get('mm_hide_empty_pages', FALSE) || $leaf->nodecount !== '0';  // returns true if $leaf->nodecount is not set
  }
}
