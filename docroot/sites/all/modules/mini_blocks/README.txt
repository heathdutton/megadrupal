-- SUMMARY --

The Mini Blocks module allows administrators to create simple key/value elements
similar to typical Drupal Blocks, but with named keys rather than auto-increment
integers. This allows admins/content editors to create editable pieces of 
content with "keys" that can be programmatically called based on their names.

This module allows programmers to call editable pieces of content that are not
associated to a particular content type with keys that remain consistent from
all environments whether they be in different sequences in local, dev, test, 
stage, prod etc.

For a full description of the module, visit the project page:
  https://www.drupal.org/project/mini_blocks

To submit bug reports and feature suggestions, or to track changes:
  https://www.drupal.org/project/issues/2405355?status=All&categories=1


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/895232 for further information.

-- CONFIGURATION --

* Configure user permissions in Administration » People » Permissions:

  - Use the administration pages and help (System module)

    Structure -> Mini Blocks
	admin/structure/block

  - Call in my theme or module

    If you create a Mini Block named "contact_form_label" you would call this
	block's value like this:
	
	$contact_form_label = mini_block_get_value('contact_form_label');

	WARNING: You may want to filter XSS depending on how you use this output.

	The return value will be a string.

-- CONTACT --

Current maintainers:
* Cory Fischer (cfischer83) - https://drupal.org/user/1073722
