## Contents of this file

* Introduction
* Requirements
* Recommended modules
* Installation
* Configuration and use
* Limitations
* Troubleshooting
* Similar projects
* Maintainers

## Introduction

This module does not *protect* your site's file integrity. Only you
can do that by configuring the site correctly and by not installing
insecure software.  It can be used to *check* a Drupal site for file
integrity breeches.

This module lets the site maintainer “fingerprint” an entire site
(except the files below the `public://` upload directory) when it is
in an untainted state.  It can then be configured to periodically
compare the site to this “fingerprint”, and report the following:

* modified files and directories;
* potential back-doors (files added to the site);
* files removed from the site;
* files writeable by the web-server.

This module should only be used on a *stable* production site to alert
the site maintainer as a first line of defense against file integrity
breeches.

Note that this module is not suitable for use on a development site,
or an unstable site where changes to the files that makes up the site
are frequent and deliberate.

The module is also not suitable for a site where

To be able to use this module, you first need to create a
“fingerprint” of the site when it is in an untainted state.  This
“fingerprint” will later be used as reference to detect modifications
to the site.

This module will not help you detect modifications *unless* you've
created a fingerprint. If you want to check file integrity and you
have not yet created a “fingerprint”, consider using the
[**Hacked!**][1] module.

## Requirements

* [**Advanced help hint**][2]:
  To hint about how to get `README.md` displayed.

## Recommended modules

* [**Advanced Help**][3]:
  When this module is enabled, the project's `README.md` may be
  displayed on the screen.
* [**Markdown filter**][4]:
  When this module is enabled, display of the project's `README.md`
  will be rendered with the markdown filter.

## Installation

Install this module as you would normally install a contributed drupal
module. See: [Installing modules][5] for further information.

By default, the module allocates 255 characters for the path field in
the database.  This is suffiscient for most sites and is also
compatible with the [InnoDB storage engine][13] used by some
databases.

If this is not suffiscient, you may increase the size by adding a line
that specifies a longer field to `settings.php` *before* you install
the module. Example:

    $conf['file_integrity_maxpath'] = 512;

If the module is already installed, and you want to increase this
field length, uninstall it, set the limit you want, and then reinstall
it.

If increasing `file_integrity_maxpath` results in the following
PDOException, the field length exceeds the database storage engines
capability:

    PDOException: … Specified key was too long; …

## Configuration and use

Make sure you have read the section “Notice” below before creating the
first fingerprint for the site.

To create a “fingerprint”, navigate to *Administration » Configuration
» System » File integrity*.

Press the *Create and save site fingerprint* button to create a
reference suite of MD5 checksums for later comparison.

Before you create a “fingerprint”, you need to make sure that the site
is in an untainted state.  The recommended practice is to create a
“fingerprint” just after you've finished building the site &ndash;
before it is exposed to the Internet.

If you modify the site (e.g. update core, or install, modify, update,
or remove a module), you need to create a new “fingerprint” just after
doing so.  Otherwise, your own modifications will be reported as
potential tampering.

Note that creating a fingerprint for a large site may take a long time
(several minutes).

After creating a fingerprint, you may set up the site to periodically
check its file integrity against the saved fingerprint.  To configure
automatic checks, expand “Settings for file integrity”.

The first settings is a radio button that may be used to disable or
enable the automatic file integrity check.  Set this to “enabled” to
enable the check.

Checks are done during cron runs. To prevent time-outs, you may want
to spread checks out over several cron runs.  Indicate the number of
files to check each time crons runs, or “all” to check all files each
time.

You may also indicate that checking should be suspended between
certain hour of the day (24 hour clock). You may want to do this
during peak hours to avoid loading the server. Setting this to
“always” will never suspend checking.

The final three radio buttons can be used to set these flags:

* Skip checksum check at each cron run. By default, a checksum of the
  contents of all the files will be computed at the end of each cron
  run and compared to the stored checksum.  On a large site with a lot
  of files, this computation will take a very long time, and may even
  cause a cron time-out.  Owners of large sites may want to turn this
  check off. Turning it off will make cron run faster, but it may also
  delay intrusion detection.

* Log progress at the end of each cron run. This will print a message
  about how many files processed so far to the log.

* Notifiy site administrator by email about potential problems. This
  will send an email to site administrator if a file fails the
  integrity check.

In addition to the automatic checks performed during cron runs, you
may at any time do a manual check. To do so, press the *Compare
current site with saved fingerprint* button to manually compare the
current site with the saved fingerprint.  Note that comparing a
fingerprint for a large site may take a long time (several minutes).

To do an instant checksum check, or to read the report generated by
automatic checks, navigate to *Administration » Reports » File
integrity*.

To do an instant checksum check, expand “Instant file checksum check”
(at the top of the page), and press “Check checksum”.  If the check
fails, you need to do visit the file integrity configuration page and
compare fingerprints to see names of the affected files.  (Note that
the instant checksum check may not work, but instead produce timeouts,
if the site is large.)

