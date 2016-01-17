jQuery(document).ready(function() {
  // Replaces jZebra's sample createApplet(), see its function header
  jzebra_create_applet(Drupal.settings.jzebra.library_path);
  // Once the applet has loaded, execute the given function
  jzebra_monitor_loading(jzebra_initialize);

  jQuery('#printnow input:submit').click(function() {
    jzebra_print_content();
    return false;
  });
});

function jzebra_monitor_loading(callback) {
  $j = document.jZebra;
  if ($j != null && typeof $j.isActive != 'undefined') {
    if ($j.isActive()) {
      callback();
    }
  }
  else {
    window.setTimeout('jzebra_monitor_loading(' + callback + ')', 100);
  }
}

function jzebra_monitor_printer_finding(callback) {
  var $j = document.jZebra;
  if ($j != null) {
    if (!$j.isDoneFinding()) {
      window.setTimeout('jzebra_monitor_printer_finding(' + callback + ')', 100);
    }
    else {
      callback();
    }
  }
  else {
    alert("Error: Could not find printers because the jZebra applet is not loaded!");
  }
}

/**
 * Initialize the page by displaying the jZebra library version and populating
 * the printer list.
 */
function jzebra_initialize() {
  var $j = document.jZebra;
  jQuery('#jzebra_version').html($j.getVersion());
  jQuery('#jzebra_message_loading').hide();
  jQuery('#jzebra_message_version').show();
  jQuery('#jzebra_printers').show();

  jQuery('#jzebra_printer_list').html('');
  // It looks like jZebra can't populate its internal printer list without
  // searching for one, but this returns an error if a printer whose name starts
  // with the first parameter below isn't found. Oh well, let's trick it and
  // supply an empty name.
  $j.findPrinter("\\{dummy printer name for listing\\}");
  jzebra_monitor_printer_finding(jzebra_populate_printers);
}

/**
 * Callback for when initialize() finishes searching for printers. Displays the
 * printers in a bullet list, allowing the user to select one.
 */
function jzebra_populate_printers() {
  var $j = document.jZebra;
  var printersCSV = $j.getPrinters();
  jQuery.each(printersCSV.split(","), function(i,v) {
    var li = jQuery('<li><a href="#">' + v + '</a></li>');
    jQuery(li).click(function() {
      jQuery('#jzebra_printers ul li').css('font-weight', 'normal');
      jQuery(this).css('font-weight', 'bold');
      // This sets the an internal state in the applet that keeps track of the
      // 'current' printer. The current printer can later be retrieved with
      // $j.getPrinterName().
      $j.findPrinter(v);
      jzebra_monitor_printer_finding('jzebra_show_print_button');
      return false;
    });
    jQuery('#jzebra_printers ul').append(li); 
  });
}

/**
 * Callback for when the user selects a printer to use. Shows the print button.
 */
function jzebra_show_print_button() {
   jQuery("#printnow").show();
}

/**
 * Callback for the submit button, actually sents the file to the printer.
 */
function jzebra_print_content() {
   var $j = document.jZebra;
   $j.appendFile(Drupal.settings.jzebra.file_path);
   $j.print();
   alert("Data sent to printer. You can close the jZebra page.");
}

/**
 * Similar to jZebra's native createApplet() but allows us to specify the applet
 * path. Upon completion, the document.jZebra holds the reference to the applet.
 *
 * @param path
 *   The absolute location (URL) where jzebra.jar can be found.
 */
function jzebra_create_applet(path) {
   var jZebra = document.createElement('applet');
   jZebra.setAttribute('name', 'jZebra');
   jZebra.setAttribute('code', 'jzebra.PrintApplet.class');
   jZebra.setAttribute('alt', 'Could not load jZebra applet');
   jZebra.setAttribute('archive', path + '/jzebra.jar');
   jZebra.setAttribute('width', '0');
   jZebra.setAttribute('height', '0');
   // Put applet in its own container to support click-through plugin extensions
   // and Safari's inactive plugin system
   jQuery('#jzebra_applet_container').append(jZebra);
}
