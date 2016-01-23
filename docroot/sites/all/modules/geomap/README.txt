********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: Geomap 
Initial Author: Peter Brownell
Contributors & Drupal 7 upgrade: Jeremy J. Chinquist [jjchinquist]
Sponsors: Code Positive [www.codepositive.com] 
          and school of everything [www.schoolofeverything.com]
Drupal Core Version: 7.x
Branch: 3.x
********************************************************************
DESCRIPTION:

  The Geomap API module will render a Google map in a block.
  
  The locations placed on the google map are obtained by 
  analysing the current page for GEO microformat informaton.
  When this info exists, a map will be rendered, when there 
  is no location information, no map will appear. 
  
  This module has been disigned to work with the geonames_cck
  module, which outputs it's data in the correct format. 
  
  More information on GEO Microformats: 
    http://microformats.org/wiki/geo 
    
  The minimal Geo Microformat includes the title <div> and lat/lng <abbr> tags
  as follows:
  
  <div class="geo" title="Canterbury">
    [displayed title: e.g. Canterbury, United Kingdom]
    <abbr class="latitude" title="51.2667">[display form of latitude: e.g. N 51� ZZ.ZZZ]</abbr>
    <abbr class="longitude" title="1.08333">[display form of longitude: e.g. W 1� ZZ.ZZZ]</abbr>
  </div>
    
  The 3x branch supports a wider range of geo data than the microformat
  specification to make the module customizable.
  
  Example data: 
  <div class="geo" title="Canterbury">
    [displayed title: e.g. Canterbury, United Kingdom]
    <abbr class="latitude" title="51.2667">[display form of latitude: e.g. N 51� ZZ.ZZZ]</abbr>
    <abbr class="longitude" title="1.08333">[display form of longitude: e.g. W 1� ZZ.ZZZ]</abbr>
    <div class="marker" >
      <div class="infowindow">
        <div class="infowindow-text">[html or plain text for window]</div>
        <div class="infowindow-option" name="<name>" value="<value>" />
      </div>
      <div class="icon" src="" >
        <div class="option" name="<name>" value="<value>" />
      </div>
    </div>
    <div class="node" nid="[nid]" link="[full drupal path including http://... ]" />
  </div>

********************************************************************
INSTALLATION:

  Note: It is assumed that you have Drupal up and running.  Be sure to
    check the Drupal web site if you need assistance.  If you run into
    problems, you should always read the INSTALL.txt that comes with the
    Drupal package and read the online documentation.

  1. Place the module directory in to Drupal
        sited/.../modules.

  2. Enable the module by navigating to:

     Administer > Modules

  Click the 'Save configuration' button at the bottom to commit your
    changes.
    


********************************************************************
CONFIGURATION

  1. Go to Administer > Settings > Geomap 
      
      Set your google map key. 
      You can obtain this key (for free) from 
        http://code.google.com/apis/maps/signup.html
  
  2. Go to Administer > Site Building > Blocks
       Assign the "Geo Microformat Googlemap" to a region
       
  3. Add some geo microformats to your pages. 
     A. The best way to do this is to use the geonames_cck module, 
        which uses the geonames service (commerical service only)
        and add a location field to a node.
     
     B. Add the rendered geo microformat directly to the node body or
        inside of a custom CCK field. If it cannot be done manually,
        maybe the CCK field contents can be calculated in a custom module or
        through the computed_field module.
        
     B1. Use several Drupal Core fields in cooperation with Views2 so that multiple +
         fields are rendered as a geomicroformat and printed to the page 
         automatically.
     
     C. Add Geo Microformats markup directly to your page. This is the fastest method. 
        
  4. Customise your installation in Administer > Structure > Content > Geomap
     A. Provide the Google Maps API Key
     B. Override the .js and .css files by placing them inside the
        folder "customisations"
      
********************************************************************
ACKNOWLEDGEMENTS

Javascrpt code: 
  Based on jQuery googleMap by Dylan Verheul <dylan@dyve.net>