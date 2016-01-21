<div class="pm-dashboard-link well-sm">
  <div class="btn-group" role="group" aria-label="...">
    <a href="<?php echo $href ?>" class="btn btn-default btn-lg pm-dashboard-link-main">
        <i class="fa <?php echo $fa_icon; ?> fa-fw"></i>
          <span>
            <?php echo $title ?>
          </span>
    </a>
    <?php if ($href_add ): ?>
      <a href="<?php echo $href_add ?>" class="btn btn-info btn-lg pm-dashboard-link-add">&nbsp;<i class="fa fa-plus fa-fw"></i>&nbsp;</a>
    <?php endif ?>
  </div>
</div>
