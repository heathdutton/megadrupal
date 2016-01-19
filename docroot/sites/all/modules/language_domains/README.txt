Language domains allows you to override the language specific domains configured
in your locale language admin settings.

When enabled, the domain setting set in the db (locale module) for a particular
language can be overriden in settings.php. You just need to add the $conf
entries as follows $conf['language_domains_<language prefix>'] = 'example.com';

This is particularly useful when you have multilingual domains, but don't
want to have to continually change them on your dev or staging site when
syncing your databases. You don't have to panic to change the domain
configurations in the database every time.

Example use:
-------------------------------------------------------------------

As an example my live site I may have the 'domain' locale property for French
(/admin/config/regional/language/edit/fr) set as: enfrancais.com and the
english domain may be set to inenglish.com.

Now on my laptop if I try setting up a development environment and try to
drush sql-sync with my database I will be pulling in that configuration and
I will be screwed trying to browse to the domains I've set up locally.
The only way to be able to change it is in the database itself since I cannot
even reach the admin page to change it locally. Panic sets in...

However if I enable the language_domains module I can override the default
domains in my settings.php (or settings.local.php if you are awesome) file.

To implement the override you simply add a $conf entry matching each of your
configured languages. So for my laptop example where I have English and French
domains I would add the following lines to settings.php:

$conf['language_domains']['en'] = 'inenglish.local';
$conf['language_domains']['fr'] = 'enfrancais.local';

-------------------------------------------------------------------

-- GETTING STARTED --

1. Add $conf['language_domains']['<language prefix>'] = '<override.domain>'; for
   each language you have enabled and configured on your site.
2. Install language_domains in the usual way (http://drupal.org/node/895232)
3. Flush at minimum menu caches.. or just flush them all.
4. Browse to your overridden domains.

Note: Remember to do all your sites.php and web server configurations to set up
      you domains first.


-- LINKS --

Project page: http://drupal.org/project/language_domains
Submit bug reports, feature suggestions: http://drupal.org/project/issues/language_domains

-- MAINTAINERS --

acrazyanimal - https://drupal.org/user/696648
