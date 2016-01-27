-- SUMMARY --

The HTPasswd Sync module let you synchronize a htpasswd and a htgroup file with
the user database.

For a full description of the module, visit the project page:
  http://drupal.org/project/htpasswdsync

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/htpasswdsync

-- REQUIREMENTS --

The syncrhonization only happen on password change. Hence, this module shall be 
installed before any user creation.

You need to run the cron.php job on a regular basis to ensure old users are 
properly cleaned up.

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure synchronized files in Administer >> User management >> HTPasswd Sync >>
  
  htpasswdsync module:

  - htpasswd file
    
    The file that will contain users and password, password are crypted, using
    the standard crypt function, with a random two charaters seed.

  - htgroup file
  
    The file that will synchronize the roles.
    
  - password hashing algorithm
    
    Let you choose how the password is encrypted/hashed. There are two options
    crypt and SHA-1.
    Crypt works only on Un*x platforms. SHA-1 shall work on bother Windows 
    based systems and Un*xes.
    
    WARNING: changing this value only change the way new or updated password
             are hashed.
             You will need to request you users to all change their password
             if you want to migrate from one hash to another.

  - roles
  
    The roles you want to export in the htgroup file.

  - overwrite
  
    Activate if you want to overwrite your htpassword file. I left inactive
    htpasswdsync will try its best to keep old entries, but will only try.
    
-- CUSTOMIZATION --

None.

-- TROUBLESHOOTING --



-- FAQ --


-- CONTACT --

Current maintainers:
* Marc Furrer (m.fu) - http://drupal.org/user/310415
* Stefan Wilhelm (7.x-xx) - http://drupal.org/user/1344522
