<?php
?>

<?php if ($prefix): ?>
  <div class="google-cse-results-prefix"><?php print $prefix; ?></div>
<?php endif; ?>

<?php if ($results_searchbox_form): ?>
  <?php print render($results_searchbox_form); ?>
<?php endif; ?>

<div id="google-cse-results">
  <noscript>
    <?php print $noscript; ?>
  </noscript>
</div>
<?php print variable_get('google_cse_custom_results_display', '') ?>
<?php if ($suffix): ?>
  <div class="google-cse-results-suffix"><?php print $suffix; ?></div>
<?php endif; ?>
