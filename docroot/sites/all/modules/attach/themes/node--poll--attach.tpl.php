<?php if ($view_mode == 'attach'): ?>
<h3><?php print $attach_link ?></h3>
<div class="attach-content attach-node-poll" id="<?php print $node->attach['uid'] ?>">
  <?php
  if ($node->choice) {
    $list = array();
    foreach ($node->choice as $choice) {
      $list[] = check_plain($choice['chtext']);
    }
    print theme_item_list($list);
  }
  ?>
</div>
<script type="text/javascript">
<!--//--><![CDATA[//><!--
var EJS = EJS || {};
EJS.attach = EJS.attach || [];
EJS.attach['<?php print $node->attach['uid'] ?>'] = '<?php print url('attach/node/' . $node->nid, array('query' => array('destination' => '__FAKED__'))) ?>';
//--><!]]>
</script>
<?php else: ?>
  <?php print render($content) ?>
<?php endif ?>

