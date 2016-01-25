<?php
// $Id$
/*
 * @file reel.tpl.php
 * Provides themed output for the reel formatter
 * @copyright Copyright(c) 2011 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html 
 * @author Lee Rowlands contact at rowlandsgroup dot com
 * 
 */
?>
<?php if (isset($rendered_image)): ?>
  <div class="reel-outer">
    <?php print $rendered_image;?>
  </div>
<?php endif; ?>