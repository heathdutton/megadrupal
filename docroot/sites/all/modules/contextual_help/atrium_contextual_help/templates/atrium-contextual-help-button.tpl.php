<?php
/**
 * @file
 * Provides view for contextual help button.
 *
 * $contextual_help - Rendered list of modal links.
 */
?>
<ul id="atrium_contextual_help">
  <li class="dropdown btn-group">
    <a class="dropdown-toggle btn <?php print $oa_toolbar_btn_class; ?>" id="contextual-help-dropdown" data-toggle="dropdown" href="#" title="<?php print $title?>">
      <i class="icon-question-sign"></i>
    </a>
    <ul class="dropdown-menu" role="menu" aria-labelledby="space-dropdown">
      <li class="dropdown-column">
        <div class="item-list">
          <h3><?php print $title; ?></h3>
            <?php print $contextual_help; ?>
        </div>
      </li>
    </ul>
  </li>
</ul>
