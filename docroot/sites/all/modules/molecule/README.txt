-- SUMMARY--

The Drupal module Molecule adds an input filter to Drupal to show the contents
of JCAMP-DX files. It enabled users to easily integrate JSSpecView and JSMol 
applets into their Drupal sites without technical knowledge. 

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

-- CONFIGURATION --

* Edit or create an input format that uses the new Molecule input filter.

-- USAGE --
The Molecule module defines three types of display options for displaying 
JCAMP-DX files:
* Spectrum: Show the spectrum contained in the JCAMP-DX file
* Molecule: Show the molecule contained in the JCAMP-DX file
* Spectrum with molecule: Show both the molecule and the spectrum in the 
  JCAMP-DX file and link them.
 
To use one of the display options create an empty-element tag in the page 
content. Please note that you have to run the Molecule input filter to turn 
the tags into an applet. Attributes should be enclosed with double quotes. 


---Spectrum---
To display a spectrum use the <spectrum /> tag

Attributes:
file 				(required) relative or absolute link to the JCAMP-DX file.

width	 			width of the applet in pixels (without 'px'). Defaults to 
                    500 if no value is given.
				
height 				height of the applet in pixels (without 'px'). Defaults to 
                    400 if no value is given.
				
disablemenu			"true" to disable the file part of the right-click menu. 
					"false" to enable it. Defaults to true if no value given.
				
peak				enter peak type and peak model value (for example IR 1.15) 
                    to select a peak on load. Doesn't work for anything other 
					IR (yet).
				
parameters			enter jspecview script to be passed on directly to the 
                    applet.

closedquestionsync	enter a closedquestion inlinechoice id. This inlinechoice 
                    will be kept up to date with the id of the selected peak.

---Molecule---
To display a molecule use the <molecule /> tag
file 				(required) relative or absolute link to the JCAMP-DX file. 
                    Enter a space with a number behind the link to select a 
					particular model from the file (for example a vibration).

width	 			width of the applet in pixels (without 'px'). Defaults to 
                    400 if no value is given.

height				height of the applet in pixels (without ‘px’). Defaults to
					the width of no value is given.

vibration			set to "on" enable vibration of the molecule. Set to "off"
					to disable vibration. Defaults to "off".
					
disablemenu			"true" to disable the file part of the right-click menu. 
					"false" to enable it. Defaults to true if no value given.
					
parameters			enter jmol script to be passed on directly to the applet.

background			enter a background in plain english (like "white") or as hex
					without a # (like FF00FF) 

---Spectrum with molecule---
To display a spectrum with a molecule use the <moleculewithspectrum /> tag
file 				(required) relative or absolute link to the JCAMP-DX file. 
                    Enter a space with a number behind the link to select a 
					particular model from the file (for example a vibration).
					
closedquestionsync	enter a closedquestion inlinechoice id. This inlinechoice 
			        will be kept up to date with the id of the selected peak.
					
spectrumwidth		The width of the spectrum applet. Defaults to 500 if no 
                    value is given.

spectrumheight		The height of the spectrum applet. Defaults to 400 if no 
                    value is given.

moleculewidth		The width of the molecule applet. Defaults to 400 if no 
                    value is given.

--Controlling the applet---
The applet can be controlled by calling the global Jmol.script(applet, script) name. Applet should be a 
reference to the applet and script should be the script.

In a file with mutiple spectra, one can switch between spectra by calling Jmol.script(JSVApplet0, "view 1.1"). Note
however that all modifiers that are defined on the <spectrum>, <molecule> or <spectrumwithmolecule> tags are only 
applied to the first view that is showed when the page is loaded. To apply them on other views append the commands to 
the script. For example: "view 1.2, enablezoom false" to disable zoom in the second view.
