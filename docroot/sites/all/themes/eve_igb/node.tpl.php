<hr />
<?php
if ($page == 0) {
	print "<h3><a href=\"$node_url\">$title</a></h3>\n<hr />\n";
}

// xxx Fix this stuff.
//	<div class="node<?php if ($sticky) { print " sticky"; }  if (!$status) { print " node-unpublished"; } "
//    <?php if ($picture) {
//      print $picture;
//    }
//     if ($page == 0) { <h2 class="title"><a href="print $node_url" print $title</a></h2><?php };
//

// Submitted: $submitted
// Taxonomy: $terms
// if links: $links

print $content;

if($links) {print $links;}

?>
<hr />

