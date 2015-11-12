Iron Curtain module

Developed by Reload, sponsored by Scribo.

This module allows the site admins to restrict access to certain paths or the
entire site, based on the visitor's IP address.

It allows users with a certain role to bypass the system, it allows a list of
paths to bypass the system and it allows you to make a list of paths which can
only be seen by whitelisted IPs, and another list which can not be accessed by
blacklisted IPs.

Go to Administration -> Configuration -> System -> Iron Curtain and set up.

Installation notes:

This module uses the M6WEB Firewall package https://github.com/M6Web/Firewall
which is installed by Composer. In order to make it work, Composer Manager needs
to be set up properly when enabling this module. If you experience any problems,
install Composer Manager and make it work, then diseable and re-enable Iron
Curtain, and the Firewall package should download properly.

Also note that this module requires PHP >= 5.4
