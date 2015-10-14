<?php
/**
 * @file
 * Template to display a view as a calendar month.
 *
 * @see template_preprocess_calendar_month.
 *
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
<div class="calendar-calendar">
  <div class="month-view">
    <table>
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
    <table>
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
    <table>
      <tbody>
        <?php foreach ((array) $units as $unit): ?>
          <tr>
            <td class="agreservations-calendar">
              <a href="<?php print(base_path()); ?>node/<?php print $unit->nid ?>"><?php print $unit->title ?></a>
              <span class="agreservations-calendar-hour"><?php print $by_hour_count > 0 ? t('Night') : ''; ?></span>
            </td>
            <?php foreach ((array) $rows as $row): ?>
              <?php foreach ($row as $cell): ?>
                <?php if (isset($cell[$unit->title]['data'])) : ?>
                  <?php if (isset($cell[$unit->title])) : ?>
                    <td class="agreservations-calendar-agenda-items">
                      <div class="agreservations-inner ">
                        <?php print ($cell[$unit->title]['data']); ?>
                      </div>
                    </td>
                  <?php endif; ?>
                <?php else: ?>
                  <td class="agreservations-calendar-agenda-items">
                    <?php print ($day['datebox']); ?>
                    <a href="<?php print(base_path()); ?>node/add/agreservation?&agres_sel_unit=<?php print $unit->nid ?>&default_agres_title=Reservation&default_agres_date=<?php print$day[date] ?> 14:00 ">+</a>
                  </td>
                <?php endif; ?>
                <td id="<?php print $cell['id']; ?>" class="<?php print $cell[$unit->title]['class']; ?>">
                  <?php print $cell[$unit->title]['data']; ?>
                </td>
              <?php endforeach; ?>
            <?php endforeach; ?>
          </tr>
        <?php endforeach; ?>
      </tbody>
    </table>
  </div>
</div>