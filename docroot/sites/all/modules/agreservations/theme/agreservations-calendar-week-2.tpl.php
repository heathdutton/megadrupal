<?php
// 
/**
 * @file
 * Template to display a view as a calendar week.
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
        <table>
            <tr class="agreservations-calendar">
                <th class="agreservations-calendar th unittypes">
                    <a class="<?php print (!isset($currentunittype)) ?  "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path().$calendarname."/".$currentselectedweek);?>"><?php print (t('show all units')); ?></a>
                </th>
                <?php foreach ($unittypes as $unittype): ?>
                <th class="agreservations-calendar th unittypes">
                    <a class="<?php print (isset($currentunittype)&&$currentunittype==$unittype->nid) ?  "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path().$calendarname."/".$currentselectedweek."/".$unittype->nid);?>"><?php print ($unittype->title); ?></a>
                </th>
                <?php endforeach; ?>
            </tr>
        </table>
        <table>
            <thead>
                <tr>
                    <th class="agreservations-calendar"><?php print $by_hour_count > 0 ? t('units') : ''; ?></th>
                    <?php foreach ($day_names as $cell): ?>
                    <th class="agreservations-calendar">
                            <?php print $cell['data']; ?>
                    </th>
                    <?php endforeach; ?>
                </tr>
            </thead> <!--<?php print $cell['class']; ?>-->
            <tbody>
                <?php foreach ($units as $unit): ?>
                <tr>
                    <td class="agreservations-calendar">

                        <a href="<?php print(base_path());?>node/<?php print $unit->nid ?>"><?php print $unit->title  ?></a>
                        <span class="agreservations-calendar-hour"><?php print $by_hour_count > 0 ? t('Night') : ''; ?></span>
                    </td>
                        <?php foreach ($rows as $day): ?>
                    <td class="agreservations-calendar-agenda-items">
                                <?php print ($day['datebox']);?>
                        <!-- <div class="calendar">-->
                        <div class="agreservations-inner">
                            <a href="<?php print(base_path());?>node/add/agreservation?edit[field_agres_ref_unit][nid][<?php print $unit->nid ?>]=<?php print $unit->nid ?>&edit[title]=Reservation&default_agres_date=<?php print$day['date']?> 14:00 ">+</a>
                                    <?php if (isset($day['night'][$unit->title]))  : ?>
                                        <?php foreach ($day['night'][$unit->title] as $unitbookings): ?>
                                            <?php print (t($unitbookings));?>
                                        <?php endforeach; ?>
                                    <?php endif; ?>
                        </div>
                        <!--</div>-->
                    </td>
                        <?php endforeach; ?>
                </tr>
                <?php endforeach; ?>
                            </tbody>
        </table>
      <!--  <table>
                <?php foreach ($units as $unit): ?>
                <tr>
                <td class="agreservations-agenda-hour">
                    <?php print $unit->title; ?>

                <?php foreach ($items[$unit->title] as $time): ?>
                   <tr>
                   <td class="agreservations-agenda-hour">
                        <span class="agreservations-hour"><?php print $time['hour']; ?></span>
                        <span class="agreservations-ampm"><?php print $time['ampm']; ?></span>
                    </td>
                        <?php foreach ($columns as $column): ?>
                    <td class="agreservations-agenda-items">
                        <div class="agreservations-calendar">
                            <div class="agreservations-inner">
                               <?php print isset($time['values'][$column]) ? implode($time['values'][$column]) : '&nbsp;'; ?>
                            </div>
                        </div>
                    </td>
                        <?php endforeach; ?>
                    </tr>
                <?php endforeach; ?>

                    </tr>

               <?php endforeach; ?>

</table>-->
    </div></div>