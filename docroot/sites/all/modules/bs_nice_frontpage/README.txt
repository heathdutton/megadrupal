--- About This Module ---
This module redirects the page /node to a custom page. 
Even in many big Drupal projects the page /node is often ignored by content and theming. In most cases the front page of drupal in "website settings" is changed from the default /node to a custom page. But the problem is: The page /node will exists and may display many nodes (in many cases even test nodes!!). You could set all nodes to not "display on frontpage" or simply active this tiny module. But the you will see the default page "Welcome to your new drupal website"
If a visitor of your page visits the page /node, he will either be  redirected to your real frontpage entered in "website settings" or you can enter a custom destination for the redirect. This could be done using the module path redirect but many projects don't use it so I decided to make a module for that.

--- Installation ---
Just place the module into your sites module folder, in most cases /sites/all/modules, and enable it
on the admin  module page /admin/build/modules.

--- Configuration and usage ---
If you want to redirect to you real frontpage, you don't have to do any configuration. If you'd like to redirect to a custom path, just visit the module settings at /admin/settings/bs_nice_frontpage and enter your destination path.

--- Maintainer ---
The maintainer of this module is manuelBS
