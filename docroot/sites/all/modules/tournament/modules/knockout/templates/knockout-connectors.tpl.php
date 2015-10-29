<?php if ($type == 'straight-line') : ?>
<div class="straight-line<?php print $first . $last; ?>"></div>
<?php else : ?>
<div class='bracket-line-top<?php print $first; ?>'></div>
<div class='bracket-top-connector'></div>
<div class='bracket-bottom-connector'></div>
<div class='bracket-line-bottom<?php print $last; ?>'></div>
<?php endif; ?>
