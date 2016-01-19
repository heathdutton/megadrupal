Module Description

eeds FileMaker imports data from FileMaker into Drupal. The module connects to 
the FileMaker PHP API to retrieve a set of records. The records can be filtered 
by multiple criteria (e.g. surname = Smith, city = Dundee).

By leveraging the power of Feeds, it's possible to:

    Do a one-off import.
    Import at regular intervals.
    Map FileMaker fields to Drupal fields.

If you also install Feeds Tamper then you can carry out some additional 
formatting of the data before it gets added to Drupal.

Dependencies:

    Feeds
    
    Libraries
    
    FileMaker Server. Please note, this module has been tested with FileMaker 
    Server 11, it has not been tested with FileMaker Server 12, so I'd be 
    interested in feedback from FileMaker 12 users. There are plans to test 
    with FileMaker 12 in the not too distant future.
    
    The FileMaker PHP API should be added to the libraries/filemaker folder. 
    The API can be downloaded here by clicking the branches tab.


Instructions for Use

After enabling the module, there will be 2 additional Feeds plugins - FileMaker 
Fetcher and FileMaker Parser. It is intended to be used with the Node Processor 
in order to pull FileMaker data into Drupal nodes.

Visit the Feeds Importers page at Structure > Feeds Importers and add an 
importer.

Setup the General Feeds Settings

    Click 'settings' in the Basic Settings section.
    
    Under 'attach to content type' choose 'use standalone form'. Feeds 
    FileMaker has not been tested with 'attach to node', please feel free to 
    provide feedback on this functionality in the issue queue if you try it out.
    
    If you wish to import at regular intervals then you can do so via the 
    'periodic import' setting.
    
    Click 'save' when done.

Setup the Fetcher

    Change the Fetcher to FileMaker Fetcher.
    Click 'settings' next to FileMaker Fetcher and enter the FileMaker 
    connection settings and any criteria you require. Save when done.

Setup the Processor

    Change the Processor to Node Processor.
    
    Click 'settings' next to Node Processor. Choose the Bundle 
    (Drupal content type) you wish to import into. Choose any other settings 
    you require and click save.
    
    Click 'Mapping' in the Processor section. Map 'FileMaker Field' to each 
    of the targets you require. Click 'save' when done.

Setup the Parser

    Change the Parser to FileMaker Parser.
    Choose which FileMaker fields map to which Drupal Fields and click save 
    when done.

Import the Data
For one-time imports visit http://www.mysite.com/import. Be aware that large 
imports will crash when carried out through the web UI. If this happens then 
either setup periodic import or choose 'Process in background' in the Basic 
Settings.

If you chose a periodic import then the import will run at the interval you 
specified (via cron). I highly recommend using a module such as Ultimate Cron 
or Elysia Cron to aid with managing cron jobs.

Credits

Developed by College of Medicine, Dentistry and Nursing and College of Life 
Sciences at the University of Dundee

FileMaker parser took architectural inspiration from Feeds XPath Parser.
