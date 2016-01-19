<p>
  The <strong>Search Profiles</strong> panel enables you to save the query
  code generated in the panel above to a named, persistent file. The
  operation is simple:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_search_profile.png" width="800" />
</p>
<p>
  The name assigned to the search profile becomes the filename stored
  within the profiles subdirectory (the location of which is part of the
  internal QB configuration set at install time). If there are spaces in
  the name, they are converted to an underscore in the filename. Entered
  case is retained, so if you want to be consistent, you may always want to
  use lower case. Finally, the file name is given the <code>*.php</code>
  extension.
</p>