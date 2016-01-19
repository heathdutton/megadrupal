<?php
/**
 * @file
 * Theme implementation to display CMIS views with a thumbnail and details.
 */
?>
<ul class="posts cmisposts">
<?php foreach ($variables['rows'] as $child) : ?>
<li>
  <div class="block-post">
    <?php echo $child->properties["cmis:link-image"]; ?>
    <div class="cmisdetails">
      <strong class="title"><?php echo $child->properties["cmis:link-title"]; ?></strong>
      <?php
      if ($child->properties['cm:description']) :
        echo $child->properties['cm:description'];
      endif;
      ?>
      <div class="block-open">
        <ul class="list">
          <li><?php echo $variables['detailheadings']['lastmodified']; ?>: 
            <strong><?php echo $child->properties["cmis:lastModificationDate"]; ?></strong>
            &nbsp;&nbsp; | &nbsp;&nbsp; <?php echo $variables['detailheadings']['size']; ?>: <strong><?php echo $child->properties["cmis:contentStreamLength"]; ?></strong>
          </li>
        </ul>
      </div>
    </div>
  </div>
</li>
<?php endforeach; ?>
</ul>
<div style="clear:both"></div>
