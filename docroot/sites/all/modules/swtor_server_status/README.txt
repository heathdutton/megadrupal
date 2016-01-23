swtor_server_status SWTOR - Star Wars The Old Republic - Server Status module.
==============================================================================
swtor_server_status SWTOR Server Status (C) 2013 by A.Wagner (http://aw030.de)
==============================================================================

swtor_server_status SWTOR Server Status module provides a block displaying 
server status informations for SWTOR game server.

The module requires php5 to run.

The following blocks are provided:

1) SWTOR - Star Wars The Old Republic - Server Status
   Displays actual server status information, display settings are 
   configurable:
   Select your desired server, optional display of a guild link, fraction 
   setting, language/locale option...
   

Installation
------------------------------------------------------------------------------

The installation is very simple:

  I. Installation
  II. Configuration

I. Installation

  1) Moving to module folder:
     Move the downloaded module package to sites/all/modules.
     => sites/all/modules/swtor_server_status/
     
  2) Activating the module:
     In administration back-end on the module configuration (admin/modules) 
     page you have now to install/activate the module and save the module 
     configuration.
  
  3) Activating and placing the block:
     After activation of the module e new block is added to the block 
     configuration page under structure/blocks. Now move the new block to 
     the desired region and make some block specific configuration.

II. Configuration

  1) After installation you should check the configuration page for the 
     swtor_server_status module and customize the settings. Options are self 
     explaining in the administration form.

  2) When you create custom styles, you can set the wrapper-elements option 
     to the desired behaviour. Wrapper elements are marked-up with 
     corresponding classes.
     

------------------------------------------------------------------------------
Module written by A.Wagner on http://drupal.org
------------------------------------------------------------------------------
