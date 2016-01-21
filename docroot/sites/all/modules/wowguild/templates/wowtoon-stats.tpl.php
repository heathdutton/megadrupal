<?php
// $Id$

/**
 * @file
 * Display the stats block.
 *
 * Available variables:
 * - $all_stats: An array of stats
 *   Example Array (
 *   [Base] => Array
 *       (
 *           [strength] => Array
 *               (
 *                   [text] => Strength 
 *                   [value] => 2718
 *               )
 *           [agility] => Array
 *               (
 *                   [text] => Agility 
 *                   [value] => 192
 *               )
 *               :
 *               :
 *       )
 *
 *   [Other] => Array
 *       (
 *           [meleeattackpower] => Array
 *               (
 *                   [text] => Melee Attack Power 
 *                   [value] => 5671
 *               )
 *           [expertise] => Array
 *               (
 *                   [text] => Expertise 
 *                   [value] => 13 / 13
 *               )
 *               :
 *               :
 *       )
 *
 *   [Melee] => Array
 *       (
 *           [meleedamage] => Array
 *               (
 *                   [text] => meleedamage
 *                   [value] => 3089 – 3788
 *               )
 *           [meleedps] => Array
 *               (
 *                   [text] => Damage per Second
 *                   [value] => 935.2 / 590.2
 *               )
 *               :
 *               :
 *       )
 * )
 * 
 * Other variables:
 * - $toon: The Toon being displayed
 * - $set: The current set being displayed.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */

?>
<table id="wowtoon-stats-table">
<?php 
  
  foreach ($all_stats as $key => $stat) {
    echo "<tr>";
    echo '<td class="stat-key">' . $stat['text'] . '</td><td class="stat-value">' . $stat['value'] . "</td>";
    echo "</tr>";
  }
  
  ?>
  </table>