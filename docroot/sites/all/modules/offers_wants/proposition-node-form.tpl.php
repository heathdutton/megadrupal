<?php
  $form = &$variables['form'];
  unset($form['comment_settings']);
?>
<div class="node-form">
  <div style="width:49%;float:left">
  <?php print drupal_render($form['want']); ?>
    <?php print drupal_render($form['title']); ?>
    <?php print drupal_render($form['body']); ?>

    <?php print drupal_render($form['offers_wants_categories']); ?>
    <?php print drupal_render($form['offers_wants_types']); ?>
  </div>

  <div style="width:49%;float:right">
    <?php print drupal_render($form['author']); ?>
    <?php print drupal_render($form['image']); ?>
    <?php print drupal_render($form['end']); ?>
  </div>

<br style = "clear:both;" />
<?php print drupal_render_children($form); ?>

<div class="options">
  <?php print drupal_render($form['additional_settings']); ?>
</div>

  <?php print drupal_render($form['actions']); ?>
</div>
