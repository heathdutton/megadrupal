<?php


class nodeaspect_PageInfoCache {

  protected $pages = array();
  protected $active;

  function reset() {
    $this->pages = array();
  }

  function pageInfo($node, $suffix) {
    if (!isset($this->pages[$node->nid][$suffix])) {
      $this->pages[$node->nid][$suffix] = _nodeaspect_page_info_build($node, $suffix);
    }
    return $this->pages[$node->nid][$suffix];
  }

  function page($node, $suffix) {
    $page_info = $this->pageInfo($node, $suffix);
    $this->active = array($page_info, $node, $suffix);
    return $page_info->page($node);
  }

  function hook_pageapi($api) {
    if (!empty($this->active)) {
      list($page_info, $node, $suffix) = $this->active;
      $page_info->hook_pageapi($api, $node, $suffix);
    }
  }

  function hook_preprocess_page(&$vars) {

    // Rearrange tabs.
    $weights = array();
    foreach ($this->pages as $nid => $pages) {
      foreach ($pages as $suffix => $info) {
        $weight = $info->localTaskWeight();
        $weights["node/%/$suffix"] = $weight;
      }
    }
    foreach (array('#primary', '#secondary') as $k) {
      if (is_array($vars['tabs'][$k])) {
        // stable sort, please.
        $sorted = array();
        foreach ($vars['tabs'][$k] as $i => $item) {
          $path = $item['#link']['path'];
          $weight = isset($weights[$path]) ? $weights[$path] : $item['#link']['weight'];
          if ($weight === FALSE) {
            unset($vars['tabs'][$k][$i]);
          }
          else {
            $sorted[$weight][] = $item;
          }
        }
        ksort($sorted);
        $vars['tabs'][$k] = array();
        foreach ($sorted as $items) {
          foreach ($items as $item) {
            $vars['tabs'][$k][] = $item;
          }
        }
      }
    }

    // Specific stuff for the active page.
    if (!empty($this->active)) {
      list($page_info, $node, $suffix) = $this->active;
      $page_info->hook_preprocess_page($vars, $node, $suffix);
    }
  }
}
