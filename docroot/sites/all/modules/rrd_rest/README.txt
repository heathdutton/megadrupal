BASIC INFO
==========

This module provides a graphical dashboard based on RRD data.  Once enabled, 
this module gives an administrator an graphical overview of the performance
of the server.

The base graph class and plugins are adapted from the Collectd Graph Panel 
open source toolkit. http://pommi.nethuis.nl/collectd-graph-panel-v0-3/

Usage
==========

Prerequisite: 
Make sure the server has Collectd and RRDTool.  The Ubuntu and Debian versions
can be installed via apt-get

1) Install this module
2) Navigate to Configuration > System > RRD Configuration.  Make sure the data directory
and RRDTool binary are available.
3) Go to monitor-server/dashboard to see the Dashboard.

NOTE: To secure the api calls, you will need to put this module behind Apache Basic Auth.
Newer version will take advantage of the Services API for user authentication.

API
==========

The following endpoints are available for use. They require user permissions in order
to run.

- monitor-server/status
  Parameters: None
  
  Description: Return information about the RRDTool Exectuable and the RRDTool data 
  directory.  Determines if the executable and data directory exists.
  
  Return: JSON Object
          {"rrd_data_dir":"/var/lib/collectd/rrd","rrd_tool":"/usr/bin/rrdtool","server_ip":"192.168.0.1"}
  
- monitor-server/get-all-hosts
  Parameters: None
  
  Description: Lists all the hosts being tracked.
  
  Return: JSON Array
          ['server-1', 'server-2', 'server-3']

- monitor-server/get-supported-plugins
  Parameters: None
  
  Description: List all the plugins that have graph and data funtions  
  
  Return: JSON Array
          ["cpu","df","disk","interface","load","memory","processes","swap"]
          
- monitor-server/get-plugins-by-host
  Parameters: host = [HOST] - The host to lookup
              plugin = [PLUGIN] - *Optional Parameter* If set, only details about that plugin is returned
              details = [TRUE|FALSE] - *Optional Parameter* TRUE will return all the details. 
              FALSE will return high level plugin data
              only return the plugin.
  
  Description: List all the Collectd plugins enabled for a particular host.  Only plugins
  with graphing functions are returned.            
  
  The JSON Objects that are returned can then be used to create the Graphing and data urls below.
  
  Return (details=true): JSON Array of objects
                        [{"plugin":"df","plugin_instance":"","type":"df","type_instance":"dev-shm"},
                         {"plugin":"df","plugin_instance":"","type":"df","type_instance":"dev"},
                         {"plugin":"df","plugin_instance":"","type":"df","type_instance":"lib-init-rw"},
                         {"plugin":"df","plugin_instance":"","type":"df","type_instance":"root"}]
  Return (details=false): JSON Array
                        ["apache","cpu","df","disk","entropy","interface","irq","load","memory","processes","swap","users"]
                   
- monitor-server/get-graph-by-plugin
  Parameters: host = [HOST]
              seconds = [SECONDS] Interval of time
              plugin = For example: load
              plugin_instance =
              type = For example: load
              type_instance=
              image_preset = large | medium
              debug
  
  Description: Generate a graph based on a plugin, the instance, and type.  This api call is
  usually passed parameters in monitor-server/get-plugins-by-host
  
  Here are a couple of example calls:
  
  - Generate lage graph for CPU #1 of a quad core server for a 5 minute interval
    monitor-server/get-graph-by-plugin?host=[HOST]&seconds=600&plugin=cpu&plugin_instance=0&type=cpu&image_preset=large
  
  - Generate medium SWAP graph for one day interval
    monitor-server/get-graph-by-plugin?host=[HOST]&seconds=86400&plugin=swap&type=swap&image_preset=medium
    
  Return: Content-type: image/png
          An graph image
 
 - monitor-server/get-data-by-plugin
  Parameter: host = [HOST]
             seconds = [SECONDS] Interval of time
             plugin = For example: load
             plugin_instance =
             type = For example: load
             type_instance =
             data_set = computed | full
             format = csv - Only used if the data_set is full
  
  Description: Generate data export using the RRDTool fetch or xport command.  The data can be returned
  in computed form or in full form.  If the data_set=full, format=csv can be specified.  This will return 
  a CSV file with the data.  This is useful if there is a need to analyze data points in excel.
  
  With data_set=computed, the return is a JSON object with max, min, and average calculated.  The raw
  calculated number, an autoscaled number and the converted symbol are returned 
  
  Return: (data_set=computed): JSON Object
  
  Return: (data_set=full): JSON Object
  
  Examples:
  - Retrieve computed data about CPU Load
  monitor-server/get-data-by-plugin?host=[HOST]&seconds=86400&plugin=load&type=load
  
  {"shortterm":{"average":{"raw":0.21371428571422,"autoscaled":"0.21","symbol":""},
                "min":{"raw":0,"autoscaled":"0.00","symbol":""},
                "max":{"raw":1.993,"autoscaled":"1.99","symbol":""}},
   "midterm":{"average":{"raw":0.20493551748089,"autoscaled":"0.20","symbol":""},
              "min":{"raw":0,"autoscaled":"0.00","symbol":""},
              "max":{"raw":1.04,"autoscaled":"1.04","symbol":""}},
   "longterm":{"average":{"raw":0.1772655707337,"autoscaled":"0.18","symbol":""},
               "min":{"raw":0,"autoscaled":"0.00","symbol":""},
               "max":{"raw":0.68,"autoscaled":"0.68","symbol":""}}
  }  
  
  - Export the CPU Load data into CSV
  monitor-server/get-data-by-plugin?host=[HOST]&seconds=86400&plugin=load&type=load&data_set=full&format=csv
  

FUTURE DEVELOPMENT
==========

Make the API public.
