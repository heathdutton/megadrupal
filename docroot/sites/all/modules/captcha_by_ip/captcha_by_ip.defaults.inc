<?php
// $Id$

/**
 * @file
 * Defaults file for captcha_by_ip.
 * Provides a way for users to distribute defaults that are loaded in module install.
 */

$defaults = array();

/**
 * Set defaults using the following example:
 * Example: $defaults['forms']['myform_id'] = array('operation' => 'exclude', 'ip_range' => 'myIPrestrictions');
 * Example2: $defaults['forms']['myform_id2'] = array('operation' => 'enforce', 'ip_range' => 'myIPrestrictions2');
 * NOTE: If captcha is not enabled on these forms before installing this module,
 * they will not be saved.
 */

/**
 * Returns the $defaults to the install file.
 */
return $defaults;
