<?php
  $item = $variables['widget_menu_children']['#item'];
?>
<li>
  <a class="<?php echo $item['operations']['menu_class']; ?>" href="<?php echo $item['menu']['href']; ?>">
    <i class="fa <?php echo $item['operations']['fa_icon'] ?> fa-fw"></i> <?php echo $item['menu']['title']; ?>
  </a>
  <div class="well-sm">
    <ul class="nav" role="menu">
      <?php echo $variables['widget_menu_children']['#children']; ?>
    </ul>
  </div>
</li>
