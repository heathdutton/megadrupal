Usersnap
========

This Drupal module integrates the [Usersnap][] feedback widget with your Drupal
website. Before you install, be sure you have a Usersnap account and API key.


### Installation instructions

Install and enable this module [like you would any other module][]. Once you've
successfully installed the module, navigate to the configuration page at
`/admin/config/services/usersnap`. There, you must enter your Usersnap API key.

This module also exposes an "access usersnap" permission, which you can grant to
specific roles that you want to have access to the Usersnap feedback widget. You
may even wish to grant this permission to the "anonymous user" role to allow any
website visitor to send feedback through Usersnap.


### Exposed configurations

__Widget button text__

It's possible for you to configure the button text displayed to end-users when
they wish to provide feedback.

__Show widget on specific pages__

In addition to role-based visibility, you may wish configure specific pages
where the feedback widget is displayed. The configuration matches closely how
Drupal core's block visibility works. You may choose an inclusive or exclusive
filter in conjunction with a set of page paths (including wildcards).

__Widget position__

You may select the position of the feedback widget in the end-user's browser,
including bottom or middle left or right.

__Widget language__

Use this configuration to set the interface language of the feedback widget.

__E-mail field__

You may optionally include an e-mail field on the feedback widget. If you choose
to include it, you can provide placeholder text and choose to require the field
during the feedback collection process.

__Comment box__

Like the e-mail field, you may optionally include a comment box field on the
feedback widget. If you choose to include it, you can provide placeholder text
and choose to require the field.


### Multilingual support

This module supports several types of multilingual websites. For sites that only
support a single language, but the language is something other than English, the
widget interface language can be configured to any language supported by
Usersnap. Other user-facing strings can be configured using the desired language
as well.

For sites that are available in multiple languages, this module integrates with
the [i18n Variable module][], which supports multilingual configuration.


[Usersnap]: https://www.usersnap.com
[like you would any other module]: https://www.drupal.org/documentation/install/modules-themes/modules-7
[i18n Variable module]: https://www.drupal.org/project/i18n
