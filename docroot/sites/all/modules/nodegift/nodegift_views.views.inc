<?php
// $Id$
/**
 * @file
 * Provide views data and handlers for nodegift_views.module
 */


/**
* Implementation of hook_views_data().
*/
function nodegift_views_views_data() {
	$data = array();
	// ----------------------------------------------------------------------
	// node_gift table group

	$data['node_gift']['table']['group']  = t('Nodegift');

	// ----------------------------------------------------------------------
	// JOINS

	$data['node_gift']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid'
	);



	// ----------------------------------------------------------------------
	// FIELDS

	$data['node_gift']['nid'] = array(
	'title' => t('Node'),
	'help' => t('The node that whas gift'),
	'relationship' => array(
    'label' => t('nodegift'),
    'base' => 'node',
    'base field' => 'nid',
    // This allows us to not show this relationship if the base is already
    // node so users won't create circular relationships.
    'skip base' => array('node', 'node_revisions'),
	),
	);

    $data['node_gift']['gift_id'] = array(
        'title' => t('Gift id'),
        'help' => t('The id if a gift.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric'
        ),
        'filter' => array(
            'title' => t('Nodegift gift id'),
            'handler' => 'views_handler_filter_numeric',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        )
    );

	$data['node_gift']['comment'] = array(
	'title' => t('Gift comment'),
	'help' => t('The comment left the user to a gift.'),
	'field' => array(
	'handler' => 'views_handler_field_markup'
	)
	);

	$data['node_gift']['time_stamp'] = array(
	'title' => t('Gift sending date'),
	'help' => t('Date and time of sending of a gift.'),
	'field' => array(
	'handler' => 'views_handler_field_date',
	'click sortable' => TRUE,
	),
	'sort' => array(
	'handler' => 'views_handler_sort_date',
	),
	'filter' => array(
	'handler' => 'views_handler_filter_date',
	),
	);

	// FIELDS RELATIONSHIP
	// ----------------------------------------------------------------------
	$data['node_gift']['sid'] = array(
	'title' => t('Sender'),
	'help' => t("The user which has sent a gift."),

	'relationship' => array(
	'base' => 'users',
	'field' => 'sid',
	'handler' => 'views_handler_relationship',
	'label' => t('The user which has send a gift.'),
	),

	'filter' => array(
	'title' => t('Nodegift sender Name'),
	'handler' => 'views_handler_filter_user_name'),

	'argument' => array(
	'handler' => 'views_handler_argument_user_uid',
	),
	);

	$data['node_gift']['rid'] = array(
	'title' => t('Receiver'),
	'help' => t("The user which has received a gift."),
	'relationship' => array(
    'base' => 'users',
    'field' => 'rid',
    'handler' => 'views_handler_relationship',
    'label' => t('The user which has received a gift.'),
	),

	'filter' => array(
    'title' => t('Nodegift receiver Name'),
    'handler' => 'views_handler_filter_user_name',
	),
	'argument' => array(
    'handler' => 'views_handler_argument_user_uid',
	)
	);

	return $data;
}
