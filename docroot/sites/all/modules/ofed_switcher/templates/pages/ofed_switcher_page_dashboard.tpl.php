<?php
/**
 * @file
 * Dashboard template page.
 */
?>

<div class="dashboard span-12">
  <div class="span-4">
    <div class="dashbox tools blue ">
      <h2><?php echo t('Quick Infos') ?></h2>
      <div class="dashbox-content">
        <?php echo $dashboard['quick-info']; ?>
      </div>
    </div>
  </div>

  <div class="span-8">
    <?php if (!empty($dashboard['content'])): ?>
      <div class="dashbox span-12">
        <h2><?php echo t('Manage Content') ?></h2>
        <div class="dashbox-content">
          <ul>
            <?php foreach ($dashboard['content'] as $ct): ?>
              <li>
                <label><?php echo $ct['label'] ?></label>
                <?php echo $ct['links']; ?>
              </li>
            <?php endforeach; ?>
          </ul>
        </div>
      </div>
    <?php endif; ?>

    <div>
      <?php if (isset($dashboard['user'])): ?>
        <div class="dashbox orange span-6">
          <h2><?php print t('Manage User') ?></h2>
          <div class="dashbox-content">
            <ul>
              <?php foreach ($dashboard['user'] as $label => $links): ?>
                <li>
                  <label><?php echo $label; ?></label>
                  <?php echo l(key($links), $links[key($links)], array('html' => TRUE)); ?>
                </li>
              <?php endforeach; ?>
            </ul>
          </div>
        </div>
      <?php endif; ?>

      <?php if (isset($dashboard['block'])): ?>
        <div class="dashbox orange span-6">
          <h2><?php print t('Manage Blocks') ?></h2>
          <div class="dashbox-content">
            <ul>
              <?php foreach ($dashboard['block'] as $label => $links): ?>
                <li>
                  <label><?php echo $label; ?></label>
                  <?php echo l(key($links), $links[key($links)], array('html' => TRUE)); ?>
                </li>
              <?php endforeach; ?>
            </ul>
          </div>
        </div>
      <?php endif; ?>
    </div>
  </div>
</div>
