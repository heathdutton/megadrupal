<div id="devcontrol" class="expanded">
  <div id="devcontrol-expand">
    <a href="#" title="<?php echo t("Expand"); ?>">
    </a>
  </div>
  <span class="text title">
    <?php echo $text; ?>
  </span>
  <ul id="devcontrol-items">
    <?php foreach ($menus as $key => $item): ?>
    <li id="devcontrol-item-<?php echo $key; ?>" title="<?php echo $item['name']; ?>" class="devcontrol-item">
      <span class="name">
        <?php echo $item['name']; ?>
      </span>
      <?php if (isset($item['children']) && $item['children']): ?>
      <ul class="devcontrol-dropdown">
        <?php foreach ($item['children'] as $link): ?>
        <li>
          <?php echo render($link); ?>
        </li>
        <?php endforeach; ?>
      </ul>
      <?php endif; ?>
    </li>
    <?php endforeach; ?>
  </ul>
</div>