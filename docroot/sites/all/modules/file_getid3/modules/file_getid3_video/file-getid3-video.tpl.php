<?php
/**
 * @file
 * Returns the HTML for a HTML5 video player.
 */
?>

<?php if ($items): ?>
  <div class="<?php print $classes; ?>"<?php print $attributes;?>>
    <div style="padding-bottom:<?php print $padding_bottom; ?>;">
      <video class="<?php print $video_classes; ?>"<?php print $video_attributes;?>>
        <?php foreach ($items as $item): ?>
          <source src="<?php print check_plain(file_create_url($item['uri'])); ?>" type="<?php print check_plain($item['filemime']); ?>" />
        <?php endforeach; ?>
      </video>
    </div>
  </div>
<?php endif; ?>
