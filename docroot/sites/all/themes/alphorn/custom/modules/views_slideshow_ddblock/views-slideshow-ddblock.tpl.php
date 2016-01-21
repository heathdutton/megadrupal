<?php
// $Id$

/*
 * @file
 * Views Slideshow Dynamic display block module template
 *
 */  
?>
<!-- dynamic display block slideshow -->
<p>This preview does just shows the node titles of the nodes which will be show in the slideshow</p>
<?php foreach ($view->result as $row): ?>
  <?php print $row->node_title; ?>
  <br />
<?php endforeach; ?>
