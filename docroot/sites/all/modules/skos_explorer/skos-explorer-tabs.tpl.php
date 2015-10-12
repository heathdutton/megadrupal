<?php
/**
 * Display jquery.ui tabs
 */
?>

<div id="skos-explorer-tabs">
  <h2><?php print $label; ?></h2>
<ul>
<li><a href="#tabs-1"><?php print t('Definition'); ?></a></li>
<li><a href="#tabs-2"><?php print t('Broader'); ?></a></li>
<li><a href="#tabs-3"><?php print t('Narrower'); ?></a></li>
<li><a href="#tabs-4"><?php print t('Related'); ?></a></li>
</ul>
<div id="tabs-1">
<?php print $definition; ?>
</div>
<div id="tabs-2">
<?php print $broader; ?>
</div>
<div id="tabs-3">
<?php print $narrower; ?>
</div>
<div id="tabs-4">
<?php print $related; ?>
</div>
</div>


