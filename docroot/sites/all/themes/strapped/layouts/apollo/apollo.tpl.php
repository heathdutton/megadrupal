<?php
/**
 * @file
 * Template - Hyperion
 */
?>
<div class="panel-display panel-four-three-adaptive clear-block" <?php print ((!empty($css_id)) ? "id=\"$css_id\"" : "")  ?>>
<!-- 4x4 grid -->
<?php
for ($j = 0; $j < 16; $j += 4):
  // Get the $content array keys of all non empty entries.
  $keys = array_keys(array_filter(array_slice($content, $j, 4, TRUE)));
  $h = strapped_adaptive_hash($keys, 4, 'strapped_adaptive_get_digit');
  // If all the row elements are empty then jump to the next row
  // immediately.
  if ($h == 0) continue;
  ?>

  <?php if ($h == 1): ?><!-- 100% - 1 column -->
  <div class="row">
    <div class="col-md-12">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h == 3): ?><!-- 25/75% - 2 columns -->
  <div class="row">
    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-9">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h == 4): ?><!-- 50/50% - 2 columns -->
  <div class="row">
    <div class="col-md-6">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-6">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h == 5): ?><!-- 75/25% - 2 columns -->
  <div class="row">
    <div class="col-md-9 la">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h ==6 || $h == 9): ?><!-- 25/25/50% - 3 columns -->
  <div class="row">
    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>

    <div class="col-md-6">
      <div class="inside">
        <?php print $content[$keys[2]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h == 7): ?><!-- 25/50/25% - 3 columns -->
  <div class="row">
    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-6">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>

    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[2]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h == 8): ?><!-- 50/25/25% - 3 columns -->
  <div class="row">
    <div class="col-md-6">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>

    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[2]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h == 10): ?><!-- 25/25/25/25% - 4 columns -->
  <div class="row">
    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>

    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[2]]; ?>
      </div>
    </div>

    <div class="col-md-3">
      <div class="inside">
        <?php print $content[$keys[3]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>
<?php endfor; ?>

<!-- 3x3 grid -->
<?php
for ($i = 16; $i < 25; $i += 3):
  // Get the $content array keys of all non empty entries.
  $keys = array_keys(array_filter(array_slice($content, $i, 3, TRUE)));
  $h = strapped_adaptive_hash($keys, 3, 'strapped_adaptive_get_digit');
  // If all the row elements are empty then jump to the next row
  // immediately.
  if ($h == 0) continue;
  ?>

  <?php if ($h == 1): ?><!-- 100% - 1 column -->
  <div class="row">
    <div class="col-md-12">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h == 3 || $h == 5): ?><!-- 33/66% - 2 columns -->
  <div class="row">
    <div class="col-md-4">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-8">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h == 4): ?><!-- 66/33% - 2 columns -->
  <div class="row">
    <div class="col-md-8">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-4">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>

  <?php if ($h == 6): ?><!-- 33/33/33% - 3 columns -->
  <div class="row">
    <div class="col-md-4">
      <div class="inside">
        <?php print $content[$keys[0]]; ?>
      </div>
    </div>

    <div class="col-md-4">
      <div class="inside">
        <?php print $content[$keys[1]]; ?>
      </div>
    </div>

    <div class="col-md-4">
      <div class="inside">
        <?php print $content[$keys[2]]; ?>
      </div>
    </div>
  </div>
<?php endif; ?>
<?php endfor; ?>
</div>
