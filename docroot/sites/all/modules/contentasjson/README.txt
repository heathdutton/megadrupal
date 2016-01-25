=======
SUMMARY
=======
This module provides an easy way to retrieve data in JSON format through a
menu callback. Compared to Services this module focuses on JSON and doesn't
need configuring of resources, endpoint or authentication.

It's low level and most easily used with AJAX to get data of the following
content types:

- node
- comment
- taxonomy_term
- taxonomy_vocabulary
- custom_data

Each of these options provides a hook to alter the data-output, look at
phpDocs for implementation examples. If no hook is provided, the default
object will be returned (can be huge!)

A node hook example can look like this:

/**
 * Implementation of contentasjson_get_node() hook
 * @see _contentasjson_get_node();
 */
function mymodule_contentasjson_get_node($node) {
  $node_array = array();
  $node_array['id'] = $node->nid;
  $node_array['title'] = $node->title;
  
  return $node_array;
}

============
AJAX EXAMPLE
============
To retrieve data with the contentasjson module you can use AJAX. The
following example shows how to get the data with jQuery:

jQuery.ajax({
	type: "POST",
	url: "contentasjson/node/1",
	success:function(data)
	{
		console.log(data);
	}
});

Always fill in all values within the -url- parameter. There are 3:

- contentasjson (module name and standard first parameter)
- type (differs from list in summary)
- ID (if no ID for custom_data, provide 0 as default value)

=============
CONFIGURATION
=============
To configure this module you need to do the following:

1. Upload (or install) the module on the server at sites/all/modules
2. Enable the module on the admin/modules page
3. Set Permissions for the module on the
   admin/people/permissions#module-contentasjson page
4. Implement hooks & (AJAX) code and you're ready to go!

=======
CREDITS
=======
- Module is sponsored by dpdk
- More information from the process around this module can be found at
  my graduation website (Dutch!) http://drupal.antwan.eu