<?php
/**
 * @file
 * Implementation for appointcal page.
 */
drupal_add_js(drupal_get_path('module', 'appointment_calendar') . '/js/appointment_calendar.js');
?>
<div>
    <?php
    $block = module_invoke('views', 'block_view', 'appointment_calendar-block');
    print render($block['content']);
    ?>
</div>
