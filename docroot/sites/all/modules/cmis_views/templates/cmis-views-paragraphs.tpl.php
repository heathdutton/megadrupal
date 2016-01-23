<?php
/**
 * @file
 * Theme implementation to display CMIS views as paragraphs.
 */
?>
<?php foreach ($variables['rows'] as $child) : ?>
  <div class="cmisviews">
  <h2><?php echo $child->properties["cmis:link"]; ?></h2>
  <p>
  <?php
  if ($child->properties['cm:description']) :
    echo $child->properties['cm:description'];
  endif;
  ?>

  <?php echo $variables['details']['format']; ?>:  <?php echo $child->properties["cmis:contentStreamMimeType"]; ?><br />
  <?php echo $variables['details']['size']; ?>: <?php echo $child->properties["cmis:contentStreamLength"]; ?><br /> 
  <?php echo $variables['details']['lastmodified']; ?>: <?php echo $child->properties["cmis:lastModificationDate"]; ?></p>
  </div>
<?php endforeach; ?>
