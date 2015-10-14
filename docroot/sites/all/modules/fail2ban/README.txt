== FAIL2BAN ==

This module allows you to submit IP addresses to the firewall, to stop
them comment spamming your site or performing brute-force dictionary
attacks.

The fail2ban daemon monitors pplication and system log files such as
/var/log/auth.log or /var/log/apache/access.log and bans IP addresses by
updating or adding firewall rules. The expiry time of these bans and other
options can be configured.


== REQUIREMENTS ==

To make use of fail2ban you will need to install the fail2ban system utility.
For most distributions it is already packaged. See http://www.fail2ban.org/
for more info.


== INSTALLATION ==

1. Extract the module to sites/*/modules and enable it via the module
   admin page.

2. In the contrib/ directory you will find the jail.local.conf file,
   which contains configuration snippets. You can add these snippets to
   your /etc/fail2ban/jail.conf file or copy the entire file to
   /etc/fail2ban/jail.local. The latter is the recommended method.

   The filters are DISABLED by default in jail.local, so you need to
   enable the jails by setting "enabled = true".

   Copy the filters from contrib/filter.d to /etc/fail2ban/filter.d

3. Restart fail2ban via "/etc/init.d/fail2ban restart".

The system utility and this module are pre-configured with the localhost
address whitelisted. The module also automatically whitelists the IP
address of the administrator who enables it on the site.


== CONFIGURATION ==

Browse to http://example.com/?q=admin/config/people/fail2ban to change
fail2ban settings.

When the checkbox on http://example.com/?q=admin/content/comment site is
enabled and a comment is selected to be unpublished, the module will create
a log entry, which in turn causes fail2ban to block the IP address that the
comment was submitted from for the period set in /etc/fail2ban/jail.local.


== AUTHORS ==

Peter Lieverdink <me@cafuego.net>

Additional jails were provided by Tipi Koivisto <tipi@koivisto.eu>


== LICENSE ==

http://www.gnu.org/licenses/gpl-2.0.html
