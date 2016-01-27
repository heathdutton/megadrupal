<?php


/**
 * @file
 * global definitions.
 */

/**
 * Definitions class for tumblr connect module.
 */
class TumblrConnectDefinitions {

  const TUMBLR_EMAIL = 'tumblr_email';
  const TUMBLR_PASSWORD = 'tumblr_password';
  const TUMBLR_CONNECT_TYPES = 'tumblr_allowed_types';
  const SUCCESS = 'success'; // something to return when we simple want to signnal operation completed
  const BATCH_SCHEDULE = 'action scheduled to be performed later.';
  const CACHE_TABLE_NAME = 'cache_tumblr_connect';
  const CACHED_LIST_CID = 800;
  const CACHED_FOLLOWERS_CID = 801;
  const CACHED_THEMED_FOLLOWERS_CID = 802;
  const TUMBLR_UNAVAILABLE_MESSAGE = 'Tumblr may not be available right now, the system will update Tumblr later.';
  const DB_RECORD_NOT_FOUND = 901;
  const NUM_FOLLOWERS_TO_DISPLAY = 'tumblr_block_items';
  const FOLLOWERS_BLOCK_NUM_COLS = 'tumblr_block_num_columns';
}

/**
 * Definitions class for tumblr web service communications.
 */
class TumblrGateway {

  const DASHBOARD = 'http://www.tumblr.com/api/dashboard';
  const WRITE_POSTS = 'http://www.tumblr.com/api/write';
  const DELETE_POSTS = 'http://www.tumblr.com/api/delete';
  const AUTHENTICATE = 'http://www.tumblr.com/api/authenticate';
  const FOLLOWERS = 'http://www.tumblr.com/followers';
  const LOGIN = 'http://www.tumblr.com/login';
  const INVALID_REMOTE_REQUEST = 'errorMessage';
  const USER_AGENT = 'Drupal (+http://drupal.org/)';
  const EMAIL_PARM = 'email';
  const PASSWORD_PARM = 'password';
}

/**
 * Definitions class for tumblr web service content types.
 */
class TumblrPostTypes {

  const LINK = "link";
  const REGULAR = "regular";
}