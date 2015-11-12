<?php


/**
 * Examples for using the Guidance action API.
 * The Guidance action API allows the module writer to extends the action set
 * of Guidance. In the module there is one sample: [page is visited]
 * [page is visited] checks if the user clicked on the guidance link
 * and visited the page.
 */
 
/**
 * Implementation of hook_guidance_calback().
 * Defines a callback that checks if the user created a content.
 */
function guidance_example_guidance_callback() {
	$callbacks['guidance_example_created_a_node'] = array(
		'info' => t('Create a content'),
		'help' => t('Create a content on the site. It helps you to discover the features of this page.'),
	);
	return $callbacks;
}

/**
 * Implementation of the defined callback: guidance_example_created_a_node.
 * Checks if the user created at least one node.
 *
 * @param array $link
 * @return boolean
 */
function guidance_example_created_a_node($link) {
  global $user;
  $res = db_fetch_object(db_query('SELECT COUNT(*) AS count FROM {node} WHERE uid = %d;', $user->uid));
  return $res->count > 0;
}
