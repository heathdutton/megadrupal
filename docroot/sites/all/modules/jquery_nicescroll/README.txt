Provide integration with [jQuery Nicescroll](http://nicescroll.areaaperta.com)
-- jQuery plugin for a better scroll on desktop, mobile, and touch device.

## Requirements ##
* [Elements](http://dgo.to/elements)
* [Libraries API](http://dgo.to/libraries)

## Installation ##
1. Get the jQuery Nicescroll Library
  * Via Git

      ```
$ cd sites/SITENAME/libraries
$ git clone https://github.com/inuyaksa/jquery.nicescroll.git
      ```
  * Download
      * Download jQuery Nicescroll library
        from https://github.com/inuyaksa/jquery.nicescroll/archive/master.zip.
      * Extract compressed jQuery Nicescroll library.
      * Rename `jquery.nicescroll-master` directory to `jquery.nicescroll`.
      * Move `jquery.nicescroll` directory to your `sites/SITENAME/libraries`
        directory.

2. Enable jQuery Nicescroll Module
   ```
   $ drush en jquery_nicescroll
   ```

   Or enable via administration page `admin/modules`.

3. Configure jQuery Nicescroll Module
   Visit configuration page `admin/config/user-interface/jquery-nicescroll`.
