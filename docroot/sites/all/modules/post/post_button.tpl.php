<?php
/**
 * @file
 * Theme implementation for Po.st service buttons.
 *
 * Available Variables
 * - $classes_container - Classes for div container.
 * - $classes_array - Classes for Po.st service div.
 * - $style - Inline sytles for div container.
 * - $services - Array of classes for enabled services.
 * - $settings - Settings saved from the admin form at admin/config/content/post.
 *
 * @see template_preprocess_post_button()
 */
?>
<div class="post-container">
  <div class="<?php echo implode(' ', $classes_container); ?>" style="<?php echo $style; ?>">
    <div class="<?php echo implode(' ', $classes_array); ?>">
      <?php foreach ($services as $service): ?>
      <a class="<?php echo $service; ?>"></a>
      <?php endforeach; ?>
    </div>
    <script src="http://i.po.st/share/script/post-widget.js#publisherKey=<?php echo $settings['publisher_key']; ?>" type="text/javascript" charset="utf-8"></script>
  </div>
</div>
