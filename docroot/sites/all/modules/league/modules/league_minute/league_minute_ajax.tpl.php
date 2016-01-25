<?php

/**
 * @file
 * A basic template for league_game entities
 *
 * Available variables:
 * - $items: An array of comment items. Use render($content) to print them all, or
 * - $game: The game id
 * - $label: The field label
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */
?>
  <h3><?php print render($label); ?></h3>
  <dl class="content">
    <?php foreach ($items as $delta => $item): ?>
      <?php print render($item); ?>
    <?php endforeach; ?>
  </dl>
