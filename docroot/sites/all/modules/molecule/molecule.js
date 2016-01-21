var molecule_syncClosedQuestion = {};
var molecule_syncDataClosedQuestion = {};
var molecule_syncApplets = {};

function molecule_syncCallback(app, msg) {
    var app_name = app.split('_', 2)[0];

    // Check if a sync is defined for the app_name
    if(molecule_syncClosedQuestion[app_name] != undefined) {
        // Get the identifier of the input field to sync with
        var identifier = molecule_syncClosedQuestion[app_name];

        // Only get the XML in the message
        var message_xml = msg.slice(msg.indexOf("<"), msg.indexOf(">")+1);

        // Get the selected peak from the xml in the message
        var index_param_start = message_xml.indexOf('index="')+'index="'.length;
        var index_param_end = message_xml.indexOf('"', index_param_start);
        var selected_peak = message_xml.slice(index_param_start, index_param_end);

        // Clear the selected peak if 'baseModel' is present in the message
        // because this would indicate that no peak is selected
        if(msg.indexOf('baseModel') != -1) {
            selected_peak = "";
        }

        // The applet is a stand-alone jmolapplet feed message back so selection does its job
        if(app_name.indexOf('jmolApplet') == 0) {
            var back_message = "Select:" + msg.substring(msg.indexOf(":") + 1);
            _jmolFindApplet(app_name).syncScript(back_message);
        }

        // Set the selected peak in the input field
        jQuery('input[name*="_[' + identifier + ']"]').val(selected_peak);
    }
    if(molecule_syncApplets[app_name] != undefined) {
        var receiver_app = molecule_syncApplets[app_name];
        Jmol._applets[molecule_syncApplets[app_name]]._syncScript(msg);

        return 1;
    }
}

function molecule_syncDrawCallback(data, app_name) {
    if(molecule_syncClosedQuestion[app_name] != undefined) {
        // Set smiles value of the applet
        var identifier = molecule_syncClosedQuestion[app_name];
        jQuery('input[name*="_[' + identifier + ']"]').val(data.src.smiles());
    }
    if(molecule_syncDataClosedQuestion[app_name] != undefined) {
        // Set data value of the applet
        identifier = molecule_syncDataClosedQuestion[app_name];
        jQuery('input[name*="_[' + identifier + ']"]').val(data.src.jmeFile());
    }
}

function molecule_setSyncClosedQuestion(app_name, identifier) {
    molecule_syncClosedQuestion[app_name] = identifier;
}

function molecule_setSyncDataClosedQuestion(app_name, identifier) {
    molecule_syncDataClosedQuestion[app_name] = identifier;
}

function molecule_setSyncApplets(spectrum_applet_name, molecule_applet_name) {
    molecule_syncApplets[spectrum_applet_name] = molecule_applet_name;
    molecule_syncApplets[molecule_applet_name] = spectrum_applet_name;
}

function molecule_waitReadyDraw(app_name) {
    // Wait for the applet to be defined
    if (typeof window[app_name] == "undefined" || window[app_name]._ready != true) {
        setTimeout(function(app_name) {
            return function() {
                molecule_waitReadyDraw(app_name);
            }
        }(app_name), 100);
    } else {
        // Set callback
        window[app_name]._applet.setAfterStructureModifiedCallback(function(app_name) {
            return function(data) {
                molecule_syncDrawCallback(data, app_name);
            }
        }(app_name));

        // If data field is available get data and update it
        if(molecule_syncDataClosedQuestion[app_name] != undefined) {
            // Set data value of the applet
            var identifier = molecule_syncDataClosedQuestion[app_name];
            window[app_name]._applet.readMolecule(jQuery('input[name*="_[' + identifier + ']"]').val());
        }
    }
}