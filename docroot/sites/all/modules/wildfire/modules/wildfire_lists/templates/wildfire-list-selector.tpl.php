<?php
/**
 * @file
 *  Template for the list selector on the send page of a broadcast.
 */
?>
<div class="description">
  <?php print $helptext; ?>
</div>
<div class="wildfire-list-col-name">
  List title
</div>
<div class="wildfire-list-col-size">
  List size
</div>
<ul id="wildfire-list-selector">
  <?php if (count($lists)): ?>
    <?php foreach($lists as $lid => $list): ?>
      <li id="wildfire-list-<?php print $lid; ?>" class="unselected">
        <div class="wildfire-list-name">
          <?php print $list['name']; ?>
        </div>
        <div class="wildfire-list-count description">
          <p><?php print $list['count']; ?></p>
        </div>
      </li>
    <?php endforeach;?>
  <?php else: ?>
    <li>
      <?php print t(
        'There are no lists. !add',
        array(
          '!add' => l(
            t('Add one?'),
            'admin/wildfire/lists/add',
            array('attributes' => array('title' => t('Add a new list')))
          ),
        )
      ); ?>
    </li>
  <?php endif; ?>
</ul>
<div id="wildfire-list-remove">
  <a href="#" title="<?php print $removehelp; ?>">
    <?php print $removelink; ?>
  </a>
</div>
