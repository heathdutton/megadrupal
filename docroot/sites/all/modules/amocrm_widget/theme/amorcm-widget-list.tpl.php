<?php

/**
 * @file
 * Template for widget list.
 */
?>

<?php foreach($elements as $group_name => $widget): ?>
  <div class="panel-amo-widget">
      <div class="panel-amo-widget-head">
        <?php print render($group_name); ?>
        <div class="panel-amo-widget-caret"></div>
      </div>
      <div class="panel-amo-widget-body">
        <p>
          <ul>
            <?php foreach($widget as $link_info): ?>
              <li>
                <?php print render($link_info['content']); ?>
              </li>
            <?php endforeach; ?>
          </ul>
        </p>
      </div>
  </div>
<?php endforeach; ?>
