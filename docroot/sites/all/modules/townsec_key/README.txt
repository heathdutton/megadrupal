Welcome to the Townsend Security Key Connection module!

This module provides the connection between Townsend Security's 
Alliance Key Manager and Drupal via the Encrypt Module.
This connection allows for secure remote key management for the encryption
process. Also it allows for NIST certified offsite encryption via
that Alliance Key Manager. 

This allows for local and remote encryption of sensitive data according to
best practices in the security industry. 


To Install:

1. Download this module alongside the Encrypt module from the Drupal.org
project page http://www.drupal.org/projects/encrypt

2. Extract the modules into your sites/all/modules directory
(or other site subdirectory if running a multi-site). 

3. Enable the modules via the module administration screen located at
/admin/modules.

4. Under the Encrypt administration page, you now have the ability to choose
your AKM as the key provider. Enter in your server settings and credentials
(including location of local key files for authentication).

5. Your keys are now safe!



***Helpful Hints****

1. IMPORTANT - keep your local authentication keys OUTSIDE the drupal root
directory accessible only to the server via your linux permissions. This is
important to prevent unauthorized access to your key management server.
It defeats the purpose to have remote key retrieval if you leave the
authentication in an easy to reach space.

2. At this time the remote encryption only supports up to 10 characters of
encryption. Soon this will be updated to allow for 16Kb of data to encrypt
in a single stream. For encryption of larger files, please contact
Townsend Security to help with the customization.
