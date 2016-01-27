<?php
/**
 * @file
 * Displays the Moon Phase page
 * 
 * Available variables:
 * - $prev: The previous day link
 * - $next: The next day link
 * - $day: The day of the year
 * - $image: The HTML image of the moon phase
 * - $definition: The description of the moon phase
 * - $table: A table of data regarding the moon phase
 * 
 * @ingroup themeable
 */
?>
<div id="moon_phase_page">
  <header id="moonav">
    <?php echo $prev; ?>
    <?php echo $day; ?>
    <?php echo $next; ?>
  </header>
  <div>
    <div class="moon-image"><?php echo $image; ?></div>
    <?php echo $definition; ?>
    <?php echo $table; ?>
  </div>
</div>