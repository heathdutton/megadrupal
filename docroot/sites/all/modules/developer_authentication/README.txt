CONTENTS OF THIS FILE
---------------------

 * About Developer authentication
 * Configuration

ABOUT DEVELOPER AUTHENTICATION
------------------------------

Allows developers to log in as user #1 with user accounts from LDAP. Can 
also be used without LDAP to login with a config-set (eg. settings.php)
password.

Useful for administering a large cluster of multisite sites that share the
all/modules directory. Just configure the module and enable it on all sites;
no need to remember the user #1 password any more.

Note: You definitely shouldn't use this unless you fully understand how the
module works! Incorrect usage can seriously harm your site's security!

CONFIGURATION
-------------

You can set these variables either in settings.php or for example via drush
vset.

 * developer_authentication_admin_pwd (optional, default: FALSE)
   SHA1 hash for single user LDAPless admin login.
   Note: MAKE SURE THIS PASSWORD IS STRONG ENOUGH; IT OFFERS FULL ADMIN
   ACCESS TO THE SITE! Also remember to change the password regularly.
 * developer_authentication_ldap_server (default: localhost)
   LDAP server host name.
 * developer_authentication_ldap_port (default: 389)
   LDAP server port.
 * developer_authentication_username_field (default: cn)
   LDAP username field.
 * developer_authentication_basedn (default: ou=members,dc=example,dc=org)
   LDAP base DN
