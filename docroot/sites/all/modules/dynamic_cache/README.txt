== DYNAMIC CACHE == 

-- Summary -- 

Dynamic Cache allows developers to dynamically disable Drupal's standard 
page cache for arbitrary conditions, by setting the global 
$conf['cache'] variable to CACHE_DISABLED (which is 0, or false) inside 
of their own module's hook_boot() implementation. 

Dynamic Cache does not include any user interface, it is intended for 
usage by module developers who wish to have more control over Drupal's 
standard caching behaviour. 

There is discussion of modifying core to allow this functionality 
natively here: "Allow hook_boot() to disable page cache" 
<http://drupal.org/node/322104>, but this has not even made it into D7, 
much less D6, which is why I decided to contribute this module, based on 
code from a live production site I developed, which enables this 
functionality immediately in D6. Hopefully a similar approach will be 
possible for D7, however this is not something I have investigated at 
this time. 

-- Usage -- 

Enable the module, then in your own custom module, in hook_boot(), you 
can set the global variable $conf['cache'] = CACHE_DISABLED, and this 
will properly disable Drupal's standard cache (it will not work for 
aggressive caching, which does not even invoke hook_boot). 

For example, you might want to disable page caching if the user has any 
items stored in "shopping cart" type session data. With the Dynamic 
Cache module enabled, you could use code similar to the following to 
make this happen: 

MYMODULE_boot(){ if( !empty( $_SESSION['cart_items'] ) ){ 
$GLOBALS['conf']['cache'] = CACHE_DISABLED; } } 

-- How It Works -- 

When the global cache variable is disabled, this module essentially 
"hijacks" the standard Drupal bootstrap process by never actually 
returning from hook_boot(); it completes the rest of the bootstrap 
process itself. 

There is a detailed description of this here: "Disable page caching 
conditionally, by hijacking the standard Drupal bootstrap process 
(working)" <http://drupal.org/node/875152> 

-- Important Note -- 

Because Dynamic Cache never returns from hook_boot(), it is critical 
that Dynamic Cache is executed AFTER any other modules that use 
hook_boot(). This means Dynamic Cache must be given a higher weight than 
all other modules which use hook_boot(). Dynamic Cache will install itself 
with a very high weight, but care should be taken to ensure no other boot 
modules are given a higher weight. Module weights can be inspected 
directly in the database's "system" table, or by using Util's Module 
Weights sub-module <http://drupal.org/project/util> 

-- Author/Maintainer -- 

Brian Cairns (brian_c) bcairns at gmail dot com
