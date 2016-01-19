<?php
/**
 * @file
 *    Provides the descriptions on the list of templates page.
 *    
 *    Includes description of the template plus a list of the template's
 *    regions, if any.
 */
?>
<p class="description"><?php print $template['description']; ?></p>
  
<?php if (is_array($regions)): ?>
  <?php foreach ($regions as $region): ?>
    <p class="description">
      <?php print $region['name']; ?>:
      <?php print $region['content']; ?>
    </p>
  <?php endforeach; ?>
<?php endif; ?>