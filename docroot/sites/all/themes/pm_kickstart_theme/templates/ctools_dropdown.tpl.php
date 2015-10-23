<!-- Split button -->
<div class="btn-group btn-group-xs">
  <a href="<?php echo $default_link['url']; ?>" class="btn btn-default <?php echo $default_link['class'] ?>"><?php echo $default_link['title']; ?></a>
  <?php if (!empty($dropdown_menu)): ?>
    <button type="button" class="btn btn-default dropdown-toggle" data-toggle="dropdown" aria-expanded="false">
      <span class="caret"></span>
      <span class="sr-only">Toggle Dropdown</span>
    </button>
    <ul class="dropdown-menu" role="menu">
      <?php foreach ($dropdown_menu as $link): ?>
        <?php if (!empty($link)): ?>
          <li>
            <a href="<?php echo $link['url']; ?>" class="<?php echo $link['class']; ?>"><?php echo $link['title']; ?></a>
          </li>
        <?php endif ?>
      <?php endforeach ?>
    </ul>
  <?php endif ?>
</div>
