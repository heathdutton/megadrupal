<?php

/**
 * @file All constants that are used throughout the FreshBooks module code.
 *
 * @author Guy Paddock (guy.paddock@redbottledesign.com)
 * @author Xavier L. (xavier@openconcept.ca)
 */

  /**
   * The name of the folder containing the FreshBooks-API library.
   *
   * @var string
   */
  define('FRESHBOOKS_LIBRARY_NAME', 'freshbooks-api');

// Variables
  /**
   * The name of the variable that contains the service URL for processing FreshBooks requests.
   *
   * @var string
   */
  define('FRESHBOOKS_VAR_DOMAIN', 'freshbooks_domain');

  /**
   * The name of the variable that contains the unique authentication token FreshBooks assigns for authenticating
   * requests.
   *
   * @var string
   */
  define('FRESHBOOKS_VAR_TOKEN', 'freshbooks_token');

// Paths
  /**
   * Menu path for FreshBooks module settings.
   *
   * @var string
   */
  define('FRESHBOOKS_PATH_SETTINGS', 'admin/config/system/freshbooks');
