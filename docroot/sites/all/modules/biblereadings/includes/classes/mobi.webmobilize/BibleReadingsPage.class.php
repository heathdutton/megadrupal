<?php

module_load_include('php', 'webmobilize', 'includes/classes/mobi.webmobilize/MobilePageBase.class');

class BibleReadingsPage extends MobilePageBase {

  public function BibleReadingsPage() {
    
  }

  public function render() {
    $output = '';

    $today = strtotime(date('Y-m-d'));
    $dow = date('w');
    $sunday = $today - ($dow * 60 * 60 * 24);

    $data = biblereadings_readings_this_week();

    foreach ($data as $i) {
      $i->psalms = biblereadings_get_today_psalms($i->date);
      $i->proverb = biblereadings_get_today_proverb($i->date);
    }

    $output .= '<div class="biblereadings-pane">';
    $output .= '<h3>' . t('Week of !date', array('!date' => date('F j, Y', $sunday))) . '</h3>';
    $output .= theme('biblereadings_readings_this_week', $data, false, true);
    $output .= '</div>';

    return $output;
  }

}