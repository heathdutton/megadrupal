<?php
/**
 * @file
 * Template to display a view as a calendar week with overlapping items
 *
 * @see template_preprocess_calendar_week.
 *
 * $day_names: An array of the day of week names for the table header.
 * $rows: The rendered data for this week.
 *
 * For each day of the week, you have:
 * $rows['date'] - the date for this day, formatted as YYYY-MM-DD.
 * $rows['datebox'] - the formatted datebox for this day.
 * $rows['empty'] - empty text for this day, if no items were found.
 * $rows['all_day'] - an array of formatted all day items.
 * $rows['items'] - an array of timed items for the day.
 * $rows['items'][$time_period]['hour'] - the formatted hour for a time period.
 * $rows['items'][$time_period]['ampm'] - the formatted ampm value, if any for a time period.
 * $rows['items'][$time_period]['values'] - An array of formatted items for a time period.
 *
 * $view: The view.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $calendar_mmtid: The MMTID of the page containing the calendar node.
 * $can_add_event: TRUE if the user can add an event to this calendar.
 * $add_vars: Pre-built array to be sent to theme('mm_schedule_add_link')
 *
 */
$header_ids = array();
foreach ($day_names as $key => $value) {
  $header_ids[$key] = $value['header_id'];
}
if ($can_add_event) {
  $add_vars['increment'] = '+1 hour';
  $add_vars['param']['attributes']['title'] = t('Add a new event at this time');
}
$time_text = array();
foreach ($start_times as $index => $time) {
  if ($time < $first_time || $time >= $last_time) {
    unset($start_times[$index]);
  }
  else {
    if ($time == '00:00:00') {
      $time_text[$time] = '<span class="calendar-hour">' . t('midnight', array(), array('context' => 'datetime')) . '</span>';
    }
    else if ($time == '12:00:00') {
      $time_text[$time] = '<span class="calendar-hour">' . t('noon', array(), array('context' => 'datetime')) . '</span>';
    }
    else {
      $time_text[$time] = '<span class="calendar-hour">' . $items[$time]['hour'] . ' ' . $items[$time]['ampm'] . '</span><span class="calendar-ampm">' . '</span>';
    }
  }
}
$start_times = array_merge($start_times);   // Reset array indices after unsets
$max_day_index = $calendar_type == 'week' ? 6 : 0;
?>

