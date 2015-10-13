<?php

namespace Drupal\at_doc\Report;

class Views extends BaseReport {

  public $name = 'Views';

  /**
   * @TODO: Col > Access
   */
  public function process() {
    $rows = array();

    $enabled_views = views_get_enabled_views();
    // Sort by human name.
    uasort($enabled_views, function($a, $b) {
      $a_human_name = strtolower(trim($a->human_name));
      $b_human_name = strtolower(trim($b->human_name));
      if (strcmp($a_human_name, $b_human_name) < 0) {
        return -1;
      }

      // Note: don't return 0.
      // If two members compare as equal, their relative order in the sorted
      // array is undefined.
      // @see http://www.php.net/manual/en/function.uasort.php
      return 1;
    });

    foreach ($enabled_views as $view) {
      $this->buildRow($rows, $view);
    }

    return array(
        '#theme'  => 'table',
        '#header' => array('Feature', 'View', 'Tag', 'Displays', 'Empty Messages'),
        '#empty'  => t('No enabled views'),
        '#rows'   => $rows,
    );
  }

  private function buildRow(&$rows, $view) {
    $c1 = isset($view->export_module) ? $view->export_module : $this->iconError() . ' unknown';
    $c2 = "<strong>{$view->human_name}</strong> ({$view->name})";
    $c2 .= _filter_autop($view->description);
    $c3 = $view->tag;
    $links = array();
    $loop_index = 0;
    $displays_count = count($view->display);

    foreach ($view->display as $display) {
      if ($display->display_plugin === 'page') {
        $link = url($display->display_options['path']);
        $links[] = $link;
      }

      // Default empty behaviours on all displays.
      if (isset($view->display['default']->display_options['empty'])) {
        $empty_behaviours = $view->display['default']->display_options['empty'];
      }

      // Overrided empty behaviours.
      $empty_behaviours = array();
      $empty_messages = array();
      if (isset($display->display_options['empty'])) {
        $empty_behaviours = $display->display_options['empty'];
      }

      // List readable empty messages.
      if (is_array($empty_behaviours)) {
        foreach ($empty_behaviours as $behaviour) {
          if (in_array($behaviour['field'], array('area_text_custom', 'area')) && !empty($behaviour['content'])) {
            $empty_messages[] = $behaviour['content'];
          }
        }
      }
      elseif (is_string($empty_behaviours)) {
        $empty_messages[] = $empty_behaviours;
      }

      $display_title = $display->display_title;
      if (module_exists('views_ui')) {
        $display_title = l($display_title, 'admin/structure/views/view/' . $view->name . '/edit/' . $display->id);
      }
      $c4 = '<strong>' . $display_title . '</strong> (' . $display->id . ')';
      $c5 = theme('item_list', array('items' => $empty_messages));
      if ($loop_index == 0) {
        $rows[] = array(
            array('data' => $c1, 'rowspan' => $displays_count),
            array(
                'data'    => $c2 . (empty($links) ? '' : theme('item_list', array('items' => $links, 'title' => t('Paths')))),
                'rowspan' => $displays_count,
            ),
            array('data' => $c3, 'rowspan' => $displays_count),
            $c4,
            $c5
        );
      }
      else {
        $rows[] = array($c4, $c5);
      }
      $loop_index++;
    }
  }

}
