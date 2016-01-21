<?php
/**
 * @file
 * Prints an quora question/post.
 */
?>

<p><div class="quora-title" >
  <a href = <?php print $url; ?> target="_blank">
    <?php print $title; ?>
  </a>
</div>
<?php if(isset($snippet)): ?>
<div class="quora-snippet">
  <?php print $snippet; ?>
</div>
<?php endif; ?></p>
