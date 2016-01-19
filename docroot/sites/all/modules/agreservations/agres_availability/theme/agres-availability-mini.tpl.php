<?php
/**
 * @file
 * Template to display a view as a mini calendar month.
 * 
 * @see template_preprocess_calendar_mini.
 *
 * $day_names: An array of the day of week names for the table header.
 * $rows: An array of data for each day of the week.
 * $view: The view.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * 
 * $show_title: If the title should be displayed. Normally false since the title is incorporated
 *   into the navigation, but sometimes needed, like in the year view of mini calendars.
 * 
 */
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);dsm($day_names);
$params = array(
  'view' => $view,
  'granularity' => 'month',
  'link' => FALSE,
);
?>
<div class="calendar-calendar"><?php print $yearinfo ?><div class="month-view">
<?php if ($show_title): ?>
<div class="date-nav-wrapper clear-block">
  <div class="date-nav">
    <div class="date-heading">
      <?php print theme('date_nav_title', $params) ?>
    </div>
  </div>
</div> 
<?php endif; ?> 
        <?php if (module_exists('agres_categories')): ?>
        <?php if (!isset($agrcategory_nid)): ?>
        <table class="mini">
            <tr class="agreservations-calendar">
                <th class="agreservations-calendar th categories">
                    <a class="<?php print (!isset($currentcategory)) ?  "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path().$agrescurrentpath."?mini=".$currentselectedmonth);?>"><?php print (t('show all categories')); ?></a>
                </th>
                <?php foreach ($categories as $category): ?>
                <th class="agreservations-calendar th categories">
                    <a class="<?php print (isset($currentcategory)&&$currentcategory==$category->nid) ?  "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path().$agrescurrentpath."?mini=".$currentselectedmonth."&cat=".$category->nid);?>"><?php print ($category->title); ?></a>
                </th>
                <?php endforeach; ?>
            </tr>
        </table>
         <?php endif; ?>
        <?php endif; ?>    
<table class="mini">
  <thead>
    <tr>
      <?php foreach ($day_names as $cell): ?>
        <th class="<?php print $cell['class']; ?>">
          <?php print $cell['data']; ?>
        </th>
      <?php endforeach; ?>
    </tr>
  </thead>
  <tbody>
    <?php foreach ((array) $agdaysresitems as $row): ?>
      <tr>
        <?php foreach ($row as $cell): ?>
          <?php if (!empty($cell['availability1'])&&$cell['availability1']==2) : ?>
            <td id="<?php print $cell['id']; ?>" class="agres_availability_free">
  <!--              <pre> <?php print_r($cell) ?> </pre>-->
              <?php print $cell['data']['datebox']; ?>
            </td>
          <?php endif; ?>
          <?php if (!empty($cell['availability1'])&&$cell['availability1']==1) : ?>
            <td id="<?php print $cell['id']; ?>" class="agres_availability_partlyfree">
  <!--              <pre> <?php print_r($cell) ?> </pre>-->
              <?php print $cell['data']['datebox']; ?>
            </td>
          <?php endif; ?>
          <?php if (!empty($cell['availability1'])&&$cell['availability1']==99) : ?>
            <td id="<?php print $cell['id']; ?>" class="agres_availability_fullybooked">
  <!--              <pre> <?php print_r($cell) ?> </pre>-->
              <?php print $cell['data']['datebox']; ?>
            </td>
          <?php endif; ?>   
          <?php if (!empty($cell['availability1'])&&$cell['availability1']==88) : ?>
            <td id="<?php print $cell['id']; ?>" class="agres_availability_unavailable">
  <!--              <pre> <?php print_r($cell) ?> </pre>-->
              <?php print $cell['data']['datebox']; ?>
            </td>
          <?php endif; ?>             
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>
</div></div>