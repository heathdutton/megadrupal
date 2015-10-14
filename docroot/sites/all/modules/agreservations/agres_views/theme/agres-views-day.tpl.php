<?php
/**
 * @file
 * Template to display a view as a calendar day, grouped by resources
 * and optionally organized into columns by a field value.
 *
 * @see template_preprocess_calendar_day.
 * $resources: An Array of the rooms or resources which can be booked.
 * $resources[$name][$rows] - An array of resourcenames each containing
 * the rows for that day and the current resource.(usually just 1 to 1 except
 * you have 1 room and multiple beds to book seperately)
 *
 * $rows: The rendered data for this day and this resource.
 * $rows['date'] - the date for this day, formatted as YYYY-MM-DD.
 * $rows['datebox'] - the formatted datebox for this day.
 * $rows['empty'] - empty text for this day, if no items were found.
 * $rows['all_day'] - an array of formatted all day items.
 * $rows['items'] - an array of timed items for the day.
 * $rows['items'][$time_period]['hour'] - the formatted hour for a time period.
 * $rows['items'][$time_period]['ampm'] - the formatted ampm value, if any for a time period.
 * $rows['items'][$time_period][$column]['values'] - An array of formatted
 *   items for a time period and field column.
 *
 * $view: The view.
 * $columns: an array of column names.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.

 */
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);
?>
<div  class="agreservations-calendar">
  <div class="day-view">
    <table class="agreservations-table">
      <tr class="agreservations-calendar">
        <th class="agreservations-calendar th categories">
          <a class="<?php print (!isset($currentcategory)) ? "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedday); ?>"><?php print (t('show all categories')); ?></a>
        </th>
        <?php foreach ($categories as $category): ?>
          <th class="agreservations-calendar th categories">
            <a class="<?php print (isset($currentcategory) && $currentcategory == $category->nid) ? "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedday . "/" . $category->nid); ?>"><?php print ($category->title); ?></a>
          </th>
        <?php endforeach; ?>
      </tr>
    </table>
    <table class="agreservations-table">
      <tr class="agreservations-calendar">
        <th class="agreservations-calendar th unittypes">
          <a class="<?php print (!isset($currentunittype)) ? "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedday); ?>"><?php print (t('show all units')); ?></a>
        </th>
        <?php foreach ($unittypes as $unittype): ?>
          <th class="agreservations-calendar th unittypes">
            <a class="<?php print (isset($currentunittype) && $currentunittype == $unittype->nid) ? "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedday . "/" . $unittype->nid); ?>"><?php print ($unittype->title); ?></a>
          </th>
        <?php endforeach; ?>
      </tr>
    </table>
    <table class="agreservations-table">
      <colgroup>
        <col width=70>
        <col width="20">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
        <col width="15">
      </colgroup>
      <tbody>
        <?php foreach ($units as $unit): ?>
          <tr>
            <th class="agreservations-calendar-agenda-hour">
            </th>
            <?php foreach ($rows['items'][$unit->title] as $hour): ?>
              <th class="agreservations-calendar-agenda-hour">
                <span class="agreservations-calendar-hour"><?php print $hour['hour']; ?></span>
                <span <?php print $hour['ampm']; ?></span>
              </th>
            <?php endforeach; ?>
          <tr>
            <td class="agreservations-calendar unitname"><a href="<?php print(base_path()); ?>node/<?php print $unit->nid ?>"><?php print $unit->title ?></a>
            </td>
            <?php foreach ($rows['items'][$unit->title] as $hour): ?>
              <?php foreach ($columns as $column): ?>
                <?php if (isset($hour['values'])) : ?>
                  <?php if (($hour['values'][$column] != '***busy***')) : ?>
                    <td class="agreservations-agenda-items <?php print ($hour['values'][$column]) ? 'calendar-has-item' : ''; ?>" <?php print isset($hour['span'][$column]) ? "colspan=" . $hour['span'][$column] : ""; ?> >
                      <div class="agreservations-calendar">
                        <?php if (isset($hour['values'][$column])) : ?>
                          <?php print isset($hour['values'][$column]) ? $hour['values'][$column] : '&nbsp;'; ?>
                        <?php endif; ?>
                        <?php if (!isset($hour['values'][$column])) : ?>
                          <a href="<?php print(base_path()); ?>node/add/agreservation?&agres_sel_unit=<?php print $unit->nid ?>&default_agres_title=Reservation&default_agres_date=<?php print $currentselectedday ?> <?php print $hour['hour']; ?>">+</a>
                        <?php endif; ?>
                      </div>
                    </td>
                  <?php endif ?>
                <?php else: ?>
                  <td class="agreservations-agenda-items">
                      <div class="agreservations-calendar">
                      
                          <a href="<?php print(base_path()); ?>node/add/agreservation?&agres_sel_unit=<?php print $unit->nid ?>&default_agres_title=Reservation&default_agres_date=<?php print $currentselectedday ?> <?php print $hour['hour']; ?>">+</a>
               
                      </div>                    
                  </td>
                <?php endif ?>
              <?php endforeach; ?>
            <?php endforeach; ?>
          <?php endforeach; ?>
        </tr>
      </tbody>
    </table>
  </div>
</div>