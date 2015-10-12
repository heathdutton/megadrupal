<?php
/**
 * @file
 * Template file for the theming the modal box.
 *
 * Available custom variables:
 * - $eventtypes
 * - $prev
 * - $next
 * - $today
 * - $year
 * - $month
 * - $week
 * - $day
 *
 */
?>
<div class="calendar-control">
  <h3 class="calendar-title"></h3>
  <div class="btn-group">
    <button class="btn btn-primary" data-calendar-nav="prev"><< <?php print $prev; ?></button>
    <button class="btn btn-default" data-calendar-nav="today"><?php print $today; ?></button>
    <button class="btn btn-primary" data-calendar-nav="next"><?php print $next; ?> >></button>
  </div>
  <div class="btn-group">
    <button class="btn btn-warning" data-calendar-view="year"><?php print $year; ?></button>
    <button class="btn btn-warning active" data-calendar-view="month"><?php print $month; ?></button>
    <button class="btn btn-warning" data-calendar-view="week"><?php print $week; ?></button>
    <button class="btn btn-warning" data-calendar-view="day"><?php print $day; ?></button>
  </div>
  <div class="select-year btn-group">
    <select name="selectionYear" class="form-control">
      <option value="2014">2014</option>
      <option selected value="2015">2015</option>
      <option value="2016">2016</option>
       <option value="2017">2017</option>
      <option value="2018">2018</option>
    </select>
   </div>

  <?php if ($eventtypes): ?>
    <?php print $eventtypes; ?>
  <?php endif; ?>
<div id="calendar"></div>
<div id="calendar-footer"><span class="month-title"></span> <span class="year-title"></span></div>
