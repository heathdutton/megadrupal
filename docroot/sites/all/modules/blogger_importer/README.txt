Blogger Importer module for Drupal

by Ben Buckman (thebuckst0p)
Created August-September 2010

This module is meant to import a blog from Blogger/Blogspot into Drupal.
It comes with NO WARRANTY, GUARANTEE, OR TECHNICAL SUPPORT OF ANY KIND.
This module was created to serve a personal use case and IS VERY LIKELY TO HAVE BUGS.
USE ENTIRELY AT YOUR OWN RISK.

Requirements:
* Blogger XML export (uploaded to server)
* QueryPath module and library to parse the XML

Instructions:
# Export your blog from Blogger (see http://www.google.com/support/blogger/bin/answer.py?hl=en&answer=97416)
# Backup your database.
# Enable the module.
# Set the permissions for 'import blogger blog'
# Upload the exported XML to a directory on your server that Drupal can access.
# Go to /admin/content/blogger_importer.
# Enter the path to the XML file you uploaded.
# Click 'Save & Process Export XML'.
# When the processing finishes, review the posts and comments that were found.
# Click 'Create nodes from imported entries'
# Review the nodes that were created.
# Click 'Create comments from imported entries'
# Review the comments that were created.


API:
This module defines a HOOK_blogger_importer_node_alter to modify nodes being created from imported posts.
Implementation: <code>HOOK_blogger_importer_node_alter(&$node, $post, $op)</code>
- only $op implemented now is 'presave' but might want more later
- ignores return value
Example usage: map the original URL of each post to a redirect handler


Patches and other improvements are welcome!
