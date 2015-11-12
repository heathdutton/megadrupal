<?php
/**
 * @file
 * Template to display a view as a calendar week this template is used when the agres_categories module is activated.
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
 *
 */
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);
//dsm($rows);
//dsm($items);//currentunittype calendarname
?>
<div class="agreservations-calendar"><div class="week-view">
    <table class="agreservations-table">
      <thead>
        <tr class="agreservations-calendar">
          <th class="agreservations-calendar th categories">
            <a class="<?php print (!isset($currentcategory)) ? "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedweek); ?>"><?php print (t('show all categories')); ?></a>
          </th>
          <?php foreach ($categories as $category): ?>
            <th class="agreservations-calendar th categories">
              <a class="<?php print (isset($currentcategory) && $currentcategory == $category->nid) ? "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedweek . "/" . $category->nid); ?>"><?php print ($category->title); ?></a>
            </th>
          <?php endforeach; ?>
        </tr>
    </table>
    <table class="agreservations-table">
      <tr class="agreservations-calendar">
        <th class="agreservations-calendar th unittypes">
          <a class="<?php print (!isset($currentunittype)) ? "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedweek . "/" . $currentcategory); ?>"><?php print (t('show all units')); ?></a>
        </th>
        <?php foreach ($unittypes as $unittype): ?>
          <th class="agreservations-calendar th unittypes">
            <a class="<?php print (isset($currentunittype) && $currentunittype == $unittype->nid) ? "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path() . $agrescurrentpath . "/" . $currentselectedweek . "/" . $currentcategory . "/" . $unittype->nid); ?>"><?php print ($unittype->title); ?></a>
          </th>
        <?php endforeach; ?>
      </tr>
    </table>
    <table class="agreservations-table">
      <tr>
        <th class="agreservations-calendar"><?php print $by_hour_count > 0 ? t('units') : ''; ?></th>
        <?php foreach ($day_names as $cell): ?>
          <th class="agreservations-calendar">
            <?php print $cell['data']; ?>
          </th>
        <?php endforeach; ?>
      </tr>
      </thead>
      <tbody>
        <?php foreach ($units as $unit): ?>
          <tr>
            <td class="agreservations-calendar">
              <a href="<?php print(base_path()); ?>node/<?php print $unit->nid ?>"><?php print $unit->title ?></a>
              <span class="agreservations-calendar-hour"></span>
            </td>
            <?php foreach ($rows as $diw => $day): ?>
              <?php if (isset($day['night'][$unit->title])) : ?>
                <?php foreach ($day['night'][$unit->title] as $itemnid => $unitbookings): ?>

                  <?php if (isset($day['night'][$unit->title][$itemnid])) : ?>
                    <?php if (($day['night'][$unit->title][$itemnid]) !== '***busy***') : ?>
                      <td class="agreservations-calendar-agenda-items" <?php print isset($spaninfo[$unit->title][$itemnid]) ? "colspan=" . $spaninfo[$unit->title][$itemnid] : ""; ?> >
                        <?php print ($day['night'][$unit->title][$itemnid]); ?>
                      </td>
                    <?php endif; ?>
                  <?php endif; ?>
                <?php endforeach; ?>
              <?php else: ?>
                <td class="agreservations-calendar-agenda-items">
                  <?php print ($day['datebox']); ?>
                  <a href="<?php print(base_path()); ?>node/add/agreservation?&agres_sel_unit=<?php print $unit->nid ?>&default_agres_title=Reservation&default_agres_date=<?php print$day['date'] ?> 14:00 ">+</a>
                </td>
              <?php endif; ?>
            <?php endforeach; ?>
          </tr>
        <?php endforeach; ?>
      </tbody>
    </table>
  </div>
</div>