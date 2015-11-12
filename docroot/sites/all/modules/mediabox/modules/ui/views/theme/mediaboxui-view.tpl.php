<?php
/**
 * Browse view template.
 */
?>
<div id="mediabox-ui-library">
  <div id="mediabox-ui-library-wrapper">
    <div id="mediabox-ui-library-content">
      <div id="mediabox-ui-library-content-wrapper">
        <div id="mediabox-ui-library-list-wrapper">
          <?php foreach ($rows as $delta => $row): ?>
            <div id="<?php print $ids[$delta]; ?>" class="mediabox-selectable">
              <?php print $row; ?>
            </div>
          <?php endforeach; ?>
        </div>
      </div>
    </div>
  </div>
</div>
