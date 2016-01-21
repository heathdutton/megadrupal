<!DOCTYPE html>
<html>
  <head>
    <meta content="text/html; charset=UTF-8" http-equiv="Content-Type" />
  </head>
  <body>

    <p><?php print $name; ?>,</p>

    
    <p>Here are your daily Bible readings for <?php print date('l, F j, Y'); ?>, provided by 
      Family Community Church.</p>


    <strong>Today's Readings:</strong>
    <ul>
      <li><?php print biblereadings_format_psalms($readings); ?></li>
      <li><?php print biblereadings_format_proverb($readings); ?></li>
      <li><?php print biblereadings_format_regular_reading($readings); ?></li>
    </ul>

    <p>Visit <a href="http://www.familycc.org">www.FamilyCC.org</a> to download the full "Read the Bible in a Year" plan.</p>

    <p>May your day be filled and guided by the Blessing of the Lord!</p>

    <p>
      Family Community Church<br />
      phone: (916) 334-7700<br />
      email: info@FamilyCC.org<br />
      web: http://www.FamilyCC.org
    </p>
    
  </body>
</html>