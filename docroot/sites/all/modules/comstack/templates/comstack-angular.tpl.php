<div<?php print $attributes; ?>>
  <?php if ($url): ?>
  <div ng-include src="'<?php print $url; ?>'"></div>
  <?php endif; ?>
  <?php if ($ui_view): ?>
  <ui-view<?php print drupal_attributes($ui_view_attributes); ?>></ui-view>
  <?php endif; ?>
</div>
