ABOUT

User bar provides an opportutnity for users to get update on the lastest content, activity etc from
the system without looking for each of this information. Modules can feed the information to the userbar
and notify end users if they need to take any action in the system.

INSTALLATION

Deploy the userbar module and enable it.

CONFIGURATION

The configuration can be accessed from the configuration UI
Configuration -> User Interfacr -> User bar

1. Select the auto-refresh interval. This the interval at which requests are placed from the end user's
browser to get the latest status from all modules. If this interval is set at a very low value, it can
overload the server on high traffic sites
2. Userbar can be used in two modes - docking or as a block. Select Dock user bar if you prefer this approach
or add the User bar block to one of the regions
3. Click on Set default userbar contents. This enables the admin to determine what contents are shown in the
userbar for all users. Users can further customize this if the have the "User can personalize userbar" permissions

PERMISSIONS:

1. Access userbar: The userbar is shown only if the user has this permission.
2. User can personalize userbar: Users with this permission can select the content for which they wish to get
continuous update. If user does not have this permission, the sitewide configuration is used.
