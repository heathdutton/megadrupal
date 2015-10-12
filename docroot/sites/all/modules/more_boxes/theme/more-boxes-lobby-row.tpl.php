<div class="connecting-looby-row">
  <?php if ($title) : ?>
    <div class="con-title">
      <?php print $title;?>
    </div>
  <?php endif;?>
  <?php if ($description) : ?>
    <div class="con-desc">
      <?php print $description;?>
    </div>
  <?php endif;?>
  <?php if ($view) : ?>
    <div class="con-view">
      <?php print $view;?>
    </div>
  <?php endif;?>
	<?php if ($more_link) : ?>
    <div class="con-more">
      <?php print $more_link;?>
    </div>
  <?php endif;?>
</div>