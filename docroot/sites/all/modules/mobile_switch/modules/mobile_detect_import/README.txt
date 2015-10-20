Mobile Detect Import module, README.txt

Update and import functions for the Mobile Detect PHP class.

Mobile Detect home: http://mobiledetect.net

DEPENDENCIES

Drupal 7.
The Mobile Detect PHP class from http://mobiledetect.net.

INSTALL

This module does not include the actual Mobile Detect class.
These must be downloaded or cloned from one of the download links
and installed in the Drupal site as a library.

See the README.txt of the Mobile Switch module for installtion
instructions.

ADMINISTER

Administer the Mobile Detect Import module under:
Administration -> Configuration -> User interface -> Mobile Switch
admin/config/user-interface/mobile-switch/mobile-detect-import

a)
Configure the 'Enable automatic version check'.

b)
The 'Refresh Mobile Detect' button can be used for a manual version check.

On a fresh installation or a initial use of the Mobile Detect class
it is recommended to check for a new version manually.

- Multi site installation

Administer Mobile Detect Import and the version files in
the default site installation.

HOW WORKS

The import function needs the enabled automatic version check or the
manually 'Refresh Mobile Detect' action in the module administration.

If a new version of the Mobile Detect class is detected, this version
will be downloaded and stored in the directory

sites/default/files/Mobile_Detect as file Mobile_Detect.php.

a)
When a new version is available, it is reported on

the 'Status Report' page,
the administration page of the Mobile Detect Import module.

b)
When a new version is reported

1)
copy the stored file to the Mobile Detect library directory
2)
OR download the new version an store the new Mobile Detect class file
in the Mobile Detect library directory.

DEVELOPMENT

The machine name of the Mobile Detect library: Mobile_Detect

Example PHP code to get library informations:

@code
$library = libraries_detect('Mobile_Detect');
@endcode

LIMITATION

No direct import of the Mobile Detect class in the library directory.
