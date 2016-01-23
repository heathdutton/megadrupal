# famfamfam.com icons
---------------------

The famfamfam.com icons module is a wrapper to integrate the famfamfam.com icon
packs with the Icon API module.

The icon packs are not included in this module, they need to be downloaded from
famfamfam.com. See Installation instructions for more details.



## Required modules
-------------------

- [Icon API](https://drupal.org/project/icon)



## Recommended modules
----------------------

- [Libraries API](https://drupal.org/project/libraries)



## Installation
---------------

Install the module as per standard Drupal instructions:
https://www.drupal.org/documentation/install/modules-themes/modules-7

Download the desired icon packs from famfamfam.com and extract them to their
associated directory in your 'libraries' directory (e.g., sites/all/libraries):

- [famfamfam_flag_icons](http://www.famfamfam.com/lab/icons/flags/famfamfam_flag_icons.zip)
- [famfamfam_silk_icons](http://www.famfamfam.com/lab/icons/silk/famfamfam_silk_icons_v013.zip)
- [famfamfam_mini_icons](http://www.famfamfam.com/lab/icons/mini/famfamfam_mini_icons.zip)
- [famfamfam_mint_icons](http://www.famfamfam.com/lab/icons/mint/famfamfam_mint_icons.zip)

**Note:** Ensure that all extracted directories match the above directory name
(e.g., sites/all/libraries/famfamfam_silk_icons).



## Makefile
-----------

For easy downloading of famfamfam.com icons and it's required/recommended
modules and/or libraries, you can use the following entries in your makefile:


```
projects[] = famfamfam

projects[] = icon

projects[] = libraries

libraries[famfamfam_flag_icons][download][type] = get
libraries[famfamfam_flag_icons][download][url] = http://www.famfamfam.com/lab/icons/flags/famfamfam_flag_icons.zip

libraries[famfamfam_silk_icons][download][type] = get
libraries[famfamfam_silk_icons][download][url] = http://www.famfamfam.com/lab/icons/silk/famfamfam_silk_icons_v013.zip

libraries[famfamfam_mini_icons][download][type] = get
libraries[famfamfam_mini_icons][download][url] = http://www.famfamfam.com/lab/icons/mini/famfamfam_mini_icons.zip

libraries[famfamfam_mint_icons][download][type] = get
libraries[famfamfam_mint_icons][download][url] = http://www.famfamfam.com/lab/icons/mint/famfamfam_mint_icons.zip
```

**Note:** It is highly recommended to specify the version of your projects, the
above format is only for the sake of simplicity.