To truncate the report, expand “Clear log messages” (above the
report), and press “Clear log messages”.

## Limitations

The module, as currently designed, is less suitable for large sites.
It works well for sites with less than 6000 files, but becomes less
usable if the site is larger than this.  The function that computes
the site checksum do it for *all* the files that make up the site in a
single pass.  On a large site, this may cause timeouts, and it may be
necessary to disable this computation to avoid timeouts (there is a
checkbox *Skip checksum check at each cron run* that let lets you do
this. Large sites will be better served by an intrusion detection
system that is independent of Drupal, such as [**OSSEC**][11].
However, I shall welcome patches that makes the module work better on
large sites.

The module will fingerprint everything below the Drupal root except
the file upload areas.  There exists a [feature request][12] to allow
the administrator to specify folders to ignore.  .

This module is *not* the fast. If your site consists of a lot of
files, it will take some time to generate or check a fingerprint.

The module uses MD5 for file integrity checks.  The limitations of MD5
are dicussed in “[Using MD5 for file integrity checks][6]”.

If an attacker gain write access to this module's files, he may modify
them to always report a matching fingerprint or checksum (for
instance, see [this question on SE Information Security][7]).  This
module is vulnerable to this type of attack.  Do not rely on this
module alone to monitor file integrity on your site.

The [**Hacked!**][1] module uses a different method to verify file
integrity and should be used to check file integrity if you believe
the module itself, or the saved “fingerprint”, is tainted.  When you
do that, it is recommended that you take your site offline and
download a fresh copy of **Hacked!** from Drupal.org.  This will
protect you from using a tainted version of **Hacked!** for checking.

##Notice

**File integrity check** considers having files (outside of the
`public://` upload directory) writable by the web-server a security
hazard, and will warn about such files during fingerprinting.  If
you're installing this module for the first time, make sure this has
been fixed *before* you fingerprint the site for the first
time. (**File integrity check** will produce a lot of warnings if you
don't do this.)

**Explanation:** Having executable PHP-files *writable* by the
web-server is a major security hazard. This means that anyone that
manages to compromise the web-server can escalate to a PHP-injection
attack. The web-server has a very large attack surface, so it is one
of the easiest targets for hackers attacking a Drupal site.

I recommend that the files that are part of Drupal is *not* owned by
web-server user, but by some other user (for example the site owner),
and having them readable (but not writeable) by the web-server *group*
(mode bits 640).  For example (`joe` is the site owner and `apache`
is the web-server group):


    -rw-r-----   joe apache   index.php

In the directory `cli_utils`, there is a CLI utility for the bourne
shell named `set_owner_and_perms.sh` that will do this. You need to
configure it for your site before you run it.

## Similar projects

The following projects may be applied to use cases that are similar to
the use case this module is designed to handle:

* [**Hacked!**][1]:
  This Drupal module differs from the **File integrity** module by not
  being designed to do used on a production site to perform automatic
  and periodic checks.  Instead, it will verify integrity of core and
  modules on demand by downloading a untainted copy from Drupal.org
  and compare.  Note that it will not detect back-doors.

* [**MD5 Check**][8]:
  This Drupal module differs from the **File integrity** module by
  only monitoring the directories of installed modules, and by
  comparing the current version to the previous, instead of comparing
  to a saved “fingerprint”.  It does not send email, but log potential
  integrity breeches using watchdog.

* [**Git**][9]:
  This is not a Drupal module. It is a general source control system
  that may also be used to monitor changes to a set of files.

* [**inotify-tools**][10]:
  This is not a Drupal module, but a C library and a set of CLI-tools
  that can be used to monitor and act upon filesystem events.

* [**OSSEC**][11]:
  This is not a Drupal module, but an open source host-based intrusion
  detection system that performs log analysis, file integrity
  checking, policy monitoring, rootkit detection, real-time alerting
  and active response.


## Maintainers

* [gisle](https://www.drupal.org/u/gisle)

Any help with development (reviews, patches) are welcome.

[1]: https://www.drupal.org/project/hacked
[2]: https://www.drupal.org/project/advanced_help_hint
[3]: https://www.drupal.org/project/advanced_help
[4]: https://www.drupal.org/project/markdown
[5]: https://drupal.org/documentation/install/modules-themes/modules-7
[6]: http://security.stackexchange.com/questions/34488/using-md5-for-file-integrity-checks
[7]: http://security.stackexchange.com/questions/19439/how-to-make-a-simple-file-integrity-checker
[8]: https://www.drupal.org/project/md5check
[9]: https://git-scm.com/
[10]: https://github.com/rvoicilas/inotify-tools/wiki
[11]: http://www.ossec.net/
[12]: https://www.drupal.org/node/2600624
[13]: https://www.drupal.org/node/2556631
