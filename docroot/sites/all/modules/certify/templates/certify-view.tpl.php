<?php foreach ($conditionsbytype as $condtype => &$conditions): ?>
  <h3><?php print call_user_func(array(get_class($conditions[0]), 'getViewHeader')) ?></h3>
  <ul class="certify_condlist"></ul>
  <?php foreach ($conditions as &$condition): ?>
    <li><?php print $condition->certify_view($node) ?></li>
  <?php endforeach ?>
  </ul>
<?php endforeach ?>