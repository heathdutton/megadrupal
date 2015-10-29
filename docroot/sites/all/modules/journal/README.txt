/* $Id: README.txt,v 1.8 2010/11/29 20:47:39 sun Exp $ */

-- SUMMARY --

Journal module adds additional fields to all forms in a Drupal site to allow
developers and site administrators to record and track all actions that have
been performed to setup a site or change its configuration.

Journal is primarily useful for developers and site administrators working in a 
team environment. Since Drupal is a full-fledged content management framework,
it is often not easy to communicate, track and audit all changes that have been
applied to a site. Even without contributed modules one is able to build a
totally customized site.

For a full description visit the project page:
  http://drupal.org/project/journal
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/journal


-- INSTALLATION --

* Copy Journal module to your modules directory and enable it on the modules
  page.

* Go to Administration » People » Permissions to allow users of certain
  roles to access the journal (which includes adding new journal entries) by
  enabling 'access journal' and 'access site reports' permissions.

* Optionally enable the Journal block at Administration » Structure » Blocks.
  This block displays all entries which have been made in the past for the URL
  currently visited.


-- USAGE --

* On any form, you or your co-workers/team are submitting to alter site
  settings and features, enter a meaningful description of your action into the
  journal entry form field.

* To view all changes, go to Administration » Reports » Journal entries.


-- CUSTOMIZATION --

* Until Form controller integration (http://drupal.org/node/228244) or a similar
  solution is implemented, you can override the variable 'journal_form_ids' in
  your site's settings.php to hide or require the journal entry field on
  specified forms.  To do this, you need to identify the $form_id of the target
  form and add it to your settings.php.  For example:
<code>
$conf['journal_form_ids'] = array(
  'fivestar_custom_widget' => 0,
  'guestbook_form_entry_form' => 0,
  'imagefield_js' => 0,
  'img_assist_header_form' => 0,
  'img_assist_properties_form' => 0,
  'link_widget_js' => 0,
  ...
);
</code>
  A value of 0 means that Journal will not show up in the given form; a value of
  1 indicates that a journal entry is required.


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) <http://drupal.org/user/54136>
* Stefan M. Kudwien (smk-ka) <http://drupal.org/user/48898>

This project has been sponsored by:
* UNLEASHED MIND
  Specialized in consulting and planning of Drupal powered sites, UNLEASHED
  MIND offers installation, development, theming, customization, and hosting
  to get you started. Visit http://www.unleashedmind.com for more information.

