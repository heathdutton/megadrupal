<?php
// $Id$

/**
 * @file
 * Display the two specs.
 *
 * Available variables:
 * - $primarySpec, $secondarySpec:
 * Array
 * (
 *     [icon] => [full html image of the icon] 
 *     [iconURL] => spell_deathknight_unholypresence.jpg
 *     [spec] => Unholy (4/0/32)
 *     [active] => 1
 * )
 * - $health: Maximum amount of health.
 * - $powerTypeId: Numberic power type  (matching wow armory)
 * - $powerType: Text power type (mana/runic power/focus/etc)
 * = $power: Numberic value of maximim
 * 
 * Other variables:
 * - $toon: The Toon being displayed
 * - $set: The current set being displayed.
 *
 * @see template_preprocess_wowtoon_specs()
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */
?>

    <table id="wowtoon-spec-table" class="summary-health-resource">
    <tr>
      <td width="30%"><div class="bar health-bar"><?php echo $health; ?></div></td>
      <td width="35%" rowspan="2">
        <div<?php if ($primarySpec['active']) echo ' class="active-spec"'; ?>>
        <?php echo $primarySpec['icon']; echo $primarySpec['spec']; ?>
        </div>
      </td>
      <td width="35%" rowspan="2">
        <div<?php if ($secondarySpec['active']) echo ' class="active-spec"'; ?>>
        <?php echo $secondarySpec['icon']; echo $secondarySpec['spec']; ?>
        </div>
      </td>
    </tr>
    <tr>
      <td>
        <div class="bar resource-<?php echo $powerTypeId; ?>">
          <span class="power-text"><?php echo $powerType; ?></span>
          <span class="power-number"><?php echo $power; ?></span>
        </div>
      </td>
    </tr>
    </table>
