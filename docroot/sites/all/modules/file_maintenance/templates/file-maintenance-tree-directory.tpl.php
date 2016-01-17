<?php

/**
 * @file
 * Tree directory template.
 */

// $Id$

  $start_level = 0;
?>

<div class="tree-level-<?php print $level ?> clearfix" style="clear: both;">
	<div id="<?php print $dir_id?>" class="tree-directory-row clearfix" onclick="FileManager.toggle(this)"><?php

		for ($i=$start_level; $i<=$level; $i++):
			if ($i == $level):
				?><div class="tree-box toggle-element"><?php print $toggle_element ?></div><?php
			else:
				?><div class="tree-box"></div><?php
			endif;
		endfor;
		?><div class="tree-box"><?php print $icon_element ?></div><?php
		?><div class="directory-name  <?php print $filename_classes; ?>"><?php
			print "$dirname";
		?></div><div class="dir-info"><?php
		?><div class="info-column file-count-column"><?php print $file_count ?></div>
		<div class="info-column file-count-deep-column"><?php print $file_count_deep ?></div>
		<div class="info-column files-size-column"><?php print format_size($files_size) ?></div>
		<div class="info-column files-size-deep-column"><?php print format_size($files_size_deep) ?></div>
		</div>
	</div>

	<div class="tree-children tree-children-<?php print $level ?><?php print ($expanded ? ' expanded' : '') ?>">
		<?php print render($subdirs) ?>
	</div>
</div>


