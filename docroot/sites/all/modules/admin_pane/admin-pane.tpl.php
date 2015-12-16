<?php
// $Id: admin-pane.tpl.php,v 1.1 2011/01/18 07:32:54 blixxxa Exp $
?>
<div id="admin-pane">
  <div id="admin-pane-shadow"></div>
  <div id="admin-pane-content">

    <?php if (!empty($local_tasks)): ?>
    <div class="local-tasks">
      <ul class="actions clearfix">
        <?php foreach ($local_tasks as $task): ?>
          <?php print render($task); ?>
        <?php endforeach; ?>
      </ul>
    </div>
    <?php endif; ?>

    <div id="menu-tabs">
      <ul id="menu-tabs-tabs" class="clearfix">
        <?php foreach ($tabs as $tab): ?>
          <li><a href="<?php print $tab['href']; ?>"><?php print $tab['title']; ?></a></li>
        <?php endforeach; ?>
      </ul>
      <?php foreach ($menus as $menu): ?>
        <div id="<?php print $menu['tab_id']; ?>"><?php print render($menu['content']); ?></div>
      <?php endforeach; ?>
    </div>

  </div>
</div>