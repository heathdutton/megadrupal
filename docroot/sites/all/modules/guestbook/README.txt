
-- SUMMARY --

Guestbook provides a site guestbook and individual user guestbooks.  Guestbook
owners can delete and comment on guestbook entries.  User avatars are shown if
they are available.

* Site guestbook and user guestbooks.
* Configurable intro text per guestbook (displayed above entries).
* Address fields (email and website) for anonymous posters.
* Configurable input format for guestbook entries.
* Various display options (entries per page, pager position, submission date,
  email and website address, and comments).
* Email notification for new guestbooks entries.
* Integration with spam.module.

For a full description visit the project page:
  http://drupal.org/project/guestbook
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/guestbook


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

* Enable the module in administer >> Modules.


-- CONFIGURATION --

* Configure user permissions in administer >> Access control >> Guestbook.

* Customize the settings in administer >> Site configuration >> Guestbook.


-- FAQ --

Q: How can guestbook entries be displayed in user profiles?

A: You need to override the theme_user_profile() function.  Add the following
   code to your override function (without <code> tags):
<code>
  if module_exists('guestbook') {
    print module_invoke('guestbook', 'page', $user->uid);
  }
</code>
   See http://drupal.org/node/34484 for further information.


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - dev@unleashedmind.com

Previous maintainers / contributors:
* Henrik Brautaset Aronsen (hba)
* Jeff Warrington (jaydub) - http://drupal.org/user/46257
* Stefan M. Kudwien (smk-ka) - dev@unleashedmind.com
* JumpingJack@drupalcenter.de - http://drupal.org/user/56823 (port to 5.x)
* tenrapid - http://drupal.org/user/37587 (port to 4.7)

