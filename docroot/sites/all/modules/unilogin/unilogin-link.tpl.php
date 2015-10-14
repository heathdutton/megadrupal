<?php
/**
 * @file
 * Template for rendering UNI•Login link.
 */
?>
<a href="<?php echo $login_url; ?>">
  <?php echo theme('image', array(
    'path' => $image_path,
    'alt' => t('Log in with UNI•Login'),
  )); ?>
</a>
