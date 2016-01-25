<?php
/**
 * @file
 * Template to display a view as a calendar month.
 *
 * @see template_preprocess_calendar_month.
 * agdaysresitems agreservations
 * $day_names: An array of the day of week names for the table header.
 * $rows: An array of data for each day of the week.
 * $view: The view.
 * $calendar_links: Array of formatted links to other calendar displays - year, month, week, day.
 * $display_type: year, month, day, or week.
 * $block: Whether or not this calendar is in a block.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $date_id: a css id that is unique for this date,
 *   it is in the form: calendar-nid-field_name-delta
 *
 */
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);
?>
<div class="agreservations-calendar"><div class="month-view">
    <?php if (module_exists('agres_categories')): ?>
      <table class="agreservations-table" style="float:left; text-align: left;">
        <tr class="agreservations-calendar">
          <th class="agreservations-calendar th categories">
            <a class="<?php print (!isset($currentcategory)) ? "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedmonth); ?>"><?php print (t('show all categories')); ?></a>
          </th>
          <?php foreach ($categories as $category): ?>
            <th class="agreservations-calendar th categories">
              <a class="<?php print (isset($currentcategory) && $currentcategory == $category->nid) ? "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedmonth . "/" . $category->nid); ?>"><?php print ($category->title); ?></a>
            </th>
          <?php endforeach; ?>
        </tr>
      </table>
    <?php endif; ?>
    <table class="agreservations-table">
      <tr class="agreservations-calendar">
        <th class="agreservations-calendar th unittypes">
          <a class="<?php print (!isset($currentunittype)) ? "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedmonth . "/" . $currentcategory); ?>"><?php print (t('show all units')); ?></a>
        </th>
        <?php foreach ($unittypes as $unittype): ?>
          <th class="agreservations-calendar th unittypes">
            <a class="<?php print (isset($currentunittype) && $currentunittype == $unittype->nid) ? "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedmonth . "/" . $currentcategory . "/" . $unittype->nid); ?>"><?php print ($unittype->title); ?></a>
          </th>
        <?php endforeach; ?>
      </tr>
    </table>
    <table class="agreservations-table"  style="table-layout:fixed">
      <colgroup>
        <col width="30px">
        <col class="colitem" width="15px" >
        <?php for ($i = 1; $i <= $daynumber; $i++): ?>
          <col class="colitem" width="14" >
        <?php endfor; ?>
      </colgroup>
      <?php foreach ((array) $units as $unit): ?>
        <tr>
          <td class="agreservations-calendar unitname" rowspan="<?php print (count($agmonth_rows_copy[$unit->title])+2); ?>" style="font-size:xx-small">
            
            <a href="<?php print(base_path()); ?>node/<?php print $unit->nid ?>"><?php print $unit->title ?></a>
          </td>
            <?php if (count($agmonth_rows_copy[$unit->title])>1) : ?>            
                <td >nr.</td>  
                <?php endif; ?>
          <?php foreach ((array) $agmonth_rows[$unit->title] as $roomrow): ?>
            <?php if (!empty($roomrow['datebox'])) : ?>
              <td>
                <?php print($roomrow['datebox']); ?>
              </td>
            <?php endif; ?>
          <?php endforeach; ?>
        <tr>
          <?php foreach ((array) $agmonth_rows_copy[$unit->title] as $korow=>$orow): ?>          
          <tr>
            <?php if (count($agmonth_rows_copy[$unit->title])>1) : ?>
            
                <td >
                  <?php print($korow+1); ?>
<!--                  <a href="<?php print(base_path() . rawurldecode('node/add/agreservation?&agres_sel_unit=' . $unit->nid . '&default_agres_title=Reservation&default_agres_date=' . $roomrow['date'] . ' 14:00')); ?>"> +</a>-->
                </td>  
                <?php endif; ?>
          <?php foreach ($orow as $keyroomrow=>$roomrow): ?>
            <?php if (!empty($roomrow['datebox'])) : ?>
              <?php if (!empty($roomrow['data'])) : ?>
                <?php foreach ((array) $roomrow['data'] as $key => $roomrowres): ?>
                  <?php if ($roomrowres !== '***busy***') : ?>
                      <?php $tmpres = node_load($key);
                      $tmpres_date = '';
                      $tmpres_date = field_view_field('node', $tmpres, 'field_agres_rdate');
                      $tiptext = t('Reservation').': '.$key.' '.strip_tags($tmpres_date[0]['#markup']); 
                      ?>
     
                   
                    <td   class="beautytips"<?php print isset($agmonth_rows_copy[$unit->title][$korow]['spaninfo'][$key]) ? " colspan=" . '"' . $agmonth_rows_copy[$unit->title][$korow]['spaninfo'][$key] . '"' : "5"; ?><?php print("title=".'"'.$tiptext.'"'); ?>>
<!--                      <pre> <?php print($korow); ?></pre>-->
<!--                      <pre> <?php print_r($agmonth_rows_copy[$unit->title][$korow]['spaninfo'][$key],false); ?></pre>-->
                      <?php print($roomrowres); ?>
                    </td>
                  <?php endif; ?>
                <?php endforeach; ?>
              <?php else: ?>
                <td >
<!--                  <pre> <?php print($korow); ?></pre>-->
                  <a href="<?php print(base_path() . rawurldecode('node/add/agreservation?&agres_sel_unit=' . $unit->nid . '&default_agres_title=Reservation&default_agres_date=' . $roomrow['date'] . ' 14:00')); ?>"> +</a>
                </td>
              <?php endif; ?>
            <?php endif; ?>
          <?php endforeach; ?>
          <?php endforeach; ?>
        </tr>
        <tr>
        <?php endforeach; ?>
    </table>
  </div>
</div>