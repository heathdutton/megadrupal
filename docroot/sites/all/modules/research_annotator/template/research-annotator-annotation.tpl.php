<?php

/**
 * @file: Default theme implementation for research annotations.
 * 
 * @note: THIS TEMPLATE HEAVILY REACTS WITH research_annotator's JQUERY. REMOVIVING OR 
 *        CHANGING EXISTING ELEMENT ATTRIBUTES MAY BE DISASTEROUS! ADDING CSS CLASSES IS SAFE.
 *
 * Available variables:
 * - $author_picture: The picture of the annotation's author. 
 * - $author_name: The username of the annotation's author.
 * - $updated_date: The last time this annotation was updated.
 * - $markup: The content of the annotation as a renderable array.
 * - $edit_link: A link to the edit annotation page as a renderable array.
 * - $delete_link: A link to the delete annotation page as a renderable array.
 */
?>

<div class="annotation">
  <div class="annotation-profile">
    <?php print $author_picture; ?>
    <div class="author-info">
      <span class="annotation-author">
        <strong><?php print t('Annotated By'); ?>: </strong>
        <?php print $author_name; ?>
      </span>
      <span class="annotation-date">
        <strong><?php print t('Posted On'); ?>: </strong>
        <?php print $updated_date; ?>
      </span>
    </div>
  </div>
  <div class="annotation-markup">
    <?php print render($markup); ?>
  </div>
  <?php if($edit_link): ?>
    <?php print render($edit_link); ?>
  <?php endif; ?>
  <?php if($delete_link): ?>
    <?php print render($delete_link); ?>
  <?php endif; ?>
</div>