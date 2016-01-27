<?php
/**
 * @file
 * Defines the templated used to produce the the Gantt Chart
 */
?>
<div style="position:relative" class="gantt" id="GanttChartDIV"></div>
<script language="javascript">
  var g = new JSGantt.GanttChart('g',document.getElementById('GanttChartDIV'), '<?php print($options['format']); ?>');
  g.setShowRes(0); g.setShowDur(0); g.setShowComp(0); g.setCaptionType('Resource'); g.setShowStartDate(0); g.setShowEndDate(0);
<?php
for ($rid = 0; $rid < count($view->result); $rid++) :
  $row = $view->result[$rid];
  // For each remaining row in the result after the 'current' row.
  for ($i = $rid + 1; $i < count($view->result); $i++) :
    if (isset($view->result[$i]->$options['parentid'])) :
      // If this row belongs to the same project as the 'current' row.
      if ($view->result[$i]->$options['parentid'] == $row->$options['parentid']) :
        // Move this row up.
        array_splice($view->result, $rid + 1, 0, array($view->result[$i]));
        array_splice($view->result, $i + 1, 1);
        break;
      endif;
    endif;
  endfor;
endfor;

$last_group = 0;

foreach ($view->result as $rid => $row) :

  $task = array();
  $task['pID'] = is_numeric($row->nid) ? $row->nid : 0;
  $task['pName'] = check_plain($row->$options['tasktitle']);

  $arr_startdate = $row->$options['startdate'];
  // Even though a date field could have more than one value, use only the
  // first.
  $task['pStart'] = date("n/j/Y", strtotime(strip_tags($arr_startdate['0']['rendered']['#markup'])));

  $arr_enddate = $row->$options['enddate'];
  // Even though a date field could have more than one value, use only the
  // first.
  $task['pEnd'] = $arr_enddate['0']['rendered']['#markup'] ? date("n/j/Y", strtotime(strip_tags($arr_enddate['0']['rendered']['#markup']))) : '';

  if (!empty($options['progress'])) :
    $arr_progress = $row->$options['progress'];
    $task['pComp'] = $arr_progress['0']['rendered']['#markup'] ? strip_tags($arr_progress['0']['rendered']['#markup']) : 0;
  endif;

  $task['pRes'] = $view->render_field($options['resource'], $rid);
  if ($task['pRes'] == 'Anonymous') :
    $task['pRes'] = '';
  endif;

  // Replace divs with spans to display caption with multiple values inline.
  $task['pRes'] = str_replace('div', 'span', $task['pRes']);
  // Wrap caption in a no-wrap span to prevent line breaks.
  $task['pRes'] = '<span style="white-space: nowrap;">' . $task['pRes'] . '</span>';
  // Separate multiple values with a comma.
  $task['pRes'] = str_replace('/span><span', '/span><span>, </span><span', $task['pRes']);
  $task['pColor'] = 'f87217';
  if ($options['colorsource'] == 'progress') :
    $cutoff = ($task['pComp'] / 100) * (strtotime($task['pEnd']) - strtotime($task['pStart'])) + strtotime($task['pStart']);
    if ($cutoff > time()) :
      $task['pColor'] = '0000ff';
    endif;
    if ($task['pComp'] == 100) :
      $task['pColor'] = '817679';
    endif;
  elseif (!empty($options['colorby'])) :
    $list = array_filter(array_map('trim', explode("\n", $options['colorlist'])), 'strlen');
    foreach ($list as $opt) :
      // Sanitize the user input with a permissive filter.
      $opt = content_filter_xss($opt);
      if (strpos($opt, '|') !== FALSE) :
        list($key, $value) = explode('|', $opt);
        $colors[$key] = (isset($value) && $value !== '') ? $value : $key;
      else :
        $colors[$opt] = $opt;
      endif;
    endforeach;
    $color_key = $row->$options['colorby'];
    if (is_array($colors) && array_key_exists($color_key, $colors)) :
      $task['pColor'] = $colors[$color_key];
    endif;
  endif;
  $task['pLink'] = url('node/' . $task['pID']);
  $task['pMile'] = $task['pEnd'] ? 0 : 1;
  $task['pParent'] = 0;
  $task['pParentTitle'] = '';

  if (!empty($options['parentid'])) :
    $arr_parentid = $row->$options['parentid'];
    $task['pParent'] = $arr_parentid['0']['raw']['target_id'] ? $arr_parentid['0']['raw']['target_id'] : '';
    $task['pParentTitle'] = $arr_parentid['0']['rendered']['#markup'] ? $arr_parentid['0']['rendered']['#markup'] : '';
  endif;

  $task['pDepend'] = '';
  if (!empty($options['dependson'])) :
    $arr_dependson = $row->$options['dependson'];
    if (!empty($arr_dependson)) :
      $arr_task = array();
      foreach ($arr_dependson as $key => $value) :
        $arr_task[] = $arr_dependson[$key]['raw']['target_id'] ? $arr_dependson[$key]['raw']['target_id'] : '';
      endforeach;
      $task['pDepend'] = implode(',', $arr_task);
    endif;
  endif;

  $format = "g.AddTaskItem(new JSGantt.TaskItem(%d, '%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d, 1, '%s'));\n";
  if ($task['pParent'] != $last_group) :
    // This task has a different parent from the last - add the parent group.
    $task['pParentLink'] = url('node/' . $task['pParent']);
    printf($format, $task['pParent'], $task['pParentTitle'], '', '', '', $task['pParentLink'], 0, '', 0, 1, 0, 0);
    $last_group = $task['pParent'];
  endif;
  printf($format, $task['pID'], $task['pName'], $task['pStart'], $task['pEnd'], $task['pColor'], $task['pLink'], $task['pMile'], $task['pRes'], $task['pComp'], 0, $task['pParent'], $task['pDepend']);
endforeach;
?>
  g.Draw();
  g.DrawDependencies();
</script>
