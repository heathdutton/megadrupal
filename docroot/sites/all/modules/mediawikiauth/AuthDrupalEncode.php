<?php

/*
 * AuthDrupalEncode.php 0.6, 2008-06
 * 
 * This produces an obfuscated string that is meant to be hard to spoof and
 * thus verifies that the login is legitimate.
 * 
 * The best way to do this would be to encrypt the username on the Drupal
 * side, transmit an encrypted string, and decrypt it here. However, haven't
 * found a portable encryption alternative. md5() and crypt() are both one-way
 * and cannot be de-crypted. mcrypt() is not installed with PHP by default, 
 * so many servers do not have it. 
 */


function authdrupal_encode($plain_string)
{
	// Admin must set the key to something unique to their site to prevent this
	// technique from being trivially spoofable--so refuse to encode if the 
	// key is unchanged
	if ( empty( $GLOBALS['wgAuthDrupal_security_key'] ) 
	     || ( 'ReplaceThisString' == $GLOBALS['wgAuthDrupal_security_key'] ) ) {
	     	return null;
	}
	
	// concatenate the given string with the secret key
	$str = $plain_string . $_SERVER["REMOTE_ADDR"] . $GLOBALS['wgAuthDrupal_security_key'];

	// sort the characters
	$a = str_split( $str, 1 );
	sort( $a, SORT_STRING );

	// turn back into string and scramble with md5()
	return md5( implode( '', $a ) );
}

/**
 * StaticUserLogout
 *
 * This is redundant code since the Drupal user logout hook code does
 * all this. I haven't decided where I want it in the long run, so I'm
 * keeping it in both places for now.--Maarten.
 *
 * Can't call User object functions from SetupAuthDrupal() because User.php
 * has not been included at that point. Hence, this is the code from
 * User::logout(), commenting out code that depends on having an actual User
 * object
 *
 * XXX NOTE this code is replicated in Mediawiki.module so if you edit it here,
 * see if you need to fix it there too. (Should really be shared.)
 */

function StaticUserLogout($prefix = null, $path = null, $domain = null) {
	if (is_null($prefix)) {
		$prefix = $GLOBALS['wgCookiePrefix'];
	}
	if (is_null($path)) {
		$path = $GLOBALS['wgCookiePath'];
	}
	if (is_null($domain)) {
		$domain = $GLOBALS['wgCookieDomain'];
	}
	// this lifted from wiki/includes/Setup.php which hasn't been included
	// when we need these
	if (is_null($prefix)) {
	  if ( $GLOBALS['wgDBprefix'] ) {
	    $prefix = $GLOBALS['wgDBname'] . '_' . $GLOBALS['wgDBprefix'];
	  }
	  elseif ( $GLOBALS['wgSharedDB'] ) {
	    // This is not supported yet--haven't researched it--Maarten.
	    // XXX should throw an error into watchdog log?
	    $prefix = $GLOBALS['wgSharedDB'];
	  } else {
	    $prefix = $GLOBALS['wgDBname'];
	  }
	}
        $GLOBALS['wgCookiePrefix'] = $prefix; // not sure this is necessary

	// $_SESSION['wsUserID'] = 0;  let's unset the cookie instead
	setcookie( $prefix . '_session', '', time() - 3600, $path, $domain, $GLOBALS['wgCookieSecure'] );

	setcookie( $prefix . 'UserName', '', time() - 3600, $path, $domain, $GLOBALS['wgCookieSecure'] );
	setcookie( $prefix . 'UserID', '', time() - 3600, $path, $domain, $GLOBALS['wgCookieSecure'] );
	setcookie( $prefix . 'Token', '', time() - 3600, $path, $domain, $GLOBALS['wgCookieSecure'] );

	# Remember when user logged out, to prevent seeing cached pages
	$ts_now = gmdate('YmdHis', time()); // emulates wfTimestampNow()
	setcookie( $prefix . 'LoggedOut', $ts_now, time() + 86400, $path, $domain, $GLOBALS['wgCookieSecure'] );
}
