Hide Modules allows selected modules not being listed at the module
administration page except for selected roles.

It enables administrators to safely grant 'administer modules' permission to
other roles and keeping at the same time some modules out of the list to avoid
untrusted users enabling or disabling them.
This might come helpful when many roles should be able to configure modules,
but some critical modules should be enabled or disabled only by selected roles.

Example use case
Imagine you are User 1 (root) and you grant user administration permissions to
the 'site admin' role. You also installed the userprotect module to keep your
own root user safe from modification by other user admins at the same time.
If you grant 'administer modules' permission to site admins, they could disable
userprotect, thus leaving your root user editable.
Adding userprotect to the list of hidden modules would keep your site tight,
allowing other site builders to manage modules.

Usage
1. Fill the list of modules that should be hidden from module administrators.
   Module list values should be comma-separated.
   Note: 'Hide Modules' is always implicitly on this list.
2. Grant 'administer hide modules' permission.
   These users will be allowed to configure the hidden modules list.
   They also will be able to list all modules.
3. Grant 'view hidden modules' permission.
   These users will be allowed to list hidden modules.
   Hidden Modules itself is excluded to avoid users disabling it.
   Only users with administration rights can do this.
4. Check your current 'administer software updates' permission.
   Although users with this permission won't be able to enable or disable
   hidden modules, they can still list them in the 'Available updates' page.
