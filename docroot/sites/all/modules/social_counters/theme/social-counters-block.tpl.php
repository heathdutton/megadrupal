<?php
/**
 * @file
 * Output of Social Counters in block.
 */
?>

<?php foreach($items as $data): ?>
  <div class="<?php print $data['classes']; ?>">
    <span class="title"><?php print $data['title']; ?><?php print $data['delimiter']; ?></span>
    <span class="number"><?php print $data['number']; ?></span>
  </div>
<?php endforeach; ?>