
INTRODUCTION
------------
phpbb2drupal allows the migration of a phpbb installation to Drupal. The core
of this module imports data suitable for Drupal core, which includes: users,
forums as taxonomy, user avatars as pictures, forum topics and posts, any
attachments to those topics or posts.

The current version of this module is a complete rewrite from the old code
base following a long discussion about the direction this module should take.
To support D7 it was rewritten to use the migrate module as a base. A lot of
suporting code was taken from the old code base to help parse BBCode and clean
up posts, but essentially this is a complete rewrite.


INSTALLATION
------------
The module itself can be obtained from drupal.org in the ususal manner and
enabled as usual. Once this is done you'll need to go to the settings page:
admin/content/phpbb

There you'll need to tell the migration where to find the phpbb data.

The files location can either be a copy of the phpbb files inside the public
storage for Drupal, so in your sites/default/files directory, or equivalent, or
it can point to the domain where the files are held and can access them
directly. If you are poijting at the domain you'll need to follow the
instructions below to allow Drupal to access the files it needs.

The database settings just tell the migration where to access the phpbb
database. If you tell it to use the local database then the code will look for
the phpbb installation on the default database but will still use the supplied
prefix.

Once these settings have been saved, all available migrations, depending on
which modules you have enabled, will be available on the "Migrate" tab.


PREPARE PHPBB FILES
-------------------
If you are copying the files, or accessing them directly, you may find that
the phpbb .htaccess files get in the way. These files are there to secure your
various file stores against direct access but can be a pain.

The migration looks for files in the following directories:
/files
/images/avatars/upload

The easiest thing to do here is rename the .htaccess files on both those
diretories so they have no effect while the avatar and attachment migrations
are under way, then rename them back so you don't lose the security they
provide. Of course, of you're taking a local copy of the files needed, just
don't copy those files across.

Another word about local copies. The migration will still look to the full path
of where it expects to find the files, so under the directory you set in the
migration settings you need to make sure the source directories listed above
are there with their full path intact.


CAPABILITIES
------------
The current functionality of this module includes migration of the following
key parts of a PHPBB forum:

 - User accounts, with avatars as user pictures and working passwords, without
       the use of any external modules.
 - Forums imported as core forum taxonomy with hierarchy and containers intact.
 - All phpbb topics imported as forum posts.
 - All phpbb forum posts imported as forum post comments.
 - All phpbb forum attachments imported to a file field on either the topic
       entity or it's comments.
 - Private Messages imported to the privatemsg module (optional).
 
This also includes a migration from the PHPBB3 Blog Mod to Drupal blogs, which
is not part of core phpbb but thought might be handy for others.


ROADMAP
-------
 - Import of any custom BBCode to BBCode module
 - Import of any custom smilies
 - Better handling of post attachments using Media module
 - Import of poll data
