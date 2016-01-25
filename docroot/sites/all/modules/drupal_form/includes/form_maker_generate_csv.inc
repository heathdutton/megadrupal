<?php
/**
 * @file
 * This file generate csv from submissions table.
 **/

/**
 * Download submissions table as CSV.
 */
function form_maker_generate_csv() {
  $form_id = $_GET['form_id'];
  $value = db_query("SELECT * FROM {form_maker_submits_table} WHERE form_id=:form_id", array(':form_id' => $form_id));
  $rows = array();
  while ($a = $value->fetchObject()) {
    $rows[] = $a;
  }
  $n = count($rows);
  $labels = array();
  for ($i = 0; $i < $n; $i++) {
    $row = &$rows[$i];
    if (!in_array($row->element_label, $labels)) {
      array_push($labels, $row->element_label);
    }
  }
  $label_titles = array();
  $sorted_labels = array();
  $value = db_query("SELECT label_order FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $form_id));
  $rows_lable = array();
  while ($a = $value->fetchObject()) {
    $rows_lable[] = $a->label_order;
  }
  $value = db_query("SELECT title FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $form_id));
  $rows_title = array();
  while ($a = $value->fetchObject()) {
    $rows_title[] = $a->title;
  }
  $ptn = "/[^a-zA-Z0-9_]/";
  $rpltxt = "";
  $title = preg_replace($ptn, $rpltxt, $rows_title[0]);
  $sorted_labels_id = array();
  $sorted_labels = array();
  $label_titles = array();
  if ($labels) {
    $label_id = array();
    $label_order = array();
    $label_order_original = array();
    $label_type = array();
    $label_all = explode('#****#', $rows_lable[0]);
    $label_all = array_slice($label_all, 0, count($label_all) - 1);
    foreach ($label_all as $key => $label_each) {
      $label_id_each = explode('#**id**#', $label_each);
      array_push($label_id, $label_id_each[0]);
      $label_oder_each = explode('#**label**#', $label_id_each[1]);
      array_push($label_order_original, $label_oder_each[0]);
      $ptn = "/[^a-zA-Z0-9_]/";
      $rpltxt = "";
      $label_temp = preg_replace($ptn, $rpltxt, $label_oder_each[0]);
      array_push($label_order, $label_temp);
      array_push($label_type, $label_oder_each[1]);
    }
    foreach ($label_id as $key => $label) {
      if (in_array($label, $labels)) {
        array_push($sorted_labels, $label_order[$key]);
        array_push($sorted_labels_id, $label);
        array_push($label_titles, $label_order_original[$key]);
      }
    }
  }
  $m = count($sorted_labels);
  $group_id_s = array();
  $l = 0;
  if (count($rows) > 0 and $m) {
    for ($i = 0; $i < count($rows); $i++) {
      $row = &$rows[$i];
      if (!in_array($row->group_id, $group_id_s)) {
        array_push($group_id_s, $row->group_id);
      }
    }
  }
  $data = array();
  for ($www = 0; $www < count($group_id_s); $www++) {
    $i = $group_id_s[$www];
    $temp = array();
    for ($j = 0; $j < $n; $j++) {
      $row = &$rows[$j];
      if ($row->group_id == $i) {
        array_push($temp, $row);
      }
    }
    $f = $temp[0];
    $date = $f->date;
    $ip = $f->ip;
    $data_temp['Submit date'] = $date;
    $data_temp['Ip'] = $ip;
    $ttt = count($temp);
    for ($h = 0; $h < $m; $h++) {
      for ($g = 0; $g < $ttt; $g++) {
        $t = $temp[$g];
        if ($t->element_label == $sorted_labels_id[$h]) {
          if (strpos($t->element_value, "*@@url@@*")) {
            $new_file = str_replace("*@@url@@*", '', $t->element_value);
            $new_filename = explode('/', $new_file);
            $data_temp[$label_titles[$h]] = $new_file;
          }
          else
            $data_temp[$label_titles[$h]] = $t->element_value;
        }
      }
    }
    $data[] = $data_temp;
  }
  // File name for download.
  $filename = $title . "_" . date('Ymd') . ".csv";
  header("Content-type:application/csv; charset='utf-8'");
  header("Content-Disposition: attachment; filename=\"$filename\"");
  echo chr(239) . chr(187) . chr(191);
  $flag = FALSE;
  foreach ($data as $row) {
    if (!$flag) {
      echo '"' . implode('","', array_keys($row)) . "\"\r\n";
      $flag = TRUE;
    }
    array_walk($row, 'form_maker_csv_cleanData');
    echo '"' . implode('","', array_values($row)) . "\"\r\n";
  }
}

/**
 * Clean data for CSV.
 */
function form_maker_csv_cleanData(&$str) {
  $str = str_replace('<br>', '', $str);
  $str = str_replace('<br />', '', $str);
  $str = str_replace('<br/>', '', $str);
  $str = preg_replace("/\t/", "\\t", $str);
  $str = preg_replace("/\r?\n/", "\\n", $str);
  if (strstr($str, '"')) {
    $str = '"' . str_replace('"', '""', $str) . '"';
  }
}