<div class="calendar-calendar"><div class="<?php print $calendar_type ?>-view">
  <?php if ($calendar_type == 'week'): ?>
    <div id="header-container">
      <table class="full">
        <tbody>
          <tr class="holder">
            <td class="calendar-time-holder"></td>
            <?php for ($i = 0; $i <= $max_day_index; $i++): ?>
              <td class="calendar-day-holder"></td>
            <?php endfor; ?>
            <td class="calendar-day-holder margin-right"></td>
          </tr>
          <tr>
            <th class="calendar-agenda-hour">&nbsp;</th>
            <?php foreach ($day_names as $cell): ?>
              <th class="<?php print $cell['class']; ?>" id="<?php print $cell['header_id']; ?>">
                <?php print $cell['data']; ?>
              </th>
            <?php endforeach; ?>
            <th class="calendar-day-holder margin-right"></th>
          </tr>
        </tbody>
      </table>
    </div>
  <?php endif; ?>
  <div id="multi-day-container">
  <table class="full">
  <tbody>
    <tr class="holder">
      <td class="calendar-time-holder"></td>
      <?php for ($i = 0; $i <= $max_day_index; $i++): ?>
        <td class="calendar-day-holder"></td>
      <?php endfor; ?>
    </tr>
    <?php for ($i = 0; $i < $multiday_rows; $i++): ?>
    <?php
      $colpos = 0;
      $rowclass = "all-day";
      if ($i == 0) {
        $rowclass .= " first";
      }
      if ($i == $multiday_rows - 1) {
        $rowclass .= " last";
      }
    ?>
    <tr class="<?php print $rowclass?>">
      <?php if ($i == 0 && ($by_hour_count > 0 || !empty($start_times))) :?>
        <td class="<?php print $agenda_hour_class ?>" rowspan="<?php print $multiday_rows?>">
          <span class="calendar-hour"><?php print t('All day', array(), array('context' => 'datetime'))?></span>
        </td>
      <?php endif; ?>
      <?php for ($j = 0; $j <= $max_day_index; $j++): ?>
        <?php $cell = (empty($all_day[$j][$i])) ? NULL : $all_day[$j][$i]; ?>
        <?php if ($cell != NULL && $cell['filled'] && $cell['wday'] == $j): ?>
          <?php for (; $colpos < $cell['wday']; $colpos++) : ?>
          <?php
            $colclass = "calendar-agenda-items multi-day";
            if ($colpos == 0) {
              $colclass .= " first";
            }
            if ($colpos == $max_day_index) {
              $colclass .= " last";
            }
          ?>
          <td class="<?php print $colclass?>"><div class="inner">&nbsp;</div></td>
          <?php endfor;?>
          <?php
            $colclass = "calendar-agenda-items multi-day";
            if ($colpos == 0) {
              $colclass .= " first";
            }
            if ($colpos == $max_day_index) {
              $colclass .= " last";
            }
          ?>
          <td colspan="<?php print $cell['colspan']?>" class="<?php print $colclass?>">
            <div class="inner">
            <?php print $cell['entry']?>
            </div>
          </td>
          <?php $colpos += $cell['colspan']; ?>
        <?php endif; ?>
      <?php endfor; ?>
      <?php while ($colpos <= $max_day_index) : ?>
        <?php
          $colclass = "calendar-agenda-items multi-day no-entry";
          if ($colpos == 0) {
            $colclass .= " first";
          }
          if ($colpos == $max_day_index) {
            $colclass .= " last";
          }
        ?>
        <td class="<?php print $colclass?>"><div class="inner">&nbsp;</div></td>
        <?php $colpos++; ?>
      <?php endwhile;?>
    </tr>
    <?php endfor; ?>
    <?php if ($multiday_rows == 0) :?>
      <tr>
        <?php
        for ($j = 0; $j <= $max_day_index; $j++):
          $colclass = 'calendar-agenda-items multi-day no-entry';
          if ($j == 0) {
            $colclass .= ' first';
          }
          if ($j == $max_day_index) {
            $colclass .= ' last';
          } ?>
          <td class="<?php print $colclass?>"><div class="inner">&nbsp;</div></td>
        <?php endfor; ?>
      </tr>
    <?php endif; ?>
  </tbody>
  </table>
  </div>
  <div class="header-body-divider">&nbsp;</div>
  <div id="single-day-container">
    <?php if (!empty($scroll_content)) : ?>
      <script>
        try {
          // Hide container while it renders...  Degrade w/o javascript support
          jQuery('#single-day-container').css('visibility','hidden');
        } catch(e) {
          // swallow
        }
      </script>
    <?php endif; ?>
    <table class="full subdiv-<?php print $subdivisions; ?>">
      <tbody>
        <tr class="holder">
          <td class="calendar-time-holder"></td>
          <?php for ($i = 0; $i <= $max_day_index; $i++): ?>
            <td class="calendar-day-holder"></td>
          <?php endfor; ?>
        </tr>
        <tr>
          <td class="first" headers="<?php print $header_ids[0]; ?>">
            <?php foreach ($start_times as $time_cnt => $start_time): ?>
              <?php
                if ($time_cnt == 0) {
                  $class = 'first ';
                }
                elseif ($time_cnt == count($start_times) - 1) {
                  $class = 'last ';
                }
                else {
                  $class = '';
                } ?>
              <div class="<?php print $class?>calendar-agenda-hour">
                <?php print $time_text[$start_time]; ?>
              </div>
            <?php endforeach; ?>
          </td>
          <?php for ($index = 0; $index <= $max_day_index; $index++): ?>
            <?php if ($index == $max_day_index) : ?>
              <td class="last">
            <?php else : ?>
              <td headers="<?php print $header_ids[$index + 1]; ?>">
            <?php endif; ?>
            <?php foreach ($start_times as $time_cnt => $start_time): ?>
              <?php
                if ($time_cnt == 0) {
                  $class = 'first ';
                }
                elseif ($time_cnt == count($start_times) - 1) {
                  $class = 'last ';
                }
                else {
                  $class = '';
                } ?>
              <?php $time = $items[$start_time];?>
              <div class="<?php print $class?>calendar-agenda-items single-day">
                <?php if (!empty($time['values'][$index])) :?>
                  <div class="calendar item-wrapper">
                    <div class="inner">
                      <?php foreach($time['values'][$index] as $item) :?>
                        <?php if (!empty($item['is_first'])) :?>
                          <div class="item <?php print $item['class']?> first_item">
                        <?php else : ?>
                          <div class="item <?php print $item['class']?>">
                        <?php endif; ?>
                        <?php print $item['entry'] ?>
                        </div>
                      <?php endforeach; ?>
                    </div>
                  </div>
                <?php endif; ?>
                <?php if (!empty($add_vars)) {
                  $add_vars['start_date'] = clone($view->date_info->min_date);
                  date_modify($add_vars['start_date'], "+$index day");
                  date_time_set($add_vars['start_date'], substr($start_time, 0, 2), substr($start_time, 3, 2));
                }
                for ($subdiv = 0; $subdiv < $subdivisions; $subdiv++): ?>
                  <div class="half-hour">&nbsp;
                    <?php if (/*empty($time['values'][$index]) &&*/ !empty($add_vars)) {
                      print theme('mm_schedule_add_link', $add_vars);
                      date_modify($add_vars['start_date'], "+$sub_increment minute");
                   }?>
                  </div>
                <?php endfor ?>
              </div>
            <?php endforeach; /* $start_times as $time_cnt => $start_time */?>
          </td>
          <?php endfor; /* $index = 0; $index < 7; $index++ */ ?>
        </tr>
      </tbody>
    </table>
  </div>
  <div class="single-day-footer">&nbsp;</div>
</div></div>
<?php if (!empty($scroll_content)) : ?>
<script>
try {
  // Size and position the viewport inline so there are no delays
  calendar_resizeViewport(jQuery);
//  calendar_scrollToFirst(jQuery);

  // Show it now that it is complete and positioned
  jQuery('#single-day-container').css('visibility','visible');
} catch(e) {
  // swallow
}
</script>
<?php endif; ?>