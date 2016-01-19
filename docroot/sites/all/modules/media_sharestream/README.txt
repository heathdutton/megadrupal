CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Usage

INTRODUCTION
------------

Current Maintainers:

 * Brandon Neil <https://www.drupal.org/user/586386/>
 * Rick Porter <https://www.drupal.org/user/323030/>

Media: ShareStream adds ShareStream as a supported media provider.

The initial development of this module was heavily influenced by the
Media: YouTube and Media: Flickr modules.

REQUIREMENTS
------------

Media: ShareStream has one dependency.

Contributed modules
 * Media Internet - A submodule of the Media module.

INSTALLATION
------------

Media: ShareStream can be installed via the standard Drupal installation process
(http://drupal.org/node/895232).

You also need to provide a ShareStream Public Pick-n-Play Server url at Administer > Configuration > Media > ShareStream settings,
(admin/config/media/media-sharestream)

USAGE
-----

Media: ShareStream integrates the ShareStream video-sharing service with the
Media module to allow users to add and manage ShareStream videos as they would
any other piece of media.

Internet media can be added on the Web tab of the Add file page (file/add/web).
With Media: ShareStream enabled, users can add a ShareStream video by entering
its URL or embed code.
