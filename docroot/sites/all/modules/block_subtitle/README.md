Block Subtitle
===
This module allows subtitle to be set to blocks.

### Installation

1. Enable the Block Subtitle module under Administer >> Site Building >> Modules
2. Once enable you need to edit block.tpl.php in your theme. Add the following code to show the block subtitle. Create the block.tpl.php in your theme if it does not exist.

        <?php if ($block->subtitle): ?>
            <h3 class="subtitle"><?php print $block->subtitle; ?></h3>
        <?php endif; ?>
        
### Project page

http://drupal.org/project/block_subtitle
