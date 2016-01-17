<?php
// $Id: agreservations-calendar-day-c.tpl.php,v 1.1.2.3 2010/11/13 02:26:16 agill Exp $

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

<div  class="agreservations-calendar"><div class="day-view">
            <table>
            <tr class="agreservations-calendar">
                <th class="agreservations-calendar th categories">
                    <a class="<?php print (!isset($currentcategory)) ?  "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path().$calendarname."/".$currentselectedday);?>"><?php print (t('show all categories')); ?></a>
                </th>
                <?php foreach ($categories as $category): ?>
                <th class="agreservations-calendar th categories">
                    <a class="<?php print (isset($currentcategory)&&$currentcategory==$category->nid) ?  "agreservations-calendar a categorysel" : "agreservations-calendar a categories"; ?>" href="<?php print(base_path().$calendarname."/".$currentselectedday."/".$category->nid);?>"><?php print ($category->title); ?></a>
                </th>
                <?php endforeach; ?>
            </tr>
        </table>
        <table width="800px">
            <tr class="agreservations-calendar">
                <th class="agreservations-calendar th unittypes">
                    <a class="<?php print (!isset($currentunittype)) ?  "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path().$calendarname."/".$currentselectedday);?>"><?php print (t('show all units')); ?></a>
                </th>
                <?php foreach ($unittypes as $unittype): ?>
                <th class="agreservations-calendar th unittypes">
                    <a class="<?php print (isset($currentunittype)&&$currentunittype==$unittype->nid) ?  "agreservations-calendar a unittypessel" : "agreservations-calendar a unittypes"; ?>" href="<?php print(base_path().$calendarname."/".$currentselectedday."/".$unittype->nid);?>"><?php print ($unittype->title); ?></a>
                </th>
                <?php endforeach; ?>
            </tr>
        </table>
        <table width="800px">
            <thead >
                <tr>
                    <th class="agreservations-calendar-dayview-hour"></th>
                    <?php foreach ($header_columns as $column): ?>
                    <th class="agreservations-calendar-agenda-items" <?php print ($column['span'] > 0) ? 'colspan='. $column['span']:''; ?> >

                    </th>
                    <?php endforeach; ?>
                </tr>
            </thead>
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
                    <td class="agreservations-calendar"><a href="<?php print(base_path());?>node/<?php print $unit->unitnid ?>"><?php print $unit->title  ?></a>
                    </td>

                        <?php foreach ($rows['items'][$unit->title] as $hour): ?>
                            <?php foreach ($columns as $column): ?>
                                <?php if($hour['values'][$column] != '***busy***') : ?>
                    <td class="agreservations-agenda-items <?php print ($hour['values'][$column])?'calendar-has-item':'';  ?>" <?php print isset($hour['span'][$column]) ? "colspan=".$hour['span'][$column] : ""; ?> >
                        <div class="agreservations-calendar">
                            <div class="agreservations-inner">
                                                <?php if(isset($hour['values'][$column])) : ?>
                                                    <?php print isset($hour['values'][$column]) ? $hour['values'][$column] : '&nbsp;'; ?>
                                                <?php endif; ?>

                            </div>
                                                                          <?php if(!isset($hour['values'][$column])) : ?>
                                <a href="<?php print(base_path());?>node/add/agreservation?edit[field_agres_ref_unit][nid][<?php print $unit->unitnid ?>]=<?php print $unit->unitnid ?>&edit[title]=Reservation&default_agres_date=<?php print $currentselectedday?> <?php print $hour['hour']; ?>">+</a>
                                                <?php endif; ?>
                        </div>
                    </td>
                                <?php else: ?>
                                <?php endif ?>
                            <?php endforeach; ?>
                        <?php endforeach; ?>
                    <?php endforeach; ?>

                </tr>
            </tbody>
        </table>
    </div></div>