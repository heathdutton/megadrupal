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
<div class="agreservations-calendar"><div class="agreservations-month-view">
        <?php if (module_exists('agres_categories')): ?>
      
        <table class="agreservations-table" style="float:left; text-align: left;">
            <tr class="agreservations-calendar">
                <th class="agreservations-calendar th categories">
                    <a class="<?php print (!isset($currentcategory)) ?  "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path().$agrescurrentpath."/".$currentselectedmonth);?>"><?php print (t('show all categories')); ?></a>
                </th>
                <?php foreach ($categories as $category): ?>
                <th class="agreservations-calendar th categories">
                    <a class="<?php print (isset($currentcategory)&&$currentcategory==$category->nid) ?  "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path().$agrescurrentpath."/".$currentselectedmonth."/".$category->nid);?>"><?php print ($category->title); ?></a>
                </th>
                <?php endforeach; ?>
            </tr>
        </table>
        <?php endif; ?>  
        <table class="agreservations-table">
            <tr class="agreservations-calendar">
                <th class="agreservations-calendar th unittypes">
                    <a class="<?php print (!isset($currentunittype)) ?  "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path().$agrescurrentpath."/".$currentselectedmonth."/".$currentcategory);?>"><?php print (t('show all units')); ?></a>
                </th>
                <?php foreach ($unittypes as $unittype): ?>
                <th class="agreservations-calendar th unittypes">
                    <a class="<?php print (isset($currentunittype)&&$currentunittype==$unittype->nid) ?  "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path().$agrescurrentpath."/".$currentselectedmonth."/".$currentcategory."/".$unittype->nid);?>"><?php print ($unittype->title); ?></a>
                </th>
                <?php endforeach; ?>
            </tr>
        </table>    
<table class="agreservations-table"  style="table-layout:fixed">
<!--  <thead>
    <tr>
      <?php foreach ($day_names as $cell): ?>
        <th class="<?php print $cell['class']; ?>">
          <?php print $cell['data']; ?>
        </th>
      <?php endforeach; ?>
    </tr>
  </thead>-->
<!--  <tbody>-->
   
  <?php foreach ((array) $units as $unit): ?>
        <tr>
                    <td class="agreservations-calendar unitname" rowspan="2" style="width:40px;">

                        <a href="<?php print(base_path());?>node/<?php print $unit->nid ?>"><?php print $unit->title  ?></a>
                  
                    </td>  
                    
      <?php foreach ((array) $agmonth_rows[$unit->title] as $roomrow): ?>
                     <?php if (!empty($roomrow['datebox'])) : ?>
                    <td>                     
                     <?php print($roomrow['datebox']);?>
                    </td> 
                     <?php endif; ?>  
      <?php endforeach; ?>
        <tr>
  
    <?php foreach ((array) $agmonth_rows[$unit->title] as $roomrow): ?>
          
          <?php if (!empty($roomrow['datebox'])) : ?>
         
                               <?php if (!empty($roomrow['data'])) : ?>

                                 <?php foreach ((array) $roomrow['data'] as $key => $roomrowres): ?>
          
                                     <?php if ($roomrowres!=='***busy***') : ?>                                
                                       <td  <?php print isset($agmonth_rows[$unit->title]['spaninfo'][$key]) ? "colspan=".'"'.$agmonth_rows[$unit->title]['spaninfo'][$key].'"' : "5"; ?>>
<!--                                         <div class="agreservations-inner">-->
                                            <?php print($roomrowres); ?>
<!--                                         </div>-->
                                        </td>
                                     <?php endif; ?>
                                 <?php endforeach; ?>                                                     
                               <?php else: ?>
                                 <td >
                                   <a href="<?php print(base_path().rawurldecode('node/add/agreservation?&agres_sel_unit='.$unit->nid.'&default_agres_title=Reservation&default_agres_date='.$roomrow['date'].' 14:00'));?>"> +</a>                                
                                 </td>
                               <?php endif; ?>                              
                  

                  

 
          <?php endif; ?> 
    <?php endforeach; ?>
            </tr>
            <tr>
 <?php endforeach; ?>
<!--  </tbody>-->
</table>
</div></div>

<script>
try {
  // ie hack to make the single day row expand to available space
  if ($.browser.msie ) {
    var multiday_height = $('tr.multi-day')[0].clientHeight; // Height of a multi-day row
    $('tr[iehint]').each(function(index) {
      var iehint = this.getAttribute('iehint');
      // Add height of the multi day rows to the single day row - seems that 80% height works best
      var height = this.clientHeight + (multiday_height * .8 * iehint); 
      this.style.height = height + 'px';
    });
  }
}catch(e){
  // swallow 
}
</script>