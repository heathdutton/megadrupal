===== Report builder ======
A module by...
          ____         ,--.                                             ,--.
        ,'  , `.   ,--/  /|   ,---,           ,---,       ,---,       ,--.'|
     ,-+-,.' _ |,---,': / '  '  .' \        .'  .' `\  ,`--.' |   ,--,:  : |
  ,-+-. ;   , ||:   : '/ /  /  ;    '.    ,---.'     \ |   :  :,`--.'`|  ' :
 ,--.'|'   |  ;||   '   ,  :  :       \   |   |  .`\  |:   |  '|   :  :  | |
|   |  ,', |  ':'   |  /   :  |   /\   \  :   : |  '  ||   :  |:   |   \ | :
|   | /  | |  |||   ;  ;   |  :  ' ;.   : |   ' '  ;  :'   '  ;|   : '  '; |
'   | :  | :  |,:   '   \  |  |  ;/  \   \'   | ;  .  ||   |  |'   ' ;.    ;
;   . |  ; |--' |   |    ' '  :  | \  \ ,'|   | :  |  ''   :  ;|   | | \   |
|   : |  | ,    '   : |.  \|  |  '  '--'  '   : | /  ; |   |  ''   : |  ; .'
|   : '  |/     |   | '_\.'|  :  :        |   | '` ,/  '   :  ||   | '`--'
;   | |`-'      '   : |    |  | ,'        ;   :  .'    ;   |.' '   : |
|   ;/          ;   |,'    `--''          |   ,.'      '---'   ;   |.'
'---'           '---'                     '---'                '---'


===== Overview =====
Report Builder helps narrow down and organize views results in a dynamic report.
Report builder helps users to to create a table or chart with each data point
sourced by an individual view.  Each view has its own set of exposed filters
which can be adjusted.  This allows users to easily create graphs or tables that
summarize drupal data, which they can reconfigure and play around with by
changing the exposed filters.

===== Example Use Case =====
You're building a helpdesk ticket site for a client.  The client wants to be
able to generate reports of the ticket data and you don't want them mucking
around in the views UI and messing up your delicate views.  You generate a
report builder view which totals all of the tickets.  You add exposed filters
for ticket priority, submitted date, and the 'assigned to' user.  The client can
then create any of the following reports as both tables and graphs:
-Total TIckets by Priority
-Total Tickets by Month
-# of Tickets Assigned to Each User.

Without the views UI!

===== Installation =====
Nothing special to do here.  Install as you would any Drupal module.  Report
builder depends on:

Views - http://drupal.org/project/views
Ctools - http://drupal.org/project/ctools
Google Chart API - http://drupal.org/project/chart


===== Getting Started =====
Report builder includes a sample view with the Report Builder display type so
you can see how a view could be set up.  Generally the report builder views
displays should have 1 field and 1 result with a set of exposed filters.  Most
likely "Use Aggregation" will be used to generate sums, averages, etc.  Also
included are 2 sample reports based on the view.  Take a look at
admin/structure/report-builder.

===== Roadmap =====
1) Add a plugin system for display plugins (not just tables or Chart API charts)
2) Make reports have more than one row of data.

===== Mainteners =====
This module is maintained by mkadin (http://drupal.org/user/1093166). I would
love to have some help if you're interested!
