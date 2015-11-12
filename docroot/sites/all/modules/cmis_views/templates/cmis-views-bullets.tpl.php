<?php
/**
 * @file
 * Theme implementation to display CMIS views as bullets.
 */
?>
<ul class="cmisviews">
<?php foreach ($variables['rows'] as $child) : ?>
  <li><?php echo $child->properties["cmis:link"]; ?> (<?php echo $child->properties["cmis:contentStreamMimeType"]; ?>, <?php echo $child->properties["cmis:contentStreamLength"]; ?>)</li>
<?php endforeach; ?>
</ul>
