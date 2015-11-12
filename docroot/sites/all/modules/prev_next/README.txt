
Copyright 2009 http://2bits.com

An API for browsing next/previous nodes without overloading your database server.

Description
===========
This module allows you to know the previous or next nodes for any given node. This
is very useful for providing navigational links to the user without the expensive
queries required to dynamically deduce such information on the fly.

The use case is two fold:

Usability/Navigation
--------------------
For example, on a site with a gallery of images, you want to show a next/previous link
with a thumbnail under each image. Your site's visitor click on the link to show new
content or browse it.

Scalability
-----------
Although the previous and next nodes can be deduced with some SQL work, the queries to
do so are very heavy on the database, and can bring a site to its knees. This module
solves this problem by storing the previous/next node in a table so lookups are fast.
Once the module is installed, it will build this index backwards via cron until all
nodes have been indexed.

Configuration
=============
The module can be restricted to certain content types to be included in the previous/next
indexing. For example, you want the site's visitors to browse through video and image nodes
only, but not blogs and regular pages.

The number of nodes to index is defined in the settings too. The default is 200, but you may
want to lower that for a site on shared hosts. Once the indexing is complete for all the site's
nodes, cron will do do anything. You can always reindex the site using the "Re-Index" button
on the settings page.

API
===

The module provides only one API call. If you do not call that function from another module
or your theme, this module will do nothing.

<?php
$n_nid = prev_next_nid($nid, $op);
?>

Examples for using it are:

<?php
// Get the previous node id
$prev_nid = prev_next_nid($nid, 'prev');

// Get the previous node id
$next_nid = prev_next_nid($nid, 'next');
?>

Example
-------
To implement the functionality for this module for nodes that are of content
type 'image' or 'video', add the following function in your template.php file,
or a custom module:

<?php
function pn_node($node, $mode = 'n') {
  if (!function_exists('prev_next_nid')) {
    return NULL;
  }

  switch($mode) {
    case 'p':
      $n_nid = prev_next_nid($node->nid, 'prev');
      $link_text = 'previous';
      break;

    case 'n':
      $n_nid = prev_next_nid($node->nid, 'next');
      $link_text = 'next';
      break;

    default:
      return NULL;
  }

  if ($n_nid) {
    $n_node = node_load($n_nid);

    $options = array(
      'attributes' => array('class' => 'thumbnail'),
      'html'  => TRUE,
    );
    switch($n_node->type) {
      // For image nodes only
      case 'image':
        // This is an image node, get the thumbnail
        $html = l(image_display($n_node, 'thumbnail'), "node/$n_nid", $options);
        $html .= l($link_text, "node/$n_nid", array('html' => TRUE));
        return $html;

      // For video nodes only
      case 'video':
        foreach ($n_node->files as $fid => $file) {
          $html  = '<img src="' . base_path() . $file->filepath;
          $html .= '" alt="' . $n_node->title;
          $html .= '" title="' . $n_node->title;
          $html .= '" class="image image-thumbnail" />';
          $img_html = l($html, "node/$n_nid", $options);
          $text_html = l($link_text, "node/$n_nid", array('html' => TRUE));
          return $img_html . $text_html;
        }
      default:
        // Add other node types here if you want.
    }
  }
}
?>

Then in your node-image.tpl.php and node-video.tpl.php you call this function as follows:

<code>
  <ul id="node-navigation">
    <li class="next"><?php print pn_node($node, 'n'); ?></li>
    <li class="prev"><?php print pn_node($node, 'p'); ?></li>
  </ul>
</code>

Alternately, you can also add the calls to pn_node() in the phptemplate_preprocess_node()
function of your template.php file for the content types you are interested in.

Bugs/Features/Patches:
----------------------
If you want to report bugs, feature requests, or submit a patch, please do so
at the project page on the Drupal web site.
http://drupal.org/project/prev_next

Author
------
Khalid Baheyeldin (http://baheyeldin.com/khalid and http://2bits.com)

If you use this module, find it useful, and want to send the author
a thank you note, then use the Feedback/Contact page at the URL above.

The author can also be contacted for paid customizations of this
and other modules.

