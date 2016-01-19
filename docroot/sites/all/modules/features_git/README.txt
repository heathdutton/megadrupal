Features to Git allows trusted users to write features directly to disk,
commit them to git and push them to a remote. This makes it possible for
site builders to be able to manage features in git without having to get
their hands dirty on the command line.

**WARNING** 
Only grant access to this module to trusted users. Any changes they make
will be written to disk and committed to git immediately. This module
requires relaxed filesystem permissions that allows the web server the
ability to write files to disk.

## Getting Started To set this module you must do the following things: 

1.  Download and enable the module 
    *   `drush dl features_git`
    *   `drush en features_git`
2.  Grant users access 
    *   visit `admin/people/permissions`
    *   grant "the Export a feature to git" permission to the appropriate
role/s
3.  Make ssh-echopass executable 
    *   `chmod +x
/path/to/drupal/sites/all/modules/contrib/features_git/ssh-echpass`
4.  Make your tree writable by Apache **(this has security implications)** 
    *   `cd /path/to/drupal`
    *   `find -type f -exec chmod 660 {} \;`
    *   `find -type d -exec chmod 2770 {} \;`
    *   `chown -R :www-data .` # (change www-data to the group Apache/nginx
runs as)
5.  Add your SSH private keys for each user to `~www-data/.ssh (assuming
apache runs as www-data)`
6.  Add SSH alias for each user to ~www-data/.ssh/config, like so: 
        Host git-USERNAME
        Hostname git.example.com
        User git
        IdentityFile /var/www/.ssh/key-USERNAME
        

7.  Add per user git remotes  
    `git remote add origin-USERNAME git@git-USERNAME:project.git` 

If you do not understand some of the steps above, you really shouldn't be
running this module. If you don't understand the implications of the
security warning above, you really shouldn't be running this module.
Support requests for users struggling with either of the above items will
be closed with a "by design" note - this is for your own protection.

## Limitations

features_git has only been tested on Linux and I have no interest in making
it work on Windows servers. It should work on OS X, but I don't have a Mac
to test on.

The error handling isn't as nice as I would like, but some of that is
because of limitations of FAPI and/or my knowledge of FAPI or both.

## Acknowledgement

Development of this module was sponsored by [Technocrat][1].

 [1]: http://technocrat.com.au
