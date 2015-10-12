Trusted Modules
--------------------------------

Installation
=================

1. Go to /admin/modules and enable the "Trusted Modules" module.

2. Go to /admin/reports/trusted/certificates

3. Click "Download Drupal.org certificate"


Usage
=================

Go to /admin/reports/trusted/settings to configure if an untrusted module should be automatically disabled if it is enabled.

Go to /admin/reports/trusted to see a list of trusted and untrusted modules. This will only show enabled modules.


How it works
=================

In the following examples, we will use Drupal.org as the package repository. It can be replaced by your in-house package repository, or a 3rd party package repository.

- Drupal.org digitally signs the SHA-256 hash of every published version of every module on Drupal.org.

- Project maintainers digitally signs the SHA-256 hash of every published version of their module.

- Trusted Modules downloads the Drupal.org public certificate

- Trusted Modules examines all enabled modules on your website

  - looks for 4 files in the root of a module directory:

    1. /path/to/module/.module.cert.pem
         The module's public certificate in PEM format.

    2. /path/to/module/.module.cert.sig
         The Base64 encoded signature of the module's public certificate, signed by Drupal.org.

            ----BEGIN SIGNATURE----
            DVMeUvPg4dK6XgCSz43Ypebxx5ExhyKK2hC+x47ohrVhH5cxSSnzupYd+kv/cMhA6WsR5jKyCmtl18N0NWlkMgnjgNHlSa7snAknFguLgND6MC26hyKas5Vtxuvp6G/MVb8zA7r+
            D5GMFagj75nxZY7CW+vQQKzZXTZ2aEdy8NncnJ7VDwy/HVIkbmt/gaTHSyoKRQ1+/f2yMEb1SBtTpf1U42Or7DAhebfFvnavYQ4a30TXRGN/UD4NQKuEBIrV4qZY3pWDriDx6QzW
            NMUTfeoXHFC7xCrkAYZgEoN+OIzuPURvmk9VbghGINjajdRTdyTEHiNBrQdZzyLaUQGwyRYICRfYvcZMv6NzNZjVwC56JJ+qBzpE+M3dKRKv3PGfTq1uqZv4ygdNjYpeghsFCjN+
            czmU7mEhte0gIMwshvyksi5aqjrTYcd5p6tpbCiPY6sdNIrcDETEch2Dz+wLFCwYZnH2YgOEOIeYUVd8gmRIpRuXh6Ma0ej0PUiYGfs9hCyO1sM08BjAsra2xa/ftHyZWW1G9u1k
            bnZ1Y2VjSqk6Ckje/du+D1c99pwEtwJPq1PRqcx38kDW7VydjO38y3yOUz2Y/3OKyIZZKQFW8UtHjCYacreoEn/1i4LLCjzznh1nVqEBFOjjUw4LJ9W46iQEsVwwbYzfv2KLdZer8P8=
            ----END SIGNATURE----

    3. /path/to/module/.checksum
        The SHA-256 checksum. Eg.:

            sha256: ef035f2e6dc79f17d9ae4d14df7861e5994c9830cb4c296465f45f7f11d1fcd7

    4. /path/to/module/.checksum.sig:
        The Base64 encoded signature, signed by the project maintainer. Eg.:

            ----BEGIN SIGNATURE----
            DVMeUvPg4dK6XgCSz43Ypebxx5ExhyKK2hC+x47ohrVhH5cxSSnzupYd+kv/cMhA6WsR5jKyCmtl18N0NWlkMgnjgNHlSa7snAknFguLgND6MC26hyKas5Vtxuvp6G/MVb8zA7r+
            D5GMFagj75nxZY7CW+vQQKzZXTZ2aEdy8NncnJ7VDwy/HVIkbmt/gaTHSyoKRQ1+/f2yMEb1SBtTpf1U42Or7DAhebfFvnavYQ4a30TXRGN/UD4NQKuEBIrV4qZY3pWDriDx6QzW
            NMUTfeoXHFC7xCrkAYZgEoN+OIzuPURvmk9VbghGINjajdRTdyTEHiNBrQdZzyLaUQGwyRYICRfYvcZMv6NzNZjVwC56JJ+qBzpE+M3dKRKv3PGfTq1uqZv4ygdNjYpeghsFCjN+
            czmU7mEhte0gIMwshvyksi5aqjrTYcd5p6tpbCiPY6sdNIrcDETEch2Dz+wLFCwYZnH2YgOEOIeYUVd8gmRIpRuXh6Ma0ej0PUiYGfs9hCyO1sM08BjAsra2xa/ftHyZWW1G9u1k
            bnZ1Y2VjSqk6Ckje/du+D1c99pwEtwJPq1PRqcx38kDW7VydjO38y3yOUz2Y/3OKyIZZKQFW8UtHjCYacreoEn/1i4LLCjzznh1nVqEBFOjjUw4LJ9W46iQEsVwwbYzfv2KLdZer8P8=
            ----END SIGNATURE----



Why does each project have its own public certificate? Isn't one Drupal.org public certificate enough?
==========================================================================================================

The project environment can change for several reasons, any of which may dissuade me from upgrading to the next version.


1. Change of project ownership
 - allows me to do research on the new project maintainer to assess competency

2. Change of project namespace ownership
 - https://drupal.org/project/<project> no longer belongs to the previous project. May be a completely different project with the same name.

3. Change of package management domain ownership
 - eg. drupal.org no longer operated by Drupal community


We can be automatically alerted to these types of important changes when modules are signed by the project owner.


How is the Drupal.org public certificate downloaded?
=========================================================

It is currently obtained using the openssl s_client command:
openssl s_client -connect drupal.org:443 < /dev/null

In the future, Drupal may decide to publish the code signing key in another location. If so, we will update this.


Should project certificates be downloaded out-of-band, in case of a man-in-the-middle-attack?
=================================================================================================

Trusted Modules will automatically verify a project certificate by checking if it is signed by at least one of your trusted package repositories. This way, you will know if a project certificate is trusted and has not been altered.



